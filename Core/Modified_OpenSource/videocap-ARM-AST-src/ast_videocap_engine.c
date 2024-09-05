/***************************************************************
****************************************************************
**                                                            **
**    (C)Copyright 2009-2015, American Megatrends Inc.        **
**                                                            **
**            All Rights Reserved.                            **
**                                                            **
**        6145-F, Northbelt Parkway, Norcross,                **
**                                                            **
**        Georgia - 30071, USA. Phone-(770)-246-8600.         **
**                                                            **
****************************************************************
 ****************************************************************/

#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <asm/delay.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>
#if (LINUX_VERSION_CODE > KERNEL_VERSION(3,4,11))
#include <linux/dma-direction.h>
#endif
#include <linux/uaccess.h>

#ifndef SOC_AST2600
#include <mach/platform.h>
#endif

#include "ast_videocap_ioctl.h"
#include "ast_videocap_data.h"
#include "ast_videocap_functions.h"

static unsigned char EncodeKeys[256];
unsigned long JPEGHeaderTable[12288];

static void *prev_cursor_pattern_addr;
static uint32_t prev_cursor_status;
static uint32_t prev_cursor_position;
static uint16_t prev_cursor_pos_x;
static uint16_t prev_cursor_pos_y;
static uint32_t prev_cursor_checksum;
static uint32_t full_screen_capture;

static uint16_t cursor_pos_x;
static uint16_t cursor_pos_y;

static int ReInitVideoEngine = 0;
static int ISRDetectedModeOutOfLock = 1;

unsigned int TimeoutCount = 0;
unsigned int ModeDetTimeouts = 0;
unsigned int CaptureTimeouts = 0;
unsigned int CompressionTimeouts = 0;
unsigned int SpuriousTimeouts = 0;
unsigned int AutoModeTimeouts = 0;

struct ast_videocap_engine_info_t ast_videocap_engine_info;

static int ast_videocap_data_in_old_video_buf;

static int ModeDetectionIntrRecd;
static int CaptureIntrRecd;
static int CompressionIntrRecd;
static int AutoModeIntrRecd;

/* Status variables for help in debugging  */
int WaitingForModeDetection;
int WaitingForCapture;
int WaitingForCompression;
int WaitingForAutoMode = 0;
int CaptureMode = VIDEOCAP_YUV_SUPPORT;

static void *vga_mem_virt_addr;

/*  Timer related variables for Video Engine */
#define VE_TIMEOUT          1   /* in Seconds   */
unsigned long TimeToWait;
wait_queue_head_t mdwq;
wait_queue_head_t capwq;
wait_queue_head_t compwq;
wait_queue_head_t autowq;

/* Defines for Video Interrupt Control Register */
#define IRQ_WATCHDOG_OUT_OF_LOCK     (1<<0)
#define IRQ_CAPTURE_COMPLETE         (1<<1)
#define IRQ_COMPRESSION_PACKET_READY (1<<2)
#define IRQ_COMPRESSION_COMPLETE     (1<<3)
#define IRQ_MODE_DETECTION_READY     (1<<4)
#define IRQ_FRAME_COMPLETE           (1<<5)
#define ALL_IRQ_ENABLE_BITS (IRQ_WATCHDOG_OUT_OF_LOCK |  \
		IRQ_CAPTURE_COMPLETE | \
		IRQ_COMPRESSION_PACKET_READY | \
		IRQ_COMPRESSION_COMPLETE | \
		IRQ_MODE_DETECTION_READY | \
		IRQ_FRAME_COMPLETE)

/* Defines for Video Interrupt Status Register */
#define STATUS_WATCHDOG_OUT_OF_LOCK     (1<<0)
#define STATUS_CAPTURE_COMPLETE         (1<<1)
#define STATUS_COMPRESSION_PACKET_READY (1<<2)
#define STATUS_COMPRESSION_COMPLETE     (1<<3)
#define STATUS_MODE_DETECTION_READY     (1<<4)
#define STATUS_FRAME_COMPLETE           (1<<5)
#define ALL_IRQ_STATUS_BITS (STATUS_WATCHDOG_OUT_OF_LOCK |  \
		STATUS_CAPTURE_COMPLETE | \
		STATUS_COMPRESSION_PACKET_READY | \
		STATUS_COMPRESSION_COMPLETE | \
		STATUS_MODE_DETECTION_READY | \
		STATUS_FRAME_COMPLETE)

#define MAX_SYNC_CHECK_COUNT        (20)

#define BLOCK_SIZE_YUV444 (0x08)
#define BLOCK_SIZE_YUV420 (0x10)
#define VR060_VIDEO_COMPRESSION_SETTING 0x00080400

#define ASPEED_VIDEO_JPEG_NUM_QUALITIES	12
#define ASPEED_VIDEO_JPEG_HEADER_SIZE	10
#define ASPEED_VIDEO_JPEG_QUANT_SIZE	116
#define ASPEED_VIDEO_JPEG_DCT_SIZE	34

INTERNAL_MODE Internal_Mode[] = {
	// 1024x768
	{1024, 768, 0, 65},
	{1024, 768, 1, 65},
	{1024, 768, 2, 75},
	{1024, 768, 3, 79},
	{1024, 768, 4, 95},

	// 1280x1024
	{1280, 1024, 0, 108},
	{1280, 1024, 1, 108},
	{1280, 1024, 2, 135},
	{1280, 1024, 3, 158},

	// 1600x1200
	{1600, 1200, 0, 162},
	{1600, 1200, 1, 162},
	{1600, 1200, 2, 176},
	{1600, 1200, 3, 189},
	{1600, 1200, 4, 203},
	{1600, 1200, 5, 230},

	// 1920x1200 reduce blank
	{1920, 1200, 0, 157},
	{1920, 1200, 1, 157},
};
struct ast_videocap_jpeg_tile_info_t tile_info = {0};
static const unsigned int InternalModeTableSize = (sizeof(Internal_Mode) / sizeof(INTERNAL_MODE));

inline uint32_t ast_videocap_read_reg(uint32_t reg)
{
	return ioread32((void __iomem*)ast_videocap_reg_virt_base + reg);
}

inline void ast_videocap_write_reg(uint32_t data, uint32_t reg)
{
	iowrite32(data, (void __iomem*)ast_videocap_reg_virt_base + reg);
}

inline void ast_videocap_set_reg_bits(uint32_t reg, uint32_t bits)
{
	uint32_t tmp;

	tmp = ast_videocap_read_reg(reg);
	tmp |= bits;
	ast_videocap_write_reg(tmp, reg);
}

inline void ast_videocap_clear_reg_bits(uint32_t reg, uint32_t bits)
{
	uint32_t tmp;

	tmp = ast_videocap_read_reg(reg);
	tmp &= ~bits;
	ast_videocap_write_reg(tmp, reg);
}

int ast_videocap_remap_vga_mem(void)
{
	uint32_t reg;
	unsigned int total_index, vga_index;
	unsigned int vga_mem_size;
	/*reference AST 2300 / 2400 / 2500 / 2600 data sheet
	    VGA Memory Space map to ARM Memory Space
	*/
#if defined(SOC_AST2300) || defined(SOC_AST2400)
	unsigned int vga_mem_address[4][4]={{0x43800000, 0x47800000, 0x4F800000, 0x5F800000},
											{0x43000000, 0x47000000, 0x4F000000, 0x5F000000},
											{0x43000000, 0x46000000, 0x4E000000, 0x5E000000},
											{0x0, 	   0x44000000, 0x4C000000, 0x5C000000}};
#elif defined(SOC_AST2500) || defined(SOC_AST2530)
	unsigned int vga_mem_address[4][4]={{0x87800000, 0x8F800000, 0x9F800000, 0xBF800000},
											{0x87000000, 0x8F000000, 0x9F000000, 0xBF000000},
											{0x86000000, 0x8E000000, 0x9E000000, 0xBE000000},
											{0x84000000, 0x8C000000, 0x9C000000, 0xBC000000}};
#elif defined(SOC_AST2600)
	unsigned int vga_mem_address[4][4]={{0x8F800000, 0x9F800000, 0xBF800000, 0xFF800000},
											{0x8F000000, 0x9F000000, 0xBF000000, 0xFF000000},
											{0x8E000000, 0x9E000000, 0xBE000000, 0xFE000000},
											{0x8C000000, 0x9C000000, 0xBC000000, 0xFC000000}};
#endif

	/* Total memory size
		AST2050 / AST2150: MMC Configuration Register[3:2]
			00: 32 MB
			01: 64 MB
			10: 128 MB
			11: 256 MB

		AST2300 / AST2400: MMC Configuration Register[1:0]
			00: 64 MB
			01: 128 MB
			10: 256 MB
			11: 512 MB

		AST2500: MMC Configuration Register[1:0]
			00: 128 MB
			01: 256 MB
			10: 512 MB
			11: 1024 MB

		AST2600: MMC Configuration Register[1:0]
			00: 256 MB
			01: 512 MB
			10: 1024 MB
			11: 2048 MB
	*/
#ifdef SOC_AST2600
	reg = ioread32((void * __iomem)(ast_sdram_reg_virt_base + 0x04));
#else
	reg = ioread32((void * __iomem)SDRAM_CONFIG_REG);
#endif
	total_index = (reg & 0x03);

	/* VGA memory size
		Hardware Strapping Register[3:2] definition:
		VGA Aperture Memory Size Setting:
		    00: 8 MB
		    01: 16 MB
		    10: 32 MB
		    11: 64 MB
	*/
#ifndef SOC_AST2600
	reg = ioread32((void * __iomem)SCU_HW_STRAPPING_REG);
#endif

	vga_index = ((reg >> 2) & 0x03);
	vga_mem_size = 0x800000 << ((reg >> 2) & 0x03);

	vga_mem_virt_addr = (void * __iomem)ioremap(vga_mem_address[vga_index][total_index], vga_mem_size);
	if (vga_mem_virt_addr == NULL) {
		printk(KERN_WARNING "ast_videocap: vga_mem_virt_addr ioremap failed\n");
		return -1;
	}

	return 0;
}

void ast_videocap_unmap_vga_mem(void)
{
#ifdef SOC_AST2600
	if (ast_sdram_reg_virt_base != NULL)
		iounmap(ast_sdram_reg_virt_base);

	if (ast_scu_reg_virt_base != NULL)
		iounmap(ast_scu_reg_virt_base);
#endif

	if(vga_mem_virt_addr != NULL)
		iounmap(vga_mem_virt_addr);
}

#if defined(SOC_AST2600)
#define SCU_MISC_CTRL			0xC0
#else
#define SCU_MISC_CTRL			0x2C
#endif
#define SCU_MISC_CTRL_DAC_DISABLE		0x00000008 /* bit 3 */
#if defined(SOC_AST2400) || defined(SOC_AST2500) || defined(SOC_AST2530) || defined(SOC_AST2600)
#define SCU_MULTIFUN_CTRL4      0x8C
#define SCU_MULTIFUN_CTRL4_VPODE_ENABLE 0x00000001 /* bit 0 */
#define SCU_MULTIFUN_CTRL6      0x94
#define SCU_MULTIFUN_CTRL6_DVO_MASK     0x00000003 /* bit 1:0 */
#endif
static int ast_Videocap_vga_dac_status(void)
{
#ifdef SOC_AST2600
	if ((ioread32((void *__iomem)ast_scu_reg_virt_base + SCU_MULTIFUN_CTRL6) & SCU_MULTIFUN_CTRL6_DVO_MASK) == 0)
	{
		return (ioread32((void *__iomem)ast_scu_reg_virt_base + SCU_MISC_CTRL) & SCU_MISC_CTRL_DAC_DISABLE) ? 0 : 1;
	}
	else
	{
		return (ioread32((void *__iomem)ast_scu_reg_virt_base + SCU_MULTIFUN_CTRL4) & SCU_MULTIFUN_CTRL4_VPODE_ENABLE) ? 1 : 0;
	}
#else
#if defined(SOC_AST2400) || defined(SOC_AST2500) || defined(SOC_AST2530)
    if ((ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MULTIFUN_CTRL6) & SCU_MULTIFUN_CTRL6_DVO_MASK) == 0) {
        return (ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MISC_CTRL) & SCU_MISC_CTRL_DAC_DISABLE) ? 0 : 1;
    } else {
        return (ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MULTIFUN_CTRL4) & SCU_MULTIFUN_CTRL4_VPODE_ENABLE) ? 1 : 0;
    }
#else
	return (ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MISC_CTRL) & SCU_MISC_CTRL_DAC_DISABLE) ? 0 : 1;
#endif
#endif
}

static void ast_videocap_vga_dac_ctrl(int enable)
{
	uint32_t reg;

#ifdef SOC_AST2600
	reg = ioread32( (void *__iomem)dp_descriptor_base + 0x100 );
	if (enable){
		reg |= DPTX100;	
	}
	else
	{
		reg &= ~DPTX100;
	}
	iowrite32(reg, (void * __iomem)dp_descriptor_base + 0x100);
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + 0x00);
	// When this register is locked, the read back value of this register is 0x00000000
	if(reg == 0)
	{
		iowrite32(0x1688A8A8, (void * __iomem)ast_scu_reg_virt_base + 0x00); /* unlock SCU */
	}
	
	if ((ioread32((void *__iomem)ast_scu_reg_virt_base + SCU_MULTIFUN_CTRL6) & SCU_MULTIFUN_CTRL6_DVO_MASK) == 0)
	{
		reg = ioread32((void *__iomem)ast_scu_reg_virt_base + SCU_MISC_CTRL);
		if (enable)
		{
			reg &= ~SCU_MISC_CTRL_DAC_DISABLE;
		}
		else
		{
			reg |= SCU_MISC_CTRL_DAC_DISABLE;
		}
		iowrite32(reg, (void *__iomem)ast_scu_reg_virt_base + SCU_MISC_CTRL);
	}
	else
	{
		reg = ioread32((void *__iomem)ast_scu_reg_virt_base + SCU_MULTIFUN_CTRL4);
		if (enable)
		{
			reg |= SCU_MULTIFUN_CTRL4_VPODE_ENABLE;
		}
		else
		{
			reg &= ~SCU_MULTIFUN_CTRL4_VPODE_ENABLE;
		}
		iowrite32(reg, (void *__iomem)ast_scu_reg_virt_base + SCU_MULTIFUN_CTRL4);
	}
#else
	reg = ioread32((void * __iomem)SCU_KEY_CONTROL_REG);
	// When this register is locked, the read back value of this register is 0x00000000
	if((reg == 0)
	{
		iowrite32(0x1688A8A8, (void * __iomem)SCU_KEY_CONTROL_REG); /* unlock SCU */
	}

#if defined(SOC_AST2400) || defined(SOC_AST2500) || defined(SOC_AST2530)
    if ((ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MULTIFUN_CTRL6) & SCU_MULTIFUN_CTRL6_DVO_MASK) == 0) {
        reg = ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MISC_CTRL);
        if (enable) {
            reg &= ~SCU_MISC_CTRL_DAC_DISABLE;
        } else {
            reg |= SCU_MISC_CTRL_DAC_DISABLE;
        }
        iowrite32(reg, (void * __iomem)AST_SCU_VA_BASE + SCU_MISC_CTRL);
    } else {
        reg = ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MULTIFUN_CTRL4);
        if (enable) {
            reg |= SCU_MULTIFUN_CTRL4_VPODE_ENABLE;
        } else {
            reg &= ~SCU_MULTIFUN_CTRL4_VPODE_ENABLE;
        }
        iowrite32(reg, (void * __iomem)AST_SCU_VA_BASE + SCU_MULTIFUN_CTRL4);
    }
#else
	reg = ioread32((void * __iomem)AST_SCU_VA_BASE + SCU_MISC_CTRL);
	if (enable) {
		reg &= ~SCU_MISC_CTRL_DAC_DISABLE;
	} else {
		reg |= SCU_MISC_CTRL_DAC_DISABLE;
	}
	iowrite32(reg, (void * __iomem)AST_SCU_VA_BASE + SCU_MISC_CTRL);
#endif
#endif
}

void ast_videocap_engine_data_init(void)
{
	ModeDetectionIntrRecd = 0;
	CaptureIntrRecd = 0;
	CompressionIntrRecd = 0;
	AutoModeIntrRecd = 0;

	WaitingForModeDetection = 0;
	WaitingForCapture = 0;
	WaitingForCompression = 0;

	ast_videocap_data_in_old_video_buf = 0;

	prev_cursor_pattern_addr = NULL;
	prev_cursor_status = 0;
	prev_cursor_position = 0;
	prev_cursor_pos_x = 0;
	prev_cursor_pos_y = 0;
	prev_cursor_checksum = 0;

	memset((void *) &ast_videocap_engine_info, 0, sizeof(struct ast_videocap_engine_info_t));
	ast_videocap_engine_info.FrameHeader.CompressionMode = COMPRESSION_MODE;
	ast_videocap_engine_info.FrameHeader.JPEGTableSelector = JPEG_TABLE_SELECTOR;
	ast_videocap_engine_info.FrameHeader.AdvanceTableSelector = ADVANCE_TABLE_SELECTOR;
	
	init_waitqueue_head(&autowq);
}

irqreturn_t ast_videocap_irq_handler(int irq, void *dev_id)
{
	uint32_t status;

	status = ast_videocap_read_reg(AST_VIDEOCAP_ISR);
	ast_videocap_write_reg(status, AST_VIDEOCAP_ISR); // clear interrupts flags

	/* Mode Detection Watchdog Interrupt */
	if (status & STATUS_WATCHDOG_OUT_OF_LOCK) {
		ISRDetectedModeOutOfLock = 1;
	}

	/* Mode Detection Ready */
	if (status & STATUS_MODE_DETECTION_READY) {
		ModeDetectionIntrRecd = 1;
		wake_up_interruptible(&mdwq);
	}

	/* Capture Engine Idle */
	if (status & STATUS_CAPTURE_COMPLETE) {
		CaptureIntrRecd = 1;
		if (!AUTO_MODE) {
			wake_up_interruptible(&capwq);
		}
	}

	/* Compression Engine Idle */
	if (status & STATUS_COMPRESSION_COMPLETE) {
		CompressionIntrRecd = 1;
		if (!AUTO_MODE) {
			wake_up_interruptible(&compwq);
		}
	}

	if ((ISRDetectedModeOutOfLock == 1) || ((CaptureIntrRecd == 1) && (CompressionIntrRecd == 1))) {
		CaptureIntrRecd = 0;
		CompressionIntrRecd = 0;
        AutoModeIntrRecd = 1;
        if (AUTO_MODE) {
        	wake_up_interruptible (&autowq);
        }
    }
	
	return IRQ_HANDLED;
}

void ast_videocap_reset_hw(void)
{
	uint32_t reg;
#ifdef SOC_AST2600
	
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + 0x00);
	// When this register is locked, the read back value of this register is 0x00000000
	if(reg == 0)
	{
		iowrite32(0x1688A8A8, (void * __iomem)ast_scu_reg_virt_base + 0x00); /* unlock SCU */
	}
	/* enable reset video engine - SCU40[6] = 1 */
	reg = ioread32((void *__iomem)ast_scu_reg_virt_base + 0x40);
	reg |= 0x00000040;
	iowrite32(reg, (void *__iomem)ast_scu_reg_virt_base + 0x40);
	udelay(100);
	/* enable video engine clock - SCU080, use SCU084 to set */
	iowrite32(0x0000002E, (void * __iomem)ast_scu_reg_virt_base + 0x84);

	/* Clear SCU300 30:28 to fix kvm noise issue */
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + 0x300);
	reg &= ~(0x70000000);
	iowrite32(reg, (void * __iomem)ast_scu_reg_virt_base + 0x300);

	wmb();
	mdelay(10);
	/* disable reset video engine - SCU44[6] = 1 (A1 datasheet v0.6) */
	iowrite32(0x00000040, (void *__iomem)ast_scu_reg_virt_base + 0x44);
	/* support wide screen resolution */
	// SCU40 -> SCU100
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + 0x100);
	reg |= (0x00000001);
	iowrite32(reg, (void * __iomem)ast_scu_reg_virt_base + 0x100);
#else

	reg = ioread32((void * __iomem)SCU_KEY_CONTROL_REG);
	// When this register is locked, the read back value of this register is 0x00000000
	if(reg == 0)
	{
		iowrite32(0x1688A8A8, (void * __iomem)SCU_KEY_CONTROL_REG); /* unlock SCU */
	}

	/* enable reset video engine */
	reg = ioread32((void * __iomem)SCU_SYS_RESET_REG);
	reg |= 0x00000040;
	iowrite32(reg, (void * __iomem)SCU_SYS_RESET_REG);

	udelay(100);

	/* enable video engine clock */
	reg = ioread32((void * __iomem)SCU_CLK_STOP_REG);
	reg &= ~(0x0000002B);
	iowrite32(reg, (void * __iomem)SCU_CLK_STOP_REG);

	wmb();

	mdelay(10);

	/* disable reset video engine */
	reg = ioread32((void * __iomem)SCU_SYS_RESET_REG);
	reg &= ~(0x00000040);
	iowrite32(reg, (void * __iomem)SCU_SYS_RESET_REG);

	#if defined(SOC_AST2300) || defined(SOC_AST2400) || defined(SOC_AST2500) || defined(SOC_AST2530)
	/* support wide screen resolution */
	reg = ioread32((void * __iomem)SCU_SOC_SCRATCH1_REG);
	reg |= (0x00000001);
	iowrite32(reg, (void * __iomem)SCU_SOC_SCRATCH1_REG);
	#endif

#endif
}


void ast_videocap_hw_init(void)
{
	uint32_t reg;
#ifndef SOC_AST2600
	reg = ioread32((void * __iomem)SCU_KEY_CONTROL_REG);
	// When this register is locked, the read back value of this register is 0x00000000
	if(reg == 0)
	{
		iowrite32(0x1688A8A8, (void * __iomem)SCU_KEY_CONTROL_REG); /* unlock SCU */
	}

#if defined(SOC_AST2500) || defined(SOC_AST2530)
#define SCU_CLK_VIDEO_SLOW_MASK     (0x7 << 28)
#define SCU_ECLK_SOURCE_MASK        (0x3 << 2)
	reg = ioread32((void * __iomem)SCU_CLK_SELECT_REG);
	// Enable Clock & ECLK = inverse of (M-PLL / 2)
	reg &= ~(SCU_ECLK_SOURCE_MASK | SCU_CLK_VIDEO_SLOW_MASK);
	iowrite32(reg, (void * __iomem)SCU_CLK_SELECT_REG);
#endif

	/* enable reset video engine */
	reg = ioread32((void * __iomem)SCU_SYS_RESET_REG);
	reg |= 0x00000040;
	iowrite32(reg, (void * __iomem)SCU_SYS_RESET_REG);

	udelay(100);

	/* enable video engine clock */
	reg = ioread32((void * __iomem)SCU_CLK_STOP_REG);
	reg &= ~(0x0000002B);
	iowrite32(reg, (void * __iomem)SCU_CLK_STOP_REG);

	wmb();

	mdelay(10);

	/* disable reset video engine */
	reg = ioread32((void * __iomem)SCU_SYS_RESET_REG);
	reg &= ~(0x00000040);
	iowrite32(reg, (void * __iomem)SCU_SYS_RESET_REG);

#else
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + 0x00);
	// When this register is locked, the read back value of this register is 0x00000000
	if(reg == 0)
	{
		iowrite32(0x1688A8A8, (void * __iomem)ast_scu_reg_virt_base + 0x00); /* unlock SCU */
	}
	/* Follow AST2500 method. Fixes KVM video noise issue */
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + 0x300);
	reg &= ~((0x7 << 28) | (0x3 << 2));
	iowrite32(reg, (void * __iomem)ast_scu_reg_virt_base + 0x300);

	reg = ioread32((void *__iomem)ast_scu_reg_virt_base + 0x40);
	reg |= 0x00000040; /* enable reset video engine - SCU40[6] = 1 */
	iowrite32(reg, (void *__iomem)ast_scu_reg_virt_base + 0x40);
	udelay(100);

	/* enable video engine clock - SCU080, use SCU084 to set */
	iowrite32(0x0000002E, (void * __iomem)ast_scu_reg_virt_base + 0x84);

	/* Clear SCU300 30:28 to fix kvm noise issue */
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + 0x300);
	reg &= ~(0x70000000);
	iowrite32(reg, (void * __iomem)ast_scu_reg_virt_base + 0x300);

	wmb();
	mdelay(10);
	/* disable reset video engine - SCU44[6] = 1 (A1 datasheet v0.6) */
	iowrite32(0x00000040, (void *__iomem)ast_scu_reg_virt_base + 0x44);
#endif
	/* unlock video engine */
	udelay(100);
	ast_videocap_write_reg(AST_VIDEOCAP_KEY_MAGIC, AST_VIDEOCAP_KEY);

	/* clear interrupt status */
	ast_videocap_write_reg(0xFFFFFFFF, AST_VIDEOCAP_ISR);
	ast_videocap_write_reg(ALL_IRQ_STATUS_BITS, AST_VIDEOCAP_ISR);

	/* enable interrupt */
	reg = IRQ_WATCHDOG_OUT_OF_LOCK | IRQ_CAPTURE_COMPLETE | IRQ_COMPRESSION_COMPLETE | IRQ_MODE_DETECTION_READY;
	ast_videocap_write_reg(reg, AST_VIDEOCAP_ICR);

	ast_videocap_write_reg(0, AST_VIDEOCAP_COMPRESS_STREAM_BUF_READ_OFFSET);

	/* reset RC4 */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_CTRL2, 0x00000100);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL2, 0x00000100);
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_CTRL2, 0x00000100);

	//ast_videocap_vga_dac_ctrl(1);

	/* clear flag buffer */
	memset(AST_VIDEOCAP_FLAG_BUF_ADDR, 0x1, AST_VIDEOCAP_FLAG_BUF_SZ);
	ast_videocap_data_in_old_video_buf = 0;
}

void StopVideoCapture(void)
{
}

void ProcessTimeOuts(unsigned char Type)
{

	if (Type == NO_TIMEOUT) {
		return;
	}

	if (Type == SPURIOUS_TIMEOUT) {
		SpuriousTimeouts ++;
		return;
	}

	TimeoutCount ++;
	ast_videocap_data_in_old_video_buf = 0;

	if (Type == MODE_DET_TIMEOUT) {
		ModeDetTimeouts ++;
		return;
	}

	if (Type == CAPTURE_TIMEOUT) {
		CaptureTimeouts ++;
		ReInitVideoEngine = 1;
		return;
	}

	if (Type == COMPRESSION_TIMEOUT) {
		CompressionTimeouts ++;
		ReInitVideoEngine = 1;
		return;
	}
	
	if (Type == AUTOMODE_TIMEOUT) {
		AutoModeTimeouts ++;
		ReInitVideoEngine = 1;
		return;
	}
}

/* return 0 on succeeds, -1 on times out */
int ast_videocap_trigger_mode_detection(void)
{
	init_waitqueue_head(&mdwq);
	ModeDetectionIntrRecd = 0;
	TimeToWait = VE_TIMEOUT * HZ;

	/* trigger mode detection by rising edge(0 -> 1) */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_MODE_DETECT);
	wmb();
	udelay(100);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_MODE_DETECT);

	if (wait_event_interruptible_timeout(mdwq, ModeDetectionIntrRecd, TimeToWait) != 0) {
		WaitingForModeDetection = 0;
	} else { /* timeout */
		if (ModeDetectionIntrRecd == 0) {
		    ProcessTimeOuts(MODE_DET_TIMEOUT);
		    return -1; /* Possibly out of lock or invalid input, we should send a blank screen */
		} else {
		    /* ModeDetectionIntrRecd is set, but timeout triggered - should never happen!! */
		    ProcessTimeOuts(SPURIOUS_TIMEOUT);
		    /* Not much to do here, just continue on */
		}
	}

	return 0;
}

/* Return 0 if Capture Engine becomes idle, -1 if engine remains busy */
int ast_videocap_trigger_capture(void)
{
	init_waitqueue_head(&capwq);
	TimeToWait = VE_TIMEOUT*HZ;
	CaptureIntrRecd = 0;

	if (!(ast_videocap_read_reg(AST_VIDEOCAP_SEQ_CTRL) & AST_VIDEOCAP_SEQ_CTRL_CAP_STATUS)) {
		return -1; /* capture engine remains busy */
	}

	/* trigger capture */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS_IDLE);
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS);

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_CAP);
	udelay(100);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_CAP);

	WaitingForCapture = 1;
	if (wait_event_interruptible_timeout(capwq, CaptureIntrRecd, TimeToWait) != 0) {
		WaitingForCapture = 0;
	} else {
		/* Did Video Engine timeout? */
		if (CaptureIntrRecd == 0) {
			ProcessTimeOuts(CAPTURE_TIMEOUT);
			return -1; /* Capture Engine remains busy */
		} else {
			/* CaptureIntrRecd is set, but timeout triggered - should never happen!! */
			ProcessTimeOuts(SPURIOUS_TIMEOUT);
			/* Not much to do here, just continue on */
		}
	}

	return 0;
}

/* Return 0 if Compression becomes idle, -1 if engine remains busy */
int ast_videocap_trigger_compression(void)
{
	init_waitqueue_head(&compwq);
	TimeToWait = VE_TIMEOUT*HZ;
	CompressionIntrRecd = 0;

	if (!(ast_videocap_read_reg(AST_VIDEOCAP_SEQ_CTRL) & AST_VIDEOCAP_SEQ_CTRL_COMPRESS_STATUS)) {
		return -1; /* compression engine remains busy */
	}

	/* trigger compression */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS_IDLE);
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_CAP);

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS);
	udelay(100);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS);

	WaitingForCompression = 1;
	if (wait_event_interruptible_timeout(compwq, CompressionIntrRecd, TimeToWait) != 0) {
		WaitingForCompression = 0;
	} else {
		/* Did Video Engine timeout? */
		if (CompressionIntrRecd == 0) {
			ProcessTimeOuts(COMPRESSION_TIMEOUT);
			return -1; /* Compression remains busy */
		} else {
			/* CompressionIntrRecd is set, but timeout triggered - should never happen!! */
			ProcessTimeOuts(SPURIOUS_TIMEOUT);
			/* Not much to do here, just continue on */
		}
	}

	return 0;
}

int ast_videocap_automode_trigger(void)
{
	unsigned int status;

	TimeToWait = VE_TIMEOUT*HZ;
	AutoModeIntrRecd = 0;

	if (!(ast_videocap_read_reg(AST_VIDEOCAP_SEQ_CTRL) & AST_VIDEOCAP_SEQ_CTRL_CAP_STATUS)) {
		return -1; /* capture engine remains busy */
	}
	if (!(ast_videocap_read_reg(AST_VIDEOCAP_SEQ_CTRL) & AST_VIDEOCAP_SEQ_CTRL_COMPRESS_STATUS)) {
		return -1; /* compression engine remains busy */
	}

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS_IDLE);
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_CAP);
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS); 
	barrier();

    status = ast_videocap_read_reg(AST_VIDEOCAP_SEQ_CTRL);
    ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, ((status & 0xFFFFFFED) | 0x12));

    WaitingForAutoMode = 1;
    if (wait_event_interruptible_timeout(autowq, AutoModeIntrRecd, TimeToWait) != 0) {
    	WaitingForAutoMode = 0;
	 } else {
	 	if (AutoModeIntrRecd == 0) {
	 		ProcessTimeOuts(AUTOMODE_TIMEOUT);
			return -1;
		} else {			
			ProcessTimeOuts(SPURIOUS_TIMEOUT);
			/* Not much to do here, just continue on */
		}
	 }
	 

	 return 0;
}

void ast_videocap_determine_syncs(unsigned long *hp, unsigned long *vp)
{
	int i = 0;
	int hPcount = 0;
	int vPcount = 0;

	*vp = 0;
	*hp = 0;

	for (i = 0; i < MAX_SYNC_CHECK_COUNT; i ++) {
		if (ast_videocap_read_reg(AST_VIDEOCAP_MODE_DETECT_STATUS) & 0x20000000) // Hsync polarity is positive
		    hPcount ++;
		if (ast_videocap_read_reg(AST_VIDEOCAP_MODE_DETECT_STATUS) & 0x10000000) // Vsync polarity is positive
		    vPcount ++;
	}

	/* Majority readings indicate positive? Then it must be positive.*/
	if (hPcount > (MAX_SYNC_CHECK_COUNT / 2)) {
		*hp = 1;
	}

	if (vPcount > (MAX_SYNC_CHECK_COUNT / 2)) {
		*vp = 1;
	}
}

int ast_videocap_auto_position_adjust(struct ast_videocap_engine_info_t *info)
{
	uint32_t reg;
	unsigned long __maybe_unused H_Start, H_End, V_Start, V_End, i, H_Temp = 0;
//	unsigned long  V_Temp = 0;
	unsigned long __maybe_unused Mode_PixelClock = 0, mode_bandwidth, refresh_rate_index, color_depth_index;
	unsigned long __maybe_unused color_depth, mode_clock;

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, 0x0000FF00);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, 0x65 << 8);

Redo:
	ast_videocap_set_reg_bits(AST_VIDEOCAP_ISR, 0x00000010);
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_MODE_DETECT);

	if (ast_videocap_trigger_mode_detection() < 0) {
		/*  Possibly invalid input, we should send a blank screen   */
		ISRDetectedModeOutOfLock = 1;
		return -1;
	}

	H_Start = ast_videocap_read_reg(AST_VIDEOCAP_EDGE_DETECT_H) & AST_VIDEOCAP_EDGE_DETECT_START_MASK;
	H_End = (ast_videocap_read_reg(AST_VIDEOCAP_EDGE_DETECT_H) & AST_VIDEOCAP_EDGE_DETECT_END_MASK) >> AST_VIDEOCAP_EDGE_DETECT_END_SHIFT;
	V_Start = ast_videocap_read_reg(AST_VIDEOCAP_EDGE_DETECT_V) & AST_VIDEOCAP_EDGE_DETECT_START_MASK;
	V_End = (ast_videocap_read_reg(AST_VIDEOCAP_EDGE_DETECT_V) & AST_VIDEOCAP_EDGE_DETECT_END_MASK) >> AST_VIDEOCAP_EDGE_DETECT_END_SHIFT;

	//If any unsupported frame size comes, we should skip processing it.
	if ((((H_End - H_Start) + 1) > AST_VIDEOCAP_MAX_FRAME_WIDTH) ||
		( ((V_End - V_Start) + 1) > AST_VIDEOCAP_MAX_FRAME_HEIGHT) ) {
			/*  Possibly invalid input, we should send a blank screen   */
			ISRDetectedModeOutOfLock = 1;
			return -1;				
	}

	//Check if cable quality is too bad. If it is bad then we use 0x65 as threshold
	//Because RGB data is arrived slower than H-sync, V-sync. We have to read more times to confirm RGB data is arrived
	if ((abs(H_Temp - H_Start) > 1) || ((H_Start <= 1) || (V_Start <= 1) || (H_Start == 0x3FF) || (V_Start == 0x3FF))) {
		H_Temp = ast_videocap_read_reg(AST_VIDEOCAP_EDGE_DETECT_H) & AST_VIDEOCAP_EDGE_DETECT_START_MASK;
		//V_Temp = ast_videocap_read_reg(AST_VIDEOCAP_EDGE_DETECT_V) & AST_VIDEOCAP_EDGE_DETECT_START_MASK;
		//printk("REDO\n");
		//printk("Data = %x\n", ReadMMIOLong (MODE_DETECTION_REGISTER));
		//printk("H_Start = %x, H_End = %x\n", H_Start, H_End);
		//printk("V_Start = %x, V_End = %x\n", V_Start, V_End);
		goto Redo;
	}

	//printk("H_Start = %lx, H_End = %lx\n", H_Start, H_End);
	//printk("V_Start = %lx, V_End = %lx\n", V_Start, V_End);

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_TIMING_GEN_H, AST_VIDEOCAP_TIMING_GEN_LAST_PIXEL_MASK);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_TIMING_GEN_H, H_End);

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_TIMING_GEN_H, AST_VIDEOCAP_TIMING_GEN_FIRST_PIXEL_MASK);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_TIMING_GEN_H, (H_Start - 1) << AST_VIDEOCAP_TIMING_GEN_FIRST_PIXEL_SHIFT);

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_TIMING_GEN_V, AST_VIDEOCAP_TIMING_GEN_LAST_PIXEL_MASK);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_TIMING_GEN_V, V_End + 1);

	ast_videocap_clear_reg_bits(AST_VIDEOCAP_TIMING_GEN_V, AST_VIDEOCAP_TIMING_GEN_FIRST_PIXEL_MASK);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_TIMING_GEN_V, (V_Start) << AST_VIDEOCAP_TIMING_GEN_FIRST_PIXEL_SHIFT);

	info->src_mode.x = (H_End - H_Start) + 1;
	info->src_mode.y = (V_End - V_Start) + 1;

	info->dest_mode.x = info->src_mode.x;
	info->dest_mode.y = info->src_mode.y;

	printk("Mode Detected: %d x %d\n", info->src_mode.x, info->src_mode.y);

	//Set full screen flag to avoid bcd calculation and re-capture
	if(CaptureMode == VIDEOCAP_JPEG_SUPPORT) 
		full_screen_capture =1;

	//  Judge if bandwidth is not enough then enable direct mode in internal VGA

	reg = ast_videocap_read_reg(AST_VIDEOCAP_VGA_SCRATCH_9093);
	if (((reg & 0xff00) >> 8) == 0xA8) { // Driver supports to get display information in scratch register
		color_depth = (reg & 0xff0000) >> 16; //VGA's Color Depth is 0 when real color depth is less than 8
		mode_clock = (reg & 0xff000000) >> 24;
#ifdef SOC_AST2600 //Ref: (sdk7.1) drivers/soc/aspeed/ast_video.c > ast_video_vga_mode_detect()
		if (color_depth < 15) { /* Disable direct mode for less than 15 bpp */
			printk("Color Depth is less than 15 bpp\n");
			info->INFData.DirectMode = 0;
		}
		else /* Enable direct mode for 15 bpp and higher */
		{
			printk("Color Depth is 15 bpp or higher\n");
			info->INFData.DirectMode = 1;
		}
#else
		if (color_depth == 0) {
			printk("Color Depth is not 15 bpp or higher\n");
			info->INFData.DirectMode = 0;
	    } else {
			mode_bandwidth = (mode_clock * (color_depth + 32)) / 8; //Video uses 32bits
			if (info->MemoryBandwidth < mode_bandwidth) {
				info->INFData.DirectMode = 1;
			} else {
				info->INFData.DirectMode = 0;
			}
		}
#endif
	} else { /* old driver, get display information from table */
		refresh_rate_index = (ast_videocap_read_reg(AST_VIDEOCAP_VGA_SCRATCH_8C8F) >> 8) & 0x0F;
		color_depth_index = (ast_videocap_read_reg(AST_VIDEOCAP_VGA_SCRATCH_8C8F) >> 4) & 0x0F;
#ifdef SOC_AST2600 //Ref: (sdk7.1) drivers/soc/aspeed/ast_video.c > ast_video_vga_mode_detect()
		/* To fix AST2600 A0 Host VGA output tearing / noisy issue, ASPEED modified MCR74[31], MCR74[0] as 1.
		** This change caused DRAM shortage side effect in USB controller (A1 and higher).
		**
		** USB bandwidth is increased (MCR44 Maximum Grant Length) to fix DRAM shortage issue.
		** but it caused abnormal video screen with AST2600 A1/A2/A3 as side effect.
		**
		** To mitigate this abnormal video issue aspeed suggested to use direct fetch mode
		** for host resolution >= 1024x768. */
		if ((color_depth_index == 0xe) || (color_depth_index == 0xf)) { /* Disable direct mode for 14, 15 bpp */
			info->INFData.DirectMode = 0;
		} else { /* enable direct mode for host resolution >= 1024x768 */
			if (color_depth_index > 2) {
				if ((info->src_mode.x * info->src_mode.y) < (1024 * 768))
					info->INFData.DirectMode = 0;
				else
					info->INFData.DirectMode = 1;
			} else {
				printk("Color Depth is less than 15bpp\n");
				info->INFData.DirectMode = 0;
			}
		}
#else
		if ((color_depth_index == 2) || (color_depth_index == 3) || (color_depth_index == 4)) { // 15 bpp /16 bpp / 32 bpp
			for (i = 0; i < InternalModeTableSize; i ++) { /* traverse table to find mode */
				if ((info->src_mode.x == Internal_Mode[i].HorizontalActive) &&
					(info->src_mode.y == Internal_Mode[i].VerticalActive) &&
					(refresh_rate_index == Internal_Mode[i].RefreshRateIndex)) {
					Mode_PixelClock = Internal_Mode[i].PixelClock;
					//printk("Mode_PixelClock = %ld\n", Mode_PixelClock);
					break;
				}
			}

			if (i == InternalModeTableSize) {
				printk("videocap: No match mode\n");
			}

			//  Calculate bandwidth required for this mode
			//  Video requires pixelclock * 4, VGA requires pixelclock * bpp / 8
			mode_bandwidth = Mode_PixelClock * (4 + 2 * (color_depth_index - 2));
			//printk("mode_bandwidth = %ld\n", mode_bandwidth);
			if (info->MemoryBandwidth < mode_bandwidth) {
				info->INFData.DirectMode = 1;
			} else {
				info->INFData.DirectMode = 0;
			}
		} else {
			printk("Color Depth is not 15bpp or higher\n");
			info->INFData.DirectMode = 0;
		}
#endif
	}

	return 0;
}

int ast_videocap_mode_detection(struct ast_videocap_engine_info_t *info, bool in_video_capture)
{
	unsigned long HPolarity, VPolarity;
	uint32_t vga_status;

	ast_videocap_hw_init();

	/* Check if internal VGA screen is off */
	vga_status = ast_videocap_read_reg(AST_VIDEOCAP_VGA_STATUS) >> 24;
	if ((vga_status == 0x10) || (vga_status == 0x11) || (vga_status & 0xC0) || (vga_status & 0x04) || (!(vga_status & 0x3B))) {
		info->NoSignal = 1;
	} else {
		info->NoSignal = 0;
	}

	/* video source from internal VGA */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_SRC);

	/* Set VideoClockPhase Selection for Internal Video */
	//ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, 0x00000A00);

	/* polarity is same as source */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_HSYNC_POLARITY | AST_VIDEOCAP_CTRL_VSYNC_POLARITY);

	/* Set Stable Parameters */
	/*  VSync Stable Max*/
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, 0x000F0000);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, MODEDETECTION_VERTICAL_STABLE_MININUM << 16);

	/*  HSync Stable Max*/
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, 0x00F00000);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, MODEDETECTION_HORIZONTAL_STABLE_MININUM << 20);

	/*  VSync counter threshold */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, 0x0F000000);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, MODEDETECTION_VERTICAL_STABLE_THRESHOLD << 24);

	/*  HSync counter threshold */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, 0xF0000000);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_MODE_DETECT_PARAM, MODEDETECTION_HORIZONTAL_STABLE_THRESHOLD << 28);

	/* Disable Watchdog */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_WDT | AST_VIDEOCAP_SEQ_CTRL_MODE_DETECT);

	/* New Mode Detection procedure prescribed by ASPEED:
	 *
	 * Trigger mode detection once
	 * After that succeeds, figure out the polarities of HSync and VSync
	 * Program the right polarities...
	 * Then Trigger mode detection again
	 * Now with the new values, use JudgeMode to figure out the correct mode
	 */

	/* Trigger first detection, and check for timeout? */
	if (ast_videocap_trigger_mode_detection() < 0) {
		/*  Possibly invalid input, we should send a blank screen   */
		ISRDetectedModeOutOfLock = 1;
		return -1;
	}

	/* Now, determine sync polarities and program appropriately */
	ast_videocap_determine_syncs(&HPolarity, &VPolarity);

	if (info->INFData.DirectMode == 1) {
		/* Direct mode always uses positive/positive so we inverse them */
		ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_HSYNC_POLARITY | AST_VIDEOCAP_CTRL_VSYNC_POLARITY);
	} else {
		ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, (VPolarity << 1) | HPolarity);
	}

	/* Disable Watchdog */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_WDT | AST_VIDEOCAP_SEQ_CTRL_MODE_DETECT);

	/* Trigger second detection, and check for timeout? */
	if (ast_videocap_trigger_mode_detection() < 0) {
		/*  Possibly invalid input, we should send a blank screen */
		ISRDetectedModeOutOfLock = 1;
		return -1;
	}

	vga_status = ast_videocap_read_reg(AST_VIDEOCAP_VGA_STATUS) >> 24;
	if ((vga_status == 0x10) || (vga_status == 0x11) || (vga_status & 0xC0) || (vga_status & 0x04) || (!(vga_status & 0x3B))) {
		info->NoSignal = 1;
		udelay(100);
	} else {
		info->NoSignal = 0;
	}

	/* Enable Watchdog detection for next time */
	ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_WDT);

	ISRDetectedModeOutOfLock = 0;

    if (in_video_capture == 1)
    {
		ast_videocap_auto_position_adjust(info);
    }
    else if (info->NoSignal == 0) {
		ast_videocap_auto_position_adjust(info);
	}


	return 0;
}

// Set defaults for user settings
void InitDefaultSettings(struct ast_videocap_engine_info_t *info)
{
	info->FrameHeader.JPEGYUVTableMapping = JPEG_YUVTABLE_MAPPING;
	info->FrameHeader.JPEGScaleFactor = JPEG_SCALE_FACTOR;
	info->FrameHeader.AdvanceScaleFactor = ADVANCE_SCALE_FACTOR;
	info->FrameHeader.SharpModeSelection = SHARP_MODE_SELECTION;
	info->INFData.DownScalingMethod = DOWN_SCALING_METHOD;
	info->INFData.DifferentialSetting = DIFF_SETTING;
	info->FrameHeader.RC4Enable = RC4_ENABLE;
	info->FrameHeader.RC4Reset = RC4_RESET;
	info->INFData.AnalogDifferentialThreshold = ANALOG_DIFF_THRESHOLD;
	info->INFData.DigitalDifferentialThreshold = DIGITAL_DIFF_THRESHOLD;
	info->INFData.ExternalSignalEnable = EXTERNAL_SIGNAL_ENABLE;
	info->INFData.AutoMode = AUTO_MODE;

	memcpy(EncodeKeys, ENCODE_KEYS, ENCODE_KEYS_LEN);
}

int ast_videocap_get_engine_config(struct ast_videocap_engine_info_t *info, struct ast_videocap_engine_config_t *config, unsigned long *size)
{
	config->differential_setting = info->INFData.DifferentialSetting;
	config->dct_quant_quality = info->FrameHeader.JPEGScaleFactor;
	config->dct_quant_tbl_select = info->FrameHeader.JPEGTableSelector;
	config->sharp_mode_selection = info->FrameHeader.SharpModeSelection;
	config->sharp_quant_quality = info->FrameHeader.AdvanceScaleFactor;
	config->sharp_quant_tbl_select = info->FrameHeader.AdvanceTableSelector;
	config->compression_mode = info->FrameHeader.CompressionMode;
	config->vga_dac = ast_Videocap_vga_dac_status();
	*size = sizeof(struct ast_videocap_engine_config_t);
	return 0;
}

int ast_videocap_set_engine_config(struct ast_videocap_engine_info_t *info, struct ast_videocap_engine_config_t *config)
{
	ast_videocap_data_in_old_video_buf = 0; /* configurations are changed, send a full screen next time */

	info->INFData.DifferentialSetting = config->differential_setting;
	info->FrameHeader.JPEGScaleFactor = config->dct_quant_quality;
	info->FrameHeader.JPEGTableSelector = config->dct_quant_tbl_select;
	info->FrameHeader.SharpModeSelection = config->sharp_mode_selection;
	info->FrameHeader.AdvanceScaleFactor = config->sharp_quant_quality;
	info->FrameHeader.AdvanceTableSelector = config->sharp_quant_tbl_select;
	info->FrameHeader.CompressionMode = config->compression_mode;

	if (config->vga_dac == 0) { /* turn off VGA output */
		ast_videocap_vga_dac_ctrl(0);
	} else { /* turn on VGA output */
		ast_videocap_vga_dac_ctrl(1);
	}

	return 0;
}

int ast_videocap_enable_video_dac(struct ast_videocap_engine_info_t *info, struct ast_videocap_engine_config_t *config)
{
	ast_videocap_data_in_old_video_buf = 0; /* configurations are changed, send a full screen next time */
//	info->INFData.DifferentialSetting = config->differential_setting;
//	info->FrameHeader.JPEGScaleFactor = config->dct_quant_quality;
//	info->FrameHeader.JPEGTableSelector = config->dct_quant_tbl_select;
//	info->FrameHeader.SharpModeSelection = config->sharp_mode_selection;
//	info->FrameHeader.AdvanceScaleFactor = config->sharp_quant_quality;
//	info->FrameHeader.AdvanceTableSelector = config->sharp_quant_tbl_select;
//	info->FrameHeader.CompressionMode = config->compression_mode;
    config->vga_dac=1;
	ast_videocap_vga_dac_ctrl(1);
	return 0;
}

#ifdef SOC_AST2600
#ifdef SCU_M_PLL_PARAM_REG
#undef SCU_M_PLL_PARAM_REG
#endif
#define SCU_M_PLL_PARAM_REG 0x220
#endif

void ast_videocap_get_mem_bandwidth(struct ast_videocap_engine_info_t *info)
{
	uint32_t reg;
	unsigned long Numerator, Denumerator, PostDivider, OutputDivider, Buswidth = 16, MemoryClock;
#ifdef SOC_AST2600
	reg = ioread32((void * __iomem)ast_scu_reg_virt_base + SCU_M_PLL_PARAM_REG);
	Numerator = (reg & 0x1FFF); // [12:0]
	Denumerator = ((reg >> 13) & 0x3F); // [18:13]
	OutputDivider = 0; // unused
	PostDivider = ((reg >> 19) & 0xF); // [22:19]
	MemoryClock = 25 * (( Numerator + 1 ) / ( Denumerator + 1 )) / ( PostDivider + 1 );
	info->MemoryBandwidth = ((MemoryClock * 2 * Buswidth * 6) / 10) / 8;
#else
	reg = ioread32((void * __iomem)SCU_M_PLL_PARAM_REG);

	/* We discard M-PLL bypass and Turn-Off M-PLL mode */
	Numerator = ((reg >> 5) & 0x3F);
	Denumerator = (reg & 0xF);
	OutputDivider = ((reg >> 4) & 0x1); /* Fortify issue :: False Positive */
	switch ((reg >> 12) & 0x7) {
	case 0x04: /* 100 */
		PostDivider = 2;
		break;
	case 0x05: /* 101 */
		PostDivider = 4;
		break;
	case 0x06: /* 110 */
		PostDivider = 8;
		break;
	case 0x07: /* 111 */
		PostDivider = 16;
		break;
	default:
		PostDivider = 1;
		break;
	}


	#if defined(SOC_AST2300) || defined(SOC_AST2400)
	MemoryClock = (24 * (2 - OutputDivider) * (Numerator + 2) / (Denumerator + 1)) / PostDivider;
	info->MemoryBandwidth = ((MemoryClock * 2 * Buswidth * 4) / 10) / 8;
	#else
	MemoryClock = 24 * (( Numerator + 1 ) / ( Denumerator + 1 )) / ( PostDivider + 1 );
	info->MemoryBandwidth = ((MemoryClock * 2 * Buswidth * 6) / 10) / 8;
	#endif
	//printk("Memory Clock = %ld\n", MemoryClock);
	//printk("Memory Bandwidth = %d\n", info->MemoryBandwidth);
#endif
}

// Refer https://github.com/AspeedTech-BMC/linux/blob/aspeed-master-v5.15/drivers/media/platform/aspeed-video.c
static const u32 aspeed_video_jpeg_header[ASPEED_VIDEO_JPEG_HEADER_SIZE] = {
	0xe0ffd8ff, 0x464a1000, 0x01004649, 0x60000101, 0x00006000, 0x0f00feff,
	0x00002d05, 0x00000000, 0x00000000, 0x00dbff00
};

static const u32 aspeed_video_jpeg_quant[ASPEED_VIDEO_JPEG_QUANT_SIZE] = {
	0x081100c0, 0x00000000, 0x00110103, 0x03011102, 0xc4ff0111, 0x00001f00,
	0x01010501, 0x01010101, 0x00000000, 0x00000000, 0x04030201, 0x08070605,
	0xff0b0a09, 0x10b500c4, 0x03010200, 0x03040203, 0x04040505, 0x7d010000,
	0x00030201, 0x12051104, 0x06413121, 0x07615113, 0x32147122, 0x08a19181,
	0xc1b14223, 0xf0d15215, 0x72623324, 0x160a0982, 0x1a191817, 0x28272625,
	0x35342a29, 0x39383736, 0x4544433a, 0x49484746, 0x5554534a, 0x59585756,
	0x6564635a, 0x69686766, 0x7574736a, 0x79787776, 0x8584837a, 0x89888786,
	0x9493928a, 0x98979695, 0xa3a29a99, 0xa7a6a5a4, 0xb2aaa9a8, 0xb6b5b4b3,
	0xbab9b8b7, 0xc5c4c3c2, 0xc9c8c7c6, 0xd4d3d2ca, 0xd8d7d6d5, 0xe2e1dad9,
	0xe6e5e4e3, 0xeae9e8e7, 0xf4f3f2f1, 0xf8f7f6f5, 0xc4fffaf9, 0x00011f00,
	0x01010103, 0x01010101, 0x00000101, 0x00000000, 0x04030201, 0x08070605,
	0xff0b0a09, 0x11b500c4, 0x02010200, 0x04030404, 0x04040507, 0x77020100,
	0x03020100, 0x21050411, 0x41120631, 0x71610751, 0x81322213, 0x91421408,
	0x09c1b1a1, 0xf0523323, 0xd1726215, 0x3424160a, 0x17f125e1, 0x261a1918,
	0x2a292827, 0x38373635, 0x44433a39, 0x48474645, 0x54534a49, 0x58575655,
	0x64635a59, 0x68676665, 0x74736a69, 0x78777675, 0x83827a79, 0x87868584,
	0x928a8988, 0x96959493, 0x9a999897, 0xa5a4a3a2, 0xa9a8a7a6, 0xb4b3b2aa,
	0xb8b7b6b5, 0xc3c2bab9, 0xc7c6c5c4, 0xd2cac9c8, 0xd6d5d4d3, 0xdad9d8d7,
	0xe5e4e3e2, 0xe9e8e7e6, 0xf4f3f2ea, 0xf8f7f6f5, 0xdafffaf9, 0x01030c00,
	0x03110200, 0x003f0011
};

static const u32 aspeed_video_jpeg_dct[ASPEED_VIDEO_JPEG_NUM_QUALITIES]
				      [ASPEED_VIDEO_JPEG_DCT_SIZE] = {
	{ 0x0d140043, 0x0c0f110f, 0x11101114, 0x17141516, 0x1e20321e,
	  0x3d1e1b1b, 0x32242e2b, 0x4b4c3f48, 0x44463f47, 0x61735a50,
	  0x566c5550, 0x88644644, 0x7a766c65, 0x4d808280, 0x8c978d60,
	  0x7e73967d, 0xdbff7b80, 0x1f014300, 0x272d2121, 0x3030582d,
	  0x697bb958, 0xb8b9b97b, 0xb9b8a6a6, 0xb9b9b9b9, 0xb9b9b9b9,
	  0xb9b9b9b9, 0xb9b9b9b9, 0xb9b9b9b9, 0xb9b9b9b9, 0xb9b9b9b9,
	  0xb9b9b9b9, 0xb9b9b9b9, 0xb9b9b9b9, 0xffb9b9b9 },
	{ 0x0c110043, 0x0a0d0f0d, 0x0f0e0f11, 0x14111213, 0x1a1c2b1a,
	  0x351a1818, 0x2b1f2826, 0x4142373f, 0x3c3d373e, 0x55644e46,
	  0x4b5f4a46, 0x77573d3c, 0x6b675f58, 0x43707170, 0x7a847b54,
	  0x6e64836d, 0xdbff6c70, 0x1b014300, 0x22271d1d, 0x2a2a4c27,
	  0x5b6ba04c, 0xa0a0a06b, 0xa0a0a0a0, 0xa0a0a0a0, 0xa0a0a0a0,
	  0xa0a0a0a0, 0xa0a0a0a0, 0xa0a0a0a0, 0xa0a0a0a0, 0xa0a0a0a0,
	  0xa0a0a0a0, 0xa0a0a0a0, 0xa0a0a0a0, 0xffa0a0a0 },
	{ 0x090e0043, 0x090a0c0a, 0x0c0b0c0e, 0x110e0f10, 0x15172415,
	  0x2c151313, 0x241a211f, 0x36372e34, 0x31322e33, 0x4653413a,
	  0x3e4e3d3a, 0x62483231, 0x58564e49, 0x385d5e5d, 0x656d6645,
	  0x5b536c5a, 0xdbff595d, 0x16014300, 0x1c201818, 0x22223f20,
	  0x4b58853f, 0x85858558, 0x85858585, 0x85858585, 0x85858585,
	  0x85858585, 0x85858585, 0x85858585, 0x85858585, 0x85858585,
	  0x85858585, 0x85858585, 0x85858585, 0xff858585 },
	{ 0x070b0043, 0x07080a08, 0x0a090a0b, 0x0d0b0c0c, 0x11121c11,
	  0x23110f0f, 0x1c141a19, 0x2b2b2429, 0x27282428, 0x3842332e,
	  0x313e302e, 0x4e392827, 0x46443e3a, 0x2c4a4a4a, 0x50565137,
	  0x48425647, 0xdbff474a, 0x12014300, 0x161a1313, 0x1c1c331a,
	  0x3d486c33, 0x6c6c6c48, 0x6c6c6c6c, 0x6c6c6c6c, 0x6c6c6c6c,
	  0x6c6c6c6c, 0x6c6c6c6c, 0x6c6c6c6c, 0x6c6c6c6c, 0x6c6c6c6c,
	  0x6c6c6c6c, 0x6c6c6c6c, 0x6c6c6c6c, 0xff6c6c6c },
	{ 0x06090043, 0x05060706, 0x07070709, 0x0a09090a, 0x0d0e160d,
	  0x1b0d0c0c, 0x16101413, 0x21221c20, 0x1e1f1c20, 0x2b332824,
	  0x26302624, 0x3d2d1f1e, 0x3735302d, 0x22393a39, 0x3f443f2b,
	  0x38334338, 0xdbff3739, 0x0d014300, 0x11130e0e, 0x15152613,
	  0x2d355026, 0x50505035, 0x50505050, 0x50505050, 0x50505050,
	  0x50505050, 0x50505050, 0x50505050, 0x50505050, 0x50505050,
	  0x50505050, 0x50505050, 0x50505050, 0xff505050 },
	{ 0x04060043, 0x03040504, 0x05040506, 0x07060606, 0x09090f09,
	  0x12090808, 0x0f0a0d0d, 0x16161315, 0x14151315, 0x1d221b18,
	  0x19201918, 0x281e1514, 0x2423201e, 0x17262726, 0x2a2d2a1c,
	  0x25222d25, 0xdbff2526, 0x09014300, 0x0b0d0a0a, 0x0e0e1a0d,
	  0x1f25371a, 0x37373725, 0x37373737, 0x37373737, 0x37373737,
	  0x37373737, 0x37373737, 0x37373737, 0x37373737, 0x37373737,
	  0x37373737, 0x37373737, 0x37373737, 0xff373737 },
	{ 0x02030043, 0x01020202, 0x02020203, 0x03030303, 0x04040704,
	  0x09040404, 0x07050606, 0x0b0b090a, 0x0a0a090a, 0x0e110d0c,
	  0x0c100c0c, 0x140f0a0a, 0x1211100f, 0x0b131313, 0x1516150e,
	  0x12111612, 0xdbff1213, 0x04014300, 0x05060505, 0x07070d06,
	  0x0f121b0d, 0x1b1b1b12, 0x1b1b1b1b, 0x1b1b1b1b, 0x1b1b1b1b,
	  0x1b1b1b1b, 0x1b1b1b1b, 0x1b1b1b1b, 0x1b1b1b1b, 0x1b1b1b1b,
	  0x1b1b1b1b, 0x1b1b1b1b, 0x1b1b1b1b, 0xff1b1b1b },
	{ 0x01020043, 0x01010101, 0x01010102, 0x02020202, 0x03030503,
	  0x06030202, 0x05030404, 0x07070607, 0x06070607, 0x090b0908,
	  0x080a0808, 0x0d0a0706, 0x0c0b0a0a, 0x070c0d0c, 0x0e0f0e09,
	  0x0c0b0f0c, 0xdbff0c0c, 0x03014300, 0x03040303, 0x04040804,
	  0x0a0c1208, 0x1212120c, 0x12121212, 0x12121212, 0x12121212,
	  0x12121212, 0x12121212, 0x12121212, 0x12121212, 0x12121212,
	  0x12121212, 0x12121212, 0x12121212, 0xff121212 },
	{ 0x01020043, 0x01010101, 0x01010102, 0x02020202, 0x03030503,
	  0x06030202, 0x05030404, 0x07070607, 0x06070607, 0x090b0908,
	  0x080a0808, 0x0d0a0706, 0x0c0b0a0a, 0x070c0d0c, 0x0e0f0e09,
	  0x0c0b0f0c, 0xdbff0c0c, 0x02014300, 0x03030202, 0x04040703,
	  0x080a0f07, 0x0f0f0f0a, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f,
	  0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f,
	  0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0xff0f0f0f },
	{ 0x01010043, 0x01010101, 0x01010101, 0x01010101, 0x02020302,
	  0x04020202, 0x03020303, 0x05050405, 0x05050405, 0x07080606,
	  0x06080606, 0x0a070505, 0x09080807, 0x05090909, 0x0a0b0a07,
	  0x09080b09, 0xdbff0909, 0x02014300, 0x02030202, 0x03030503,
	  0x07080c05, 0x0c0c0c08, 0x0c0c0c0c, 0x0c0c0c0c, 0x0c0c0c0c,
	  0x0c0c0c0c, 0x0c0c0c0c, 0x0c0c0c0c, 0x0c0c0c0c, 0x0c0c0c0c,
	  0x0c0c0c0c, 0x0c0c0c0c, 0x0c0c0c0c, 0xff0c0c0c },
	{ 0x01010043, 0x01010101, 0x01010101, 0x01010101, 0x01010201,
	  0x03010101, 0x02010202, 0x03030303, 0x03030303, 0x04050404,
	  0x04050404, 0x06050303, 0x06050505, 0x03060606, 0x07070704,
	  0x06050706, 0xdbff0606, 0x01014300, 0x01020101, 0x02020402,
	  0x05060904, 0x09090906, 0x09090909, 0x09090909, 0x09090909,
	  0x09090909, 0x09090909, 0x09090909, 0x09090909, 0x09090909,
	  0x09090909, 0x09090909, 0x09090909, 0xff090909 },
	{ 0x01010043, 0x01010101, 0x01010101, 0x01010101, 0x01010101,
	  0x01010101, 0x01010101, 0x01010101, 0x01010101, 0x02020202,
	  0x02020202, 0x03020101, 0x03020202, 0x01030303, 0x03030302,
	  0x03020303, 0xdbff0403, 0x01014300, 0x01010101, 0x01010201,
	  0x03040602, 0x06060604, 0x06060606, 0x06060606, 0x06060606,
	  0x06060606, 0x06060606, 0x06060606, 0x06060606, 0x06060606,
	  0x06060606, 0x06060606, 0x06060606, 0xff060606 }
};


void ast_init_jpeg_table()
{
	int i;
	unsigned int base;

	for (i = 0; i < ASPEED_VIDEO_JPEG_NUM_QUALITIES; i++) {
		base = 256 * i;	/* AST HW requires this header spacing */
		memcpy(&JPEGHeaderTable[base], aspeed_video_jpeg_header,
		       sizeof(aspeed_video_jpeg_header));

		base += ASPEED_VIDEO_JPEG_HEADER_SIZE;
		memcpy(&JPEGHeaderTable[base], aspeed_video_jpeg_dct[i],
		       sizeof(aspeed_video_jpeg_dct[i]));

		base += ASPEED_VIDEO_JPEG_DCT_SIZE;
		memcpy(&JPEGHeaderTable[base], aspeed_video_jpeg_quant,
		       sizeof(aspeed_video_jpeg_quant));
	}
	memcpy(AST_VIDEOCAP_HDR_BUF_ADDR, JPEGHeaderTable , sizeof(JPEGHeaderTable));
	//printk("sizeof JPEGHeaderTable = %d\n",sizeof(JPEGHeaderTable));
	ast_videocap_write_reg((uint32_t)AST_VIDEOCAP_HDR_BUF_ADDR, AST_VIDEOCAP_CRC_BUF_ADDR);
}

int StartVideoCapture(struct ast_videocap_engine_info_t *info)
{
	unsigned long DRAM_Data;

	InitDefaultSettings(info);
	prev_cursor_status = 0;
	prev_cursor_pattern_addr = NULL;
	
	// Set full screen flag for jpeg mode alone
	if(CaptureMode == VIDEOCAP_JPEG_SUPPORT) 
		full_screen_capture = 1;

	// For first screen, whole frame should be reported as changed
	ast_videocap_write_reg(virt_to_phys(AST_VIDEOCAP_REF_BUF_ADDR), AST_VIDEOCAP_SRC_BUF2_ADDR);
	ast_videocap_write_reg(virt_to_phys(AST_VIDEOCAP_IN_BUF_ADDR), AST_VIDEOCAP_SRC_BUF1_ADDR);
	ast_videocap_write_reg(virt_to_phys(AST_VIDEOCAP_FLAG_BUF_ADDR), AST_VIDEOCAP_BCD_FLAG_BUF_ADDR);
	if (CaptureMode == VIDEOCAP_JPEG_SUPPORT)
		ast_videocap_write_reg(virt_to_phys(AST_VIDEOCAP_HDR_BUF_ADDR), AST_VIDEOCAP_CRC_BUF_ADDR);
	ast_videocap_write_reg(virt_to_phys(AST_VIDEOCAP_COMPRESS_BUF_ADDR), AST_VIDEOCAP_COMPRESSED_BUF_ADDR);

	/* reset compression control & disable RC4 encryption */
	ast_videocap_write_reg(0x00080000, AST_VIDEOCAP_COMPRESS_CTRL);

#ifdef SOC_AST2600
	// DRAM_Data = *(volatile unsigned long *) ast_sdram_reg_virt_base;
	DRAM_Data = ioread32((void * __iomem)ast_sdram_reg_virt_base + 0x04);
#else
	DRAM_Data = *(volatile unsigned long *) SDRAM_CONFIG_REG;
#endif
	
#if defined(SOC_AST2300) || defined(SOC_AST2400)
	switch (DRAM_Data & 0x03) {
	case 0:
		info->TotalMemory = 64;
		break;
	case 1:
		info->TotalMemory = 128;
		break;
	case 2:
		info->TotalMemory = 256;
		break;
	case 3:
		info->TotalMemory = 512;
		break;
	}
	switch ((DRAM_Data >> 2) & 0x03) {
	case 0:
		info->VGAMemory = 8;
		break;
	case 1:
		info->VGAMemory = 16;
		break;
	case 2:
		info->VGAMemory = 32;
		break;
	case 3:
		info->VGAMemory = 64;
		break;
	}
#elif defined(SOC_AST2500) || defined(SOC_AST2530)
	switch (DRAM_Data & 0x03) {
	case 0:
		info->TotalMemory = 128;
		break;
	case 1:
		info->TotalMemory = 256;
		break;
	case 2:
		info->TotalMemory = 512;
		break;
	case 3:
		info->TotalMemory = 1024;
		break;
	}

	switch ((DRAM_Data >> 2) & 0x03) {
	case 0:
		info->VGAMemory = 8;
		break;
	case 1:
		info->VGAMemory = 16;
		break;
	case 2:
		info->VGAMemory = 32;
		break;
	case 3:
		info->VGAMemory = 64;
		break;
	}
#elif defined(SOC_AST2600)
	switch (DRAM_Data & 0x03) {
	case 0:
		info->TotalMemory = 256;
		break;
	case 1:
		info->TotalMemory = 512;
		break;
	case 2:
		info->TotalMemory = 1024;
		break;
	case 3:
		info->TotalMemory = 2048;
		break;
	}

	switch ((DRAM_Data >> 2) & 0x03) {
	case 0:
		info->VGAMemory = 8;
		break;
	case 1:
		info->VGAMemory = 16;
		break;
	case 2:
		info->VGAMemory = 32;
		break;
	case 3:
		info->VGAMemory = 64;
		break;
	}
#else
	switch ((DRAM_Data >> 2) & 0x03) {
	case 0:
		info->TotalMemory = 32;
		break;
	case 1:
		info->TotalMemory = 64;
		break;
	case 2:
		info->TotalMemory = 128;
		break;
	case 3:
		info->TotalMemory = 256;
		break;
	}
	switch ((DRAM_Data >> 4) & 0x03) {
	case 0:
		info->VGAMemory = 8;
		break;
	case 1:
		info->VGAMemory = 16;
		break;
	case 2:
		info->VGAMemory = 32;
		break;
	case 3:
		info->VGAMemory = 64;
		break;
	}
#endif

	//printk("Total Memory = %ld MB\n", info->TotalMemory);
	//printk("VGA Memory = %ld MB\n", info->VGAMemory);
	ast_videocap_get_mem_bandwidth(info);

	ast_videocap_mode_detection(info,0);
	ast_videocap_data_in_old_video_buf = 0;

	return 0;
}

static void ast_videocap_horizontal_down_scaling(struct ast_videocap_engine_info_t *info)
{
	ast_videocap_write_reg(0, AST_VIDEOCAP_SCALING_FACTOR);
	ast_videocap_write_reg(0x10001000, AST_VIDEOCAP_SCALING_FACTOR);
}

static void ast_videocap_vertical_down_scaling(struct ast_videocap_engine_info_t *info)
{
	ast_videocap_write_reg(0, AST_VIDEOCAP_SCALING_FACTOR);
	ast_videocap_write_reg(0x10001000, AST_VIDEOCAP_SCALING_FACTOR);
}

int ast_videocap_engine_init(struct ast_videocap_engine_info_t *info)
{
	uint32_t diff_setting;
	uint32_t compress_setting;
	unsigned long OldBufferAddress, NewBufferAddress;
	unsigned long buf_offset;
	unsigned long remainder;

	#if defined(SOC_AST2300)
	ast_videocap_write_reg(0, AST_VIDEOCAP_PRIMARY_CRC_PARAME);
	ast_videocap_write_reg(0, AST_VIDEOCAP_SECONDARY_CRC_PARAM);
	#endif
	ast_videocap_write_reg(0, AST_VIDEOCAP_DATA_TRUNCATION); /* no reduction on RGB */

	/* Set to JPEG compress/decompress mode */
	if (CaptureMode == VIDEOCAP_JPEG_SUPPORT)
	{
		 
		ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_JPEG);
		ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS_JPEG_FRAME);

	}
	else
	{
		ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_JPEG);
		ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS_JPEG_FRAME);
	}

	/* Set to YUV444 mode */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_FORMAT_MASK);
	info->FrameHeader.Mode420 = 0;

	if (info->INFData.AutoMode) {
		ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_AUTO_COMPRESS);
	}
	else {
		/* disable automatic compression */
		ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_AUTO_COMPRESS);
	}
	/* Internal always uses internal timing generation */
	ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_DE_SIGNAL);

	/* Internal use inverse clock and software cursor */
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_CLK_DELAY_MASK);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_INV_CLK_NO_DELAY);

  /* clear bit 8 of VR008 when not in DirectMode. */
  if (info->INFData.DirectMode) {
		ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_DIRECT_FETCH_FRAME_BUF);

		ast_videocap_write_reg((info->TotalMemory - info->VGAMemory) << 20, AST_VIDEOCAP_DIRECT_FRAME_BUF_ADDR);
		ast_videocap_write_reg(info->src_mode.x * 4, AST_VIDEOCAP_DIRECT_FRAME_BUF_CTRL);

		/* always use auto mode since we can not get the correct setting when the VGA driver is not installed */
		ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_AUTO_MODE);
	} else {
		ast_videocap_clear_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_DIRECT_FETCH_FRAME_BUF);

		/* without VGA hardware cursor overlay image */
		ast_videocap_set_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_HW_CSR_OVERLAY); 
	}

	/* set scaling filter parameters */
	if ((info->src_mode.x == info->dest_mode.x) && (info->src_mode.y == info->dest_mode.y)) {
		/* scaling factor == 1.0, that is, non-scaling */
		ast_videocap_write_reg(AST_VIDEOCAP_SCALING_FILTER_PARAM_ONE, AST_VIDEOCAP_SCALING_FILTER_PARAM0);
		ast_videocap_write_reg(AST_VIDEOCAP_SCALING_FILTER_PARAM_ONE, AST_VIDEOCAP_SCALING_FILTER_PARAM1);
		ast_videocap_write_reg(AST_VIDEOCAP_SCALING_FILTER_PARAM_ONE, AST_VIDEOCAP_SCALING_FILTER_PARAM2);
		ast_videocap_write_reg(AST_VIDEOCAP_SCALING_FILTER_PARAM_ONE, AST_VIDEOCAP_SCALING_FILTER_PARAM3);
	}

  /* Fix ast2500 resolution 1680x1050 issue, follow SDK ast-video.c line:507 */
#if defined(SOC_AST2500) || defined(SOC_AST2600)
  if(info->src_mode.x==1680)
  {
   ast_videocap_write_reg((1728 << AST_VIDEOCAP_WINDOWS_H_SHIFT) + info->src_mode.y, AST_VIDEOCAP_CAPTURE_WINDOW);
   ast_videocap_write_reg((info->src_mode.x << AST_VIDEOCAP_WINDOWS_H_SHIFT) | info->src_mode.y, AST_VIDEOCAP_COMPRESS_WINDOW);
  }
  else
  {
   ast_videocap_write_reg((info->src_mode.x << AST_VIDEOCAP_WINDOWS_H_SHIFT) + info->src_mode.y, AST_VIDEOCAP_CAPTURE_WINDOW);
   ast_videocap_write_reg((info->src_mode.x << AST_VIDEOCAP_WINDOWS_H_SHIFT) + info->src_mode.y, AST_VIDEOCAP_COMPRESS_WINDOW);
  }
#else
   ast_videocap_write_reg((info->src_mode.x << AST_VIDEOCAP_WINDOWS_H_SHIFT) | info->src_mode.y, AST_VIDEOCAP_CAPTURE_WINDOW);
   ast_videocap_write_reg((info->src_mode.x << AST_VIDEOCAP_WINDOWS_H_SHIFT) | info->src_mode.y, AST_VIDEOCAP_COMPRESS_WINDOW);
#endif

	/* set buffer offset based on detected mode */
	buf_offset = info->src_mode.x;
	remainder = buf_offset % 8;
	if (remainder != 0) {
		buf_offset += (8 - remainder);
	}
	ast_videocap_write_reg(buf_offset * 4, AST_VIDEOCAP_SCAN_LINE_OFFSET);

	/* set buffers addresses */
	if(((CaptureMode == VIDEOCAP_JPEG_SUPPORT) && (full_screen_capture == 1))|| (ast_videocap_data_in_old_video_buf ==0))
	{
		ast_videocap_write_reg((unsigned long) AST_VIDEOCAP_IN_BUF_ADDR, AST_VIDEOCAP_SRC_BUF1_ADDR);
		ast_videocap_write_reg((unsigned long) AST_VIDEOCAP_REF_BUF_ADDR, AST_VIDEOCAP_SRC_BUF2_ADDR);
		diff_setting = AST_VIDEOCAP_BCD_CTRL_DISABLE;
	}
	else
	{
		OldBufferAddress = ast_videocap_read_reg(AST_VIDEOCAP_SRC_BUF1_ADDR);
		NewBufferAddress = ast_videocap_read_reg(AST_VIDEOCAP_SRC_BUF2_ADDR);
		ast_videocap_write_reg(NewBufferAddress, AST_VIDEOCAP_SRC_BUF1_ADDR);
		ast_videocap_write_reg(OldBufferAddress, AST_VIDEOCAP_SRC_BUF2_ADDR);
		diff_setting = AST_VIDEOCAP_BCD_CTRL_ENABLE;
	}

	diff_setting |= (info->INFData.DigitalDifferentialThreshold << AST_VIDEOCAP_BCD_CTRL_TOLERANCE_SHIFT);
	ast_videocap_write_reg(diff_setting, AST_VIDEOCAP_BCD_CTRL);

	// Set this stream register even in frame mode
	ast_videocap_write_reg(0x0000001F, AST_VIDEOCAP_STREAM_BUF_SIZE);

	//  Configure JPEG parameters
	switch (info->FrameHeader.CompressionMode) {
	case COMP_MODE_YUV420:
		info->FrameHeader.Mode420 = 1;
		ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_FORMAT_MASK);
		ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_FORMAT_YUV420 << AST_VIDEOCAP_SEQ_CTRL_FORMAT_SHIFT);
		compress_setting = 1;
		break;
	case COMP_MODE_YUV444_JPG_ONLY:
		compress_setting = 1;
		break;
	case COMP_MODE_YUV444_2_CLR_VQ:
		info->INFData.VQMode = 2;
		compress_setting = 0;
		break;
	case COMP_MODE_YUV444_4_CLR_VQ:
		info->INFData.VQMode = 4;
		compress_setting = 2;
		break;
	default:
		printk("videocap: Default CompressionMode triggered!! %ld\n",
		info->FrameHeader.CompressionMode);
		info->FrameHeader.Mode420 = 1;
		ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_FORMAT_MASK);
		ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_FORMAT_YUV420 << AST_VIDEOCAP_SEQ_CTRL_FORMAT_SHIFT);
		compress_setting = 1;
		info->FrameHeader.CompressionMode = COMP_MODE_YUV420;
		break;
	}

	/* always use the same selector */
	compress_setting |= (info->FrameHeader.JPEGTableSelector) << 11;
	compress_setting |= (info->FrameHeader.JPEGTableSelector) << 6;
	/* if mapping is set, luminance and chrominance use the same table, else use respective table with same slector */
	if (!info->FrameHeader.JPEGYUVTableMapping)
		compress_setting |= 1 << 10;
	#if defined(SOC_AST2300) || defined(SOC_AST2400) || defined(SOC_AST2500) || defined(SOC_AST2530) || defined(SOC_AST2600)
	if (!info->FrameHeader.Mode420) {
		compress_setting |= (info->FrameHeader.AdvanceTableSelector) << 27;
		compress_setting |= (info->FrameHeader.AdvanceTableSelector) << 22;
		if (!info->FrameHeader.JPEGYUVTableMapping)
			compress_setting |= 1 << 26;
		compress_setting |= 0x00010000;
	}
	#endif
	compress_setting |= 0x00080000;
	
	if(CaptureMode == VIDEOCAP_JPEG_SUPPORT) 
	{
		compress_setting |= VR060_VIDEO_COMPRESSION_SETTING;
	}
	ast_videocap_write_reg(compress_setting, AST_VIDEOCAP_COMPRESS_CTRL);

	return 0;
}

void update_cursor_position(void)
{
	uint32_t cursor_position;

	cursor_position = ast_videocap_read_reg(AST_VIDEOCAP_HW_CURSOR_POSITION);
	if (cursor_position != prev_cursor_position) { /* position is changed */
		cursor_pos_x = (uint16_t) (cursor_position & 0x00000FFF);
		cursor_pos_y = (uint16_t) ((cursor_position >> 16) & 0x7FF);
		prev_cursor_position = cursor_position;
	}
}

int ast_videocap_create_cursor_packet(struct ast_videocap_engine_info_t *info, void * ioc)
{
	struct ast_videocap_cursor_info_t *cursor_info;
	ASTCap_Ioctl *ioctl_ptr = (ASTCap_Ioctl *)ioc;
	void *cursor_pattern_addr;
	uint32_t cursor_status;
	uint32_t cursor_checksum;

	cursor_status = ast_videocap_read_reg(AST_VIDEOCAP_HW_CURSOR_STATUS);

	if ((cursor_status & AST_VIDEOCAP_HW_CURSOR_STATUS_DRIVER_MASK) != AST_VIDEOCAP_HW_CURSOR_STATUS_DRIVER_MGAIC) { /* VGA driver is not installed */
		//printk("VGA driver is not installed\n");
		return -1;
	}

	if (!(cursor_status & AST_VIDEOCAP_HW_CURSOR_STATUS_ENABLE)) { /* HW cursor is disabled */
		//printk("HW cursor is disabled\n");
		return -1;
	}

	update_cursor_position();
	cursor_pattern_addr = vga_mem_virt_addr + ast_videocap_read_reg(AST_VIDEOCAP_HW_CURSOR_PATTERN_ADDR);
	cursor_checksum = ioread32((void * __iomem)cursor_pattern_addr + AST_VIDEOCAP_CURSOR_BITMAP_DATA); /* cursor checksum is behind the cursor pattern data */

	if ((cursor_status != prev_cursor_status) || /* type or offset changed */
		(cursor_pattern_addr != prev_cursor_pattern_addr) || /* pattern address changed */
		(cursor_checksum != prev_cursor_checksum) || /* pattern changed */
		(cursor_pos_x != prev_cursor_pos_x) || (cursor_pos_y != prev_cursor_pos_y)) { /* position changed */
		cursor_info = (struct ast_videocap_cursor_info_t *) (AST_VIDEOCAP_HDR_BUF_ADDR + AST_VIDEOCAP_HDR_BUF_VIDEO_SZ);
		cursor_info->type = (cursor_status & AST_VIDEOCAP_HW_CURSOR_STATUS_TYPE) ? AST_VIDEOCAP_HW_CURSOR_TYPE_COLOR : AST_VIDEOCAP_HW_CURSOR_TYPE_MONOCHROME;
		cursor_info->pos_x = cursor_pos_x;
		cursor_info->pos_y = cursor_pos_y;
		cursor_info->offset_x = (uint16_t) ((cursor_status >> 24) & 0x3F); /* bits 29:24 */
		cursor_info->offset_y = (uint16_t) ((cursor_status >> 16) & 0x3F); /* bits 21:16 */
		cursor_info->checksum = cursor_checksum;

		/* only send the cursor pattern to client when checksum is changed */
		if ((cursor_checksum != prev_cursor_checksum) || (cursor_pattern_addr != prev_cursor_pattern_addr)) { /* pattern is changed */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,11))
			dmac_inv_range(cursor_pattern_addr, cursor_pattern_addr + AST_VIDEOCAP_CURSOR_BITMAP_DATA);
#else
#if !defined(SOC_AST2500) && !defined(SOC_AST2530) && !defined(SOC_AST2600)
			dmac_map_area(cursor_pattern_addr,AST_VIDEOCAP_CURSOR_BITMAP_DATA,DMA_BIDIRECTIONAL);
			outer_inv_range((unsigned long)cursor_pattern_addr,(unsigned long)cursor_pattern_addr+AST_VIDEOCAP_CURSOR_BITMAP_DATA);
#endif
#endif
			memcpy((void *) cursor_info->pattern, cursor_pattern_addr, AST_VIDEOCAP_CURSOR_BITMAP_DATA);
			prev_cursor_checksum = cursor_checksum;
			ioctl_ptr->Size = sizeof(struct ast_videocap_cursor_info_t);
		} else {
			ioctl_ptr->Size = sizeof(struct ast_videocap_cursor_info_t) - AST_VIDEOCAP_CURSOR_BITMAP_DATA;
		}

		if(0 == access_ok(ioctl_ptr->vPtr, sizeof(struct ast_videocap_cursor_info_t)))
		{
			printk("access ok failed in ast_videocap_cursor_info_packet\n");
		    return -1;

		}
		if (copy_to_user(ioctl_ptr->vPtr, cursor_info, sizeof(struct ast_videocap_cursor_info_t)))
		{
		    printk("copy_to_user failed in ast_videocap_cursor_info_packet\n");
		    return -1;
		}

		/* save current status */
		prev_cursor_status = cursor_status;
		prev_cursor_pos_x = cursor_pos_x;
		prev_cursor_pos_y = cursor_pos_y;
		prev_cursor_pattern_addr = cursor_pattern_addr;
		return 0;
	} else { /* no change */
		return -1;
	}
}

int partialjpeg(struct ast_videocap_engine_info_t *info, int X0, int Y0, int X1, int Y1)
{

	int bpp = (info->FrameHeader.Mode420 != 0) ? BLOCK_SIZE_YUV420 : BLOCK_SIZE_YUV444;
	int width = (X1 - X0 + 1) * bpp;
	int height = (Y1 - Y0 + 1) * bpp;
	uint32_t VR044_buff_addr = ast_videocap_read_reg(AST_VIDEOCAP_SRC_BUF1_ADDR);
	unsigned long VR044_move_pos = 0;
	unsigned long VR0314_buff_end_addr = 0;
	unsigned long VR0318_buff_start_addr = 0;

	//calculate the address of first changed tile to move into the video source buffer
	VR044_move_pos = (unsigned long)((ast_videocap_read_reg(AST_VIDEOCAP_SCAN_LINE_OFFSET)) * bpp * Y0 + 256 * X0);

	//Step 5.	Clear CBD flag buffer with 0x1 and disable BCD in VR02C.
	//Clear BCD buffer by settin 0x1
	memset(AST_VIDEOCAP_FLAG_BUF_ADDR, 0x1, AST_VIDEOCAP_FLAG_BUF_SZ);

	//Disable BCD
	ast_videocap_write_reg(AST_VIDEOCAP_BCD_CTRL_DISABLE, AST_VIDEOCAP_BCD_CTRL);

	//Step 6.	Trigger JPEG compression base on the minimum bonding box.
	// VR044 needs to be programmed to the byte address of the first changed block in YUV data buffer which is source buffer #1 (base address+offset).
	// In YUV444 mode:
	// Address = Address_in_VR044+VR048*8*Y0+256*X0
	if (VR044_move_pos > 0)
	{
		ast_videocap_write_reg((unsigned long)(VR044_buff_addr + VR044_move_pos), AST_VIDEOCAP_SRC_BUF1_ADDR);
	}

	//BACKUP the register address
	VR0314_buff_end_addr = (unsigned long)ast_videocap_read_reg(AST_VIDEOCAP_MEM_RESTRICT_END);
	VR0318_buff_start_addr = (unsigned long)ast_videocap_read_reg(AST_VIDEOCAP_MEM_RESTRICT_START);

	//Update the register
	ast_videocap_write_reg(ast_videocap_read_reg(AST_VIDEOCAP_COMPRESSED_BUF_ADDR), AST_VIDEOCAP_MEM_RESTRICT_START);
	ast_videocap_write_reg(ast_videocap_read_reg(AST_VIDEOCAP_COMPRESSED_BUF_ADDR) + AST_VIDEOCAP_COMPRESS_BUF_SZ, AST_VIDEOCAP_MEM_RESTRICT_END);

	// VR034: window width and height needs to be reprogrammed.
	ast_videocap_write_reg((width << AST_VIDEOCAP_WINDOWS_H_SHIFT) + height, AST_VIDEOCAP_CAPTURE_WINDOW);
	ast_videocap_write_reg((width << AST_VIDEOCAP_WINDOWS_H_SHIFT) + height, AST_VIDEOCAP_COMPRESS_WINDOW);

	// clear and set the jpeg compression mode.
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_JPEG);
	ast_videocap_clear_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS_JPEG_FRAME);

	ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_JPEG);
	ast_videocap_set_reg_bits(AST_VIDEOCAP_SEQ_CTRL, AST_VIDEOCAP_SEQ_CTRL_COMPRESS_JPEG_FRAME);

#ifdef SOC_AST2600
	/* Fixes capture engine busy issue when direct mode is enabled for host resolution >= 1024x768 */
	if (info->INFData.DirectMode == 0)
#endif
	{
		ast_videocap_clear_reg_bits(AST_VIDEOCAP_CTRL, AST_VIDEOCAP_CTRL_DIRECT_FETCH_FRAME_BUF);
	}

	//Delay may required, But to avoid the engine busy it will be useful.
	udelay(100);

	if (ast_videocap_automode_trigger() < 0)
	{
		printk("[%s:%d] ast_videocap_automode_trigger failed!\n", __FUNCTION__, __LINE__);
		return ASTCAP_IOCTL_BLANK_SCREEN;
	}

	//RESTORE

	//7.Restore VR044 with original source buffer address.
	if (VR044_move_pos > 0)
		ast_videocap_write_reg((unsigned long)VR044_buff_addr, AST_VIDEOCAP_SRC_BUF1_ADDR);

	//restore original
	ast_videocap_write_reg(VR0318_buff_start_addr, AST_VIDEOCAP_MEM_RESTRICT_START);
	ast_videocap_write_reg(VR0314_buff_end_addr, AST_VIDEOCAP_MEM_RESTRICT_END);

	return 0;
}

int create_bonding_box(unsigned char *addr, struct ast_videocap_engine_info_t *info)
{
	int i = 0, j = 0;
	int width = 0, height = 0;
	int tile_count = 0;
	int temp_width = 0, temp_height = 0;
	int start_x = -1, start_y = -1, end_x = 0, end_y = 0;
	int block_size = BLOCK_SIZE_YUV444;

	if ((info->src_mode.x <= 0) || (info->src_mode.y <= 0))
	{
		printk("[%s:%d] Invalid %s (%d) for creating bonding box!\n", __FUNCTION__, __LINE__, (info->src_mode.x <= 0) ? "width" : "height", (info->src_mode.x <= 0) ? info->src_mode.x : info->src_mode.y);
		return 0;
	}

	if (info->FrameHeader.Mode420 != 0)
	{
		block_size = BLOCK_SIZE_YUV420;
	}

	width = (info->src_mode.x / block_size);
	height = (info->src_mode.y / block_size);

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
#if defined(SOC_AST2600)
			if (0xf != (0xf & addr[i + (j * width)]))
#else
			if (0xf == addr[i + j * width])
#endif
			{
				if (start_x == -1)
					start_x = end_x = i;

				if (start_y == -1)
					start_y = end_y = j;

				if (i < start_x)
					start_x = i;

				if (j < start_y)
					start_y = j;

				if (i > end_x)
					end_x = i;

				if (j > end_y)
					end_y = j;

				tile_count++;
			}
		}
	}

	if ((end_x + block_size) >= (info->src_mode.x / block_size))
		end_x = (info->src_mode.x / block_size) - 1;
	else
		end_x = (end_x + block_size);

	if ((end_y + block_size) >= (info->src_mode.y / block_size))
		end_y = (info->src_mode.y / block_size) - 1;
	else
		end_y = (end_y + block_size);

	/* width, height of bonding box */
	width = (end_x - (start_x) + 1) * block_size;
	height = (end_y - (start_y) + 1) * block_size;

	/* To check overflow */
	temp_width = (start_x * block_size) + width;
	temp_height = (start_y * block_size) + height;

	// In case of overflow, adjust the width / height values
	// accroding to current host video resolution
	if (temp_width > info->src_mode.x)
	{
		width -= (temp_width - info->src_mode.x);
	}

	if (temp_height > info->src_mode.y)
	{
		height -= (temp_height - info->src_mode.y);
	}

	if (tile_count > 0)
	{
		// trigger partial JPEG capture & compression
		if (partialjpeg(info, start_x, start_y, end_x, end_y) == 0)
#if defined(SOC_AST2600)
		{
			memset(&tile_info,0,sizeof(struct ast_videocap_jpeg_tile_info_t));
			tile_info.pos_x = start_x;
			tile_info.pos_y = start_y;
			tile_info.width = width;
			tile_info.height = height;
			tile_info.compressed_size = ast_videocap_read_reg(AST_VIDEOCAP_COMPRESSED_DATA_COUNT) * AST_VIDEOCAP_COMPRESSED_DATA_COUNT_UNIT;
		}
		else
		{
			tile_info.pos_x = tile_info.pos_y = tile_info.width = tile_info.height = tile_info.compressed_size = 0;
			return ASTCAP_IOCTL_BLANK_SCREEN;
		}
		tile_info.resolution_x = info->src_mode.x;
		tile_info.resolution_y = info->src_mode.y;
		tile_info.bpp = block_size;

#else
		{
			tile_info[0].pos_x = start_x;
			tile_info[0].pos_y = start_y;
			tile_info[0].width = width;
			tile_info[0].height = height;
			tile_info[0].compressed_size = ast_videocap_read_reg(AST_VIDEOCAP_COMPRESSED_DATA_COUNT) * AST_VIDEOCAP_COMPRESSED_DATA_COUNT_UNIT;
		}
		else
		{
			tile_info[0].pos_x = tile_info[0].pos_y = tile_info[0].width = tile_info[0].height = tile_info[0].compressed_size = 0;
			return ASTCAP_IOCTL_BLANK_SCREEN;
		}
#endif
	}

	return tile_count;
}


int VideoCapture(struct ast_videocap_engine_info_t *info)
{
	uint32_t vga_status;

	/* Check if internal VGA screen is off */
	vga_status = ast_videocap_read_reg(AST_VIDEOCAP_VGA_STATUS) >> 24;
	if ((vga_status == 0x10) || (vga_status == 0x11) || (vga_status & 0xC0) || (vga_status & 0x04) || (!(vga_status & 0x3B))) {
		info->NoSignal = 1;
		udelay(100);
		return ASTCAP_IOCTL_BLANK_SCREEN;
	} else {
		info->NoSignal = 0;
	}

	if (ISRDetectedModeOutOfLock) {
		ast_videocap_data_in_old_video_buf = 0;
		if (ast_videocap_mode_detection(info,1) < 0) {
			return ASTCAP_IOCTL_BLANK_SCREEN;
		}
	}

	ast_videocap_engine_init(info);
	ast_videocap_horizontal_down_scaling(info);
	ast_videocap_vertical_down_scaling(info);

	if (info->INFData.AutoMode) {
		if (ast_videocap_automode_trigger() < 0) {
			return ASTCAP_IOCTL_BLANK_SCREEN;
		}
	} else {
	
		if (ast_videocap_trigger_capture() < 0) {
			return ASTCAP_IOCTL_BLANK_SCREEN;
		}

		// If RC4 is required, here is where we enable it

		if (ast_videocap_trigger_compression() < 0) {
			return ASTCAP_IOCTL_BLANK_SCREEN;
		}
	}

	ast_videocap_data_in_old_video_buf = 1;
	info->FrameHeader.NumberOfMB = ast_videocap_read_reg(AST_VIDEOCAP_COMPRESSED_BLOCK_COUNT) >> AST_VIDEOCAP_COMPRESSED_BLOCK_COUNT_SHIFT;

	if (CaptureMode == VIDEOCAP_JPEG_SUPPORT) //jpeg
	{
		if (full_screen_capture == 1)
		{
			//Clear BCD buffer by settin 0x1
			memset(AST_VIDEOCAP_FLAG_BUF_ADDR, 0x1, AST_VIDEOCAP_FLAG_BUF_SZ);

			memset(&tile_info,0,sizeof(struct ast_videocap_jpeg_tile_info_t));

			tile_info.pos_x = 0;

			tile_info.pos_y = 0;
			tile_info.width = info->src_mode.x;
			tile_info.height = info->src_mode.y;

			tile_info.compressed_size = ast_videocap_read_reg(AST_VIDEOCAP_COMPRESSED_DATA_COUNT) * AST_VIDEOCAP_COMPRESSED_DATA_COUNT_UNIT;
#if defined(SOC_AST2600)
			tile_info.resolution_x = info->src_mode.x;
			tile_info.resolution_y = info->src_mode.y;
			tile_info.bpp = (info->FrameHeader.Mode420 != 0) ? BLOCK_SIZE_YUV420 : BLOCK_SIZE_YUV444;
#endif
		}
		else if ((info->FrameHeader.NumberOfMB > 0) && (full_screen_capture == 0))
		{
			//calculate the bonding box ,recapture,get partial jpeg and update the MB
			info->FrameHeader.NumberOfMB = create_bonding_box(AST_VIDEOCAP_FLAG_BUF_ADDR, info);
		}
		else
		{
			// will not execute (just for reference)
		}
	}

	info->CompressData.CompressSize = ast_videocap_read_reg(AST_VIDEOCAP_COMPRESSED_DATA_COUNT) * AST_VIDEOCAP_COMPRESSED_DATA_COUNT_UNIT;

	update_cursor_position();

	if (CaptureMode == VIDEOCAP_JPEG_SUPPORT)
	{
		/* [AST2500] Capture engine busy happens in case of host resolution 1600x900 and higher.
		** so force full screen capture for host video 1600x900 and higher.
		**
		** Issue fixed on AST2600 A1/A2/A3 */
#ifdef SOC_AST2500
		if(info->src_mode.x < 1600)
#endif
		{
			full_screen_capture = 0; /* video driver will capture partial frame next time */
		}

		return (info->FrameHeader.NumberOfMB == 0) ? ASTCAP_IOCTL_NO_VIDEO_CHANGE : ASTCAP_IOCTL_SUCCESS;
	}
	return (info->CompressData.CompressSize == 12) ? ASTCAP_IOCTL_NO_VIDEO_CHANGE : ASTCAP_IOCTL_SUCCESS;
}

int ast_videocap_create_video_packet(struct ast_videocap_engine_info_t *info, void *ioc)
{
	struct ast_videocap_video_hdr_t *video_hdr = NULL;
	ASTCap_Ioctl *ioctl_ptr = (ASTCap_Ioctl *)ioc;
	unsigned long compressed_buf_size;
	unsigned long VideoCapStatus;
	int ret=0;

	/* Capture Video */
	VideoCapStatus = VideoCapture(info);
	if (VideoCapStatus != ASTCAP_IOCTL_SUCCESS) {
		ioctl_ptr->Size = 0;
		return VideoCapStatus;
	}

	/* Check if Mode changed while capturing */
	if (ISRDetectedModeOutOfLock) {
		/* Send a blank screen this time */
		ioctl_ptr->Size = 0;
		return ASTCAP_IOCTL_BLANK_SCREEN;
	}

	compressed_buf_size = info->CompressData.CompressSize;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,4,11))
  dmac_inv_range((void *) AST_VIDEOCAP_COMPRESS_BUF_ADDR, (void *) AST_VIDEOCAP_COMPRESS_BUF_ADDR + AST_VIDEOCAP_COMPRESS_BUF_SZ);
#else
#if !defined(SOC_AST2500) && !defined(SOC_AST2530) && !defined(SOC_AST2600)
	dmac_map_area((void *) AST_VIDEOCAP_COMPRESS_BUF_ADDR,AST_VIDEOCAP_COMPRESS_BUF_SZ,DMA_FROM_DEVICE);
    outer_inv_range((unsigned long)AST_VIDEOCAP_COMPRESS_BUF_ADDR,(unsigned long)AST_VIDEOCAP_COMPRESS_BUF_ADDR+AST_VIDEOCAP_COMPRESS_BUF_SZ);
#endif
#endif

	/* fill AST video data header */
	video_hdr = (struct ast_videocap_video_hdr_t *) (AST_VIDEOCAP_HDR_BUF_ADDR);
	video_hdr->iEngVersion = 1;
	video_hdr->wHeaderLen = AST_VIDEOCAP_VIDEO_HEADER_SIZE;
	video_hdr->CompressData_CompressSize = compressed_buf_size;

	video_hdr->SourceMode_X = info->src_mode.x;
	video_hdr->SourceMode_Y = info->src_mode.y;
	video_hdr->DestinationMode_X = info->src_mode.x;
	video_hdr->DestinationMode_Y = info->src_mode.y;

	video_hdr->FrameHdr_RC4Enable = info->FrameHeader.RC4Enable;
	video_hdr->FrameHdr_JPEGScaleFactor = info->FrameHeader.JPEGScaleFactor;
	video_hdr->FrameHdr_Mode420 = info->FrameHeader.Mode420;
	video_hdr->FrameHdr_NumberOfMB = info->FrameHeader.NumberOfMB;
	video_hdr->FrameHdr_AdvanceScaleFactor = info->FrameHeader.AdvanceScaleFactor;
	video_hdr->FrameHdr_JPEGTableSelector = info->FrameHeader.JPEGTableSelector;
	video_hdr->FrameHdr_AdvanceTableSelector = info->FrameHeader.AdvanceTableSelector;
	video_hdr->FrameHdr_JPEGYUVTableMapping = info->FrameHeader.JPEGYUVTableMapping;
	video_hdr->FrameHdr_SharpModeSelection = info->FrameHeader.SharpModeSelection;
	video_hdr->FrameHdr_CompressionMode = info->FrameHeader.CompressionMode;
	video_hdr->FrameHdr_RC4Reset = info->FrameHeader.RC4Reset;

	video_hdr->InfData_DownScalingMethod = info->INFData.DownScalingMethod;
	video_hdr->InfData_DifferentialSetting = info->INFData.DifferentialSetting;
	video_hdr->InfData_AnalogDifferentialThreshold = info->INFData.AnalogDifferentialThreshold;
	video_hdr->InfData_DigitalDifferentialThreshold = info->INFData.DigitalDifferentialThreshold;
	video_hdr->InfData_AutoMode = info->INFData.AutoMode;
	video_hdr->InfData_VQMode = info->INFData.VQMode;

	video_hdr->Cursor_XPos = cursor_pos_x;
	video_hdr->Cursor_YPos = cursor_pos_y;

	ioctl_ptr->Size = compressed_buf_size;

	if(CaptureMode != VIDEOCAP_JPEG_SUPPORT)
	{
		ret = access_ok(ioctl_ptr->vPtr, sizeof(struct ast_videocap_video_hdr_t));
		if (ret == 0)
		{
		    printk("access ok failed in ast_videocap_create_video_packet\n");
		    return ASTCAP_IOCTL_BLANK_SCREEN;
		}
		
		if (copy_to_user(ioctl_ptr->vPtr, video_hdr, sizeof(struct ast_videocap_video_hdr_t)))
		{
		    printk("copy_to_user failed in ast_videocap_create_video_packet\n");
		    return ASTCAP_IOCTL_BLANK_SCREEN;
		}
	}
	return ASTCAP_IOCTL_SUCCESS;
}
