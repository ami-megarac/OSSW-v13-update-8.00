/****************************************************************
 ****************************************************************
 **                                                            **
 **    (C)Copyright 2009-2018, American Megatrends Inc.        **
 **                                                            **
 **            All Rights Reserved.                            **
 **                                                            **
 **        5555 Oakbrook Pkwy Suite 200, Norcross              **
 **                                                            **
 **        Georgia - 30093, USA. Phone-(770)-246-8600.         **
 **                                                            **
 ****************************************************************
 ****************************************************************/

/****************************************************************
 *
 * pilot_fb.c
 * PILOT frame buffer driver
 *
 ****************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/platform_device.h>

#define PILOT_FB_DRV_NAME "pilotfb"
#define PILOT_GRAPHICS_REG_BASE 0x80000000 // DDR begin

uint32_t GetPhyFBStartAddress(unsigned long *vga_memory);

struct pilotvga_par
{
	struct platform_device *pdev;
	struct fb_info *info;
	void __iomem *video_base;
	u32 pseudo_palette[17];
};

static struct fb_fix_screeninfo pilotvgafb_fix = {
	.id = PILOT_FB_DRV_NAME,
	.type = FB_TYPE_PACKED_PIXELS,
	.visual = FB_VISUAL_PSEUDOCOLOR,
	.xpanstep = 1,
	.ypanstep = 1,
	.ywrapstep = 1,
	.accel = FB_ACCEL_NONE
};

static struct fb_ops pilotvgafb_ops = {
	.owner = THIS_MODULE,
	.fb_fillrect = cfb_fillrect,
	.fb_copyarea = cfb_copyarea,
	.fb_imageblit = cfb_imageblit
};

static void pilot_release(struct device *dev) {}

static int pilotvgafb_probe(struct platform_device *pdev)
{
	struct fb_info *info;
	struct device *device = &pdev->dev;
	unsigned long vga_memory = 0;
	uint32_t fb_start_address = 0;
	int retval = 0;

	info = framebuffer_alloc(sizeof(struct pilotvga_par), device);

	if (!info)
	{
		printk(KERN_ERR "Could not allocate memory for pilot_info.\n");
		retval = -ENODEV;
		goto err_alloc;
	}

	printk("map to vga memory \n");

	if (-ENOMEM == (fb_start_address = GetPhyFBStartAddress(&vga_memory)))
	{
		printk(KERN_ERR "Could not get Pilot FB start address\n");
		retval = -ENOMEM;
		goto err_map;
	}

	info->screen_base = ioremap(fb_start_address, vga_memory);
	if (info->screen_base == NULL)
	{
		printk(KERN_ERR "Pilot FB: map to VGA memory failed\n");
		retval = -ENOMEM;
		goto err_map;
	}
	
	info->fbops = &pilotvgafb_ops;
	info->fix = pilotvgafb_fix;

	info->fix.smem_start = fb_start_address;
	info->fix.smem_len = vga_memory;

	info->pseudo_palette = info->par;
	info->par = NULL;
	info->flags = FBINFO_DEFAULT;

	if (register_framebuffer(info) < 0)
	{
		retval = -EINVAL;
		goto err_reg;
	}

	device->release = pilot_release;

	fb_info(info, "%s frame buffer device\n", info->fix.id);
	printk(KERN_INFO "FB: got physical memory pool for size (%d on %08lx bus)\n", info->fix.smem_len, (unsigned long)info->fix.smem_start);
	platform_set_drvdata(pdev, info);
	return retval;

err_reg:
	iounmap(info->screen_base);
err_map:
	framebuffer_release(info);
err_alloc:
	return retval;
}

static int pilotvgafb_remove(struct platform_device *pdev)
{
	struct fb_info *info = platform_get_drvdata(pdev);
	if (info)
	{
		unregister_framebuffer(info);
		iounmap(info->screen_base);
		framebuffer_release(info);
	}
	return 0;
}

static struct platform_driver pilotvgafb_driver = {
	.probe = pilotvgafb_probe,
	.remove = pilotvgafb_remove,
	.driver = {
		.name = PILOT_FB_DRV_NAME,
	},
};

static struct platform_device pilotfb_device = {
	.name = PILOT_FB_DRV_NAME,
};

int __init pilotfb_init(void)
{
	int ret;
	ret = platform_driver_register(&pilotvgafb_driver);

	if (!ret)
	{
		ret = platform_device_register(&pilotfb_device);
		if (ret)
			platform_driver_unregister(&pilotvgafb_driver);
	}
	return ret;
}

static void __exit pilotfb_exit(void)
{
	platform_device_unregister(&pilotfb_device);
	platform_driver_unregister(&pilotvgafb_driver);
}

uint32_t GetPhyFBStartAddress(unsigned long *vga_memory)
{
	unsigned long DRAM_Data = 0;
	unsigned long TotalMemory = 0;
	unsigned long VGAMemory = 0;
	void *ast_sdram_reg_virt_base = NULL;

	if (NULL == (ast_sdram_reg_virt_base = ioremap(0x1E6E0000, SZ_128K)))
	{
		printk(KERN_WARNING "pilotfb: ast_sdram_reg_virt_base ioremap failed\n");
		return -ENOMEM;
	}

	DRAM_Data = ioread32(ast_sdram_reg_virt_base + 0x04);
	switch (DRAM_Data & 0x03)
	{
	case 0:
		TotalMemory = 0x10000000;
		break;
	case 1:
		TotalMemory = 0x20000000;
		break;
	case 2:
		TotalMemory = 0x40000000;
		break;
	case 3:
		TotalMemory = 0x80000000;
		break;
	}

	switch ((DRAM_Data >> 2) & 0x03)
	{
	case 0:
		VGAMemory = 0x800000;
		break;
	case 1:
		VGAMemory = 0x1000000;
		break;
	case 2:
		VGAMemory = 0x2000000;
		break;
	case 3:
		VGAMemory = 0x4000000;
		break;
	}

	*vga_memory = VGAMemory;
	iounmap(ast_sdram_reg_virt_base);

	return ((PILOT_GRAPHICS_REG_BASE + TotalMemory - VGAMemory));
}
module_init(pilotfb_init);
module_exit(pilotfb_exit);

MODULE_AUTHOR("American Megatrends Inc.");
MODULE_DESCRIPTION("PILOT frame buffer driver");
MODULE_LICENSE("GPL v2");
