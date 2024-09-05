/*
 ****************************************************************
 **                                                            **
 **    (C)Copyright 2009-2015, American Megatrends Inc.        **
 **                                                            **
 **            All Rights Reserved.                            **
 **                                                            **
 **        5555 Oakbrook Pkwy Suite 200, Norcross,             **
 **                                                            **
 **        Georgia - 30093, USA. Phone-(770)-246-8600.         **
 **                                                            **
 ****************************************************************
 */

#ifndef MODULE			/* If not defined in Makefile */
#define MODULE
#endif

#ifndef __KERNEL__		/* If not defined in Makefile */
#define __KERNEL__
#endif

#include <linux/module.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/version.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <asm/io.h>
#include "fmh.h"
#include "helper.h"

#include <linux/mtd/mtd.h>
//#include <linux/mtd/map.h>
//#include <linux/mtd/partitions.h>

#ifdef CONFIG_SPX_FEATURE_GLOBAL_DUAL_IMAGE_SUPPORT
#include <linux/fs.h>      // Needed by filp
#endif
#include <asm/uaccess.h>   // Needed by segment descriptors

extern unsigned long FlashStart;
extern unsigned long FlashSize;
extern unsigned long EnvStart;
extern unsigned long EnvSize;
extern unsigned long FlashSectorSize;
extern unsigned long UsedFlashStart;
extern unsigned long UsedFlashSize;

extern int GetMacroPropValue(char *devnode,char *prop, char *propvalue);

#ifdef CONFIG_SPX_FEATURE_GLOBAL_BKUP_FLASH_BANKS
#define MAX_BANKS (CONFIG_SPX_FEATURE_GLOBAL_FLASH_BANKS + CONFIG_SPX_FEATURE_GLOBAL_BKUP_FLASH_BANKS)
#else
#define MAX_BANKS CONFIG_SPX_FEATURE_GLOBAL_FLASH_BANKS
#endif

extern struct mtd_info *ractrends_mtd[MAX_BANKS];

#ifdef CONFIG_SPX_FEATURE_DISABLE_MTD_SUPPORT
unsigned int g_MTDSupport = 0;
#else
unsigned int g_MTDSupport = 1;
#endif


#define MAX_LINE_LEN 250
#define IMAGE_1 1
#define IMAGE_2 2
#ifdef CONFIG_SPX_FEATURE_GLOBAL_DUAL_IMAGE_SUPPORT
    static char  running_image;
#endif
#ifdef CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT
extern unsigned char broken_spi_banks;// specify the number of broken SPI flash bank
#endif

/*this returns offset not address*/
static long
ScanFirmwareModule(unsigned long FlashStartOffset, unsigned long FlashSize,
				unsigned long SectorSize, unsigned char* buf,FLASH_INFO* flinfo)
{
	unsigned long SectorCount = FlashSize/SectorSize;
	unsigned long i=0,prev_fmh_sec_loc=0;
	FMH fmh;
	MODULE_INFO *mod = NULL;
	unsigned long ModuleOffset;
	unsigned long Size, Location;
	int fmh_found = 0;
	size_t NumBytesRead;

	for (i=0;i<SectorCount;)
	{
		/* Scan the flash and get FMH*/
		fmh_found = ScanforFMH_UseReadFn((FlashStartOffset+(i*SectorSize)),SectorSize,flinfo,&fmh);
		if (fmh_found == 0)
		{
			i++;
			continue;
		}

		mod = &(fmh.Module_Info);

		/* Check if module type is FIRMWARE */
		if (le16_to_host(mod->Module_Type) == MODULE_FMH_FIRMWARE)
			break;

		if( (le16_to_host(mod->Module_Type) == MODULE_BOOTLOADER) && (strncmp(mod->Module_Name,"pfm",sizeof("pfm")) != 0) )
		{
			i = prev_fmh_sec_loc + (le32_to_host(fmh.FMH_AllocatedSize)/SectorSize);
			prev_fmh_sec_loc = i;   
		}
		else if(strncmp(mod->Module_Name,"pfm",sizeof("pfm")) == 0)
		{
			i+=1;
		}
		else
		{
			i+=(le32_to_host(fmh.FMH_AllocatedSize)/SectorSize);
			prev_fmh_sec_loc = i;
		}
		memset(&fmh,0,sizeof(FMH));
	}

	if (i >= SectorCount)
	{
		printk(KERN_DEBUG "Unable to Locate MODULE_FMH_FIRMWARE\n");
		return -1;
	}

	if(mod != NULL)
	{
		Size = le32_to_host(mod->Module_Size);
		Location = le32_to_host(mod->Module_Location);
	}
	else
	{
		printk(KERN_ERR "Error: dereferencing null pointer\n");
		return -1;
	}

	if(fmh.FMH_Ver_Major == 1 && fmh.FMH_Ver_Minor >= 8)
		ModuleOffset = FlashStartOffset + Location;
	else
		ModuleOffset = FlashStartOffset +(i*SectorSize) + Location;

	//copy here
	if( flinfo->read_flash(ModuleOffset,Size,&NumBytesRead,buf) != 0)
	{
		printk(KERN_ERR "Error reading flash in order to copy MODULE_FMH_FIRMWARE info\n");
		return -1;
	}

	return Size;
	
}

#ifdef CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT
unsigned long get_secondaryimage_offset(void ){
#if defined(CONFIG_SPX_FEATURE_DEDICATED_SPI_FLASH_BANK)
        int i;
        unsigned long offset=0;
#endif
	unsigned long   gbl_flash_start=0;
        unsigned long   gbl_used_flash_start=0;
        unsigned long   gbl_used_flash_size=0;
        char gb_flash_start[MAX_FLASH_SIZE];
        char gb_used_flash_size[MAX_FLASH_SIZE];
        char gb_used_flash_start[MAX_FLASH_SIZE];
        memset(gb_flash_start,0,sizeof(gb_flash_start));
        memset(gb_used_flash_size,0,sizeof(gb_used_flash_size));
        memset(gb_used_flash_start,0,sizeof(gb_used_flash_start));
        if(GetMacroPropValue("flash_macros","global-flash-start",gb_flash_start) == 0)
        {
                gbl_flash_start = simple_strtoull(gb_flash_start, NULL, MAX_FLASH_SIZE);
        }
        if(GetMacroPropValue("flash_macros","global-used-flash-size",gb_used_flash_size) == 0)
        {
                gbl_used_flash_size = simple_strtoull(gb_used_flash_size, NULL, MAX_FLASH_SIZE);
        }
        if(GetMacroPropValue("flash_macros","global-used-flash-start",gb_used_flash_start) == 0)
        {
                gbl_used_flash_start = simple_strtoull(gb_used_flash_start, NULL, MAX_FLASH_SIZE);
        }

#if defined(CONFIG_SPX_FEATURE_CONTIGIOUS_SPI_MEMORY) || defined(CONFIG_SPX_FEATURE_SINGLE_SPI_ABR)
	return gbl_used_flash_size;
#elif defined(CONFIG_SPX_FEATURE_DEDICATED_SPI_FLASH_BANK)
	for(i=0;i<CONFIG_SPX_FEATURE_SECONDARY_IMAGE_SPI;i++){
#ifdef CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT
		if (broken_spi_banks == 1)
			offset = (gbl_used_flash_start - gbl_flash_start);
		else
#endif
		{
#ifdef CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT
                        offset=FlashSize;
#else
			offset = (gbl_used_flash_start - gbl_flash_start + gbl_used_flash_size);
#endif
		}
	}
	return offset;
#endif
}
#endif

/**
 * @fn GetCurrentRunningImage
 * @brief To Get the cuurent Running Image
 * @param[in]  - No parameters.
 * @retval       - Running image
                        0 -IMAGE_1 X_LINE_LEN
                        1 -IMAGE_2.                         
 */
int GetCurrentRunningImage(void)
{
    struct file *f;
    mm_segment_t fs;	
    char buf[MAX_LINE_LEN];
    char *ptr = NULL;	
    int  img;
    
    f = filp_open("/proc/cmdline", O_RDONLY, 0);
    if(f == NULL)
    {
        printk(KERN_ALERT "file_open error!!.\n");
        return -1;
    }		
    fs = get_fs();
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(3,14,17))
    set_fs(get_ds());
#else
    set_fs(KERNEL_DS);
#endif
    memset(buf,0,MAX_LINE_LEN);
    f->f_op->read(f, buf,MAX_LINE_LEN, &f->f_pos);
    set_fs(fs);
    filp_close(f,NULL);

    ptr = strstr(buf,"imagebooted");
    if(ptr == NULL)
        return -1;
    ptr = strstr(ptr,"=");
    if(ptr == NULL)
        return -1;
    ptr++;
	
    /* In /proc/cmdline image argument specifies 1 -Image1 and 2 -Image2	*/
    img = *ptr - 48;

    if( (img != IMAGE_1) && (img != IMAGE_2) )
        return -1;

return img;
}



/************************Translation layer for reading from flash**********************************/
int mtd_read_thunk(unsigned long Offset,unsigned long NumBytes,size_t* NumBytesRead,unsigned char* Buffer)
{
   
     struct mtd_info *ractrendsmtd = NULL;

	ractrendsmtd = get_mtd_device(NULL,0);
	if(ractrendsmtd == NULL || IS_ERR(ractrends_mtd))
	{
		g_MTDSupport = 0;
		return -1;
	}
	else
	{
		g_MTDSupport = 1;
	}
	
	//then read from the mtd device for the required params
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
	return ractrendsmtd->_read(ractrendsmtd,(loff_t)Offset,NumBytes,NumBytesRead,Buffer);
#else
	return ractrendsmtd->read(ractrendsmtd,(loff_t)Offset,NumBytes,NumBytesRead,Buffer);
#endif

}

char* g_cached_flashbanksize_buf;
/* ----------------Proc File System Read Hardware Information ---------------------*/
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
int
flashbanksize_read(struct file *file,  char __user *buf, size_t count, loff_t *offset)
#else
int
flashbanksize_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
#endif
{
        static long flashbank_size = 0;
        static int firsttime = 1;
        int bank = 0,ret = 0;
        char flahbankinfo[512] = {0};

        /* We don't support seeked reads. read should start from 0 */    
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
        if (*offset != 0)
            return 0;
#else
        *eof=1;
        if (offset != 0)
            return 0;
#endif
        if(!firsttime)
        {
            ///this is not the first time.so we just return what we have read again
            //instead of scanning again
            if ( flashbank_size > 0 ) {     // line added to avoid crash if no fwinfo is available
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
                if (copy_to_user(buf,g_cached_flashbanksize_buf, flashbank_size))
                    return -EFAULT;
#else
                memcpy(buf,g_cached_flashbanksize_buf,flashbank_size);
#endif

            }
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
            *offset+=flashbank_size;
#endif
            return flashbank_size;
        }
        else{
            //this is the first time
            firsttime = 0;
        }
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36))
        /* This is a requirement. So don't change it */
        *start = buf;
#endif
        for (bank = 0 ; bank < MAX_BANKS ; bank ++)
        {
#if defined (CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT) && defined(CONFIG_SPX_FEATURE_DEDICATED_SPI_FLASH_BANK)
		if(bank == 0 && broken_spi_banks == 1)
		{
			ret = snprintf(flahbankinfo+flashbank_size,sizeof(flahbankinfo) - flashbank_size,"bank%dsize=%lx\n",bank,(long unsigned int)0);
                	if(ret < 0 || ret >= (signed)(sizeof(flahbankinfo) - flashbank_size))
                	{	
                        	printk(KERN_ALERT "Buffer Overflow\n");
                        	return 0;
                	}
		}	
		else if(bank == 1 && broken_spi_banks == 2)
                {
			ret = snprintf(flahbankinfo+flashbank_size,sizeof(flahbankinfo) - flashbank_size,"bank%dsize=%lx\n",bank,(long unsigned int)0);
                	if(ret < 0 || ret >= (signed)(sizeof(flahbankinfo) - flashbank_size))
                	{	
                        	printk(KERN_ALERT "Buffer Overflow\n");
                        	return 0;
                	}
                }
		else
		{
	   		ret = snprintf(flahbankinfo+flashbank_size,sizeof(flahbankinfo) - flashbank_size,"bank%dsize=%lx\n",bank,(long unsigned int)FlashSize);
                	if(ret < 0 || ret >= (signed)(sizeof(flahbankinfo) - flashbank_size))
                	{
                        	printk(KERN_ALERT "Buffer Overflow\n");
                       		return 0;
                	}	
		}
#else		
            if(ractrends_mtd[bank]!=NULL)
            {
                ret = snprintf(flahbankinfo+flashbank_size,sizeof(flahbankinfo) - flashbank_size,"bank%dsize=%lx\n",bank,(long unsigned int)ractrends_mtd[bank]->size);
		if(ret < 0 || ret >= (signed)(sizeof(flahbankinfo) - flashbank_size))
		{
			printk(KERN_ALERT "Buffer Overflow\n");
			return 0;
		}
            }
            else
            {
                ret = snprintf(flahbankinfo+flashbank_size,sizeof(flahbankinfo) - flashbank_size,"bank%dsize=0\n",bank);
		if(ret < 0 || ret >= (signed)(sizeof(flahbankinfo) - flashbank_size))
		{
			printk(KERN_ALERT "Buffer Overflow\n");
			return 0;
		}
            }
#endif
            flashbank_size = strlen(flahbankinfo);
        }
        /* Return length of data */
        //remember the fwinfo
	/* Sufficient memory allocated to store the `flashbankinfo`, not writing the Out of bounds */
	/* coverity[alloc_strlen : FALSE] */
        g_cached_flashbanksize_buf = kmalloc(flashbank_size,GFP_KERNEL); /* SCA Fix [Out-of-bounds access]:: False Positive */
        /* Reason for false positive - Sufficient memory allocated to store the `flashbankinfo`, not writing the Out of bounds */
        if(g_cached_flashbanksize_buf == NULL)
        {
            printk(KERN_ERR "Error: Could not allocate memory structure for holding flash bank info\n");
            //always treat like first time therefore
            firsttime = 1;
        }
        else 
        {
            memcpy(g_cached_flashbanksize_buf,flahbankinfo,flashbank_size);
        }
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
        if (copy_to_user(buf,flahbankinfo, flashbank_size))
            return -EFAULT;

        *offset+=flashbank_size;
#else
        memcpy(buf,flahbankinfo,flashbank_size);
#endif
        return flashbank_size;
}

char* g_cached_fwinfo_buf=NULL;
char* g_cached_fwinfo1_buf=NULL;
char* g_cached_fwinfo2_buf=NULL;
/* ----------------Proc File System Read Hardware Information ---------------------*/
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
int
read_fw_info(struct file *file,  char __user *buf, size_t count, loff_t *offset)
#else
int
read_fw_info(char *buf, char **start, off_t offset, int count, int *eof, void *data)
#endif
{
	static long fwInfo_size = 0;	// changed from unsigned to signed due to negative values can be returned
	FLASH_INFO flinfo;
	char *fmhinfo=NULL;	
	static int firsttime = 1;
#ifdef CONFIG_SPX_FEATURE_DEDICATED_SPI_FLASH_BANK
	int i;
#endif
	int copy_size;
	unsigned long   gbl_flash_start=0;
        unsigned long   gbl_used_flash_start=0;
        unsigned long   gbl_used_flash_size=0;
        unsigned long   gbl_erase_block_size=0;
        char gb_erase_block_size[MAX_FLASH_SIZE];
        char gb_flash_start[MAX_FLASH_SIZE];
        char gb_used_flash_size[MAX_FLASH_SIZE];
        char gb_used_flash_start[MAX_FLASH_SIZE];
        memset(gb_flash_start,0,sizeof(gb_flash_start));
        memset(gb_used_flash_size,0,sizeof(gb_used_flash_size));
        memset(gb_used_flash_start,0,sizeof(gb_used_flash_start));
        memset(gb_erase_block_size,0,sizeof(gb_erase_block_size));
        if(GetMacroPropValue("flash_macros","global-flash-start",gb_flash_start) == 0)
        {
                gbl_flash_start = simple_strtoull(gb_flash_start, NULL, MAX_FLASH_SIZE);
        }
        if(GetMacroPropValue("flash_macros","global-used-flash-start",gb_used_flash_start) == 0)
        {
                gbl_used_flash_start = simple_strtoull(gb_used_flash_start, NULL, MAX_FLASH_SIZE);

        }
        if(GetMacroPropValue("flash_macros","global-erase-block-size",gb_erase_block_size) == 0)
        {
                gbl_erase_block_size = simple_strtoull(gb_erase_block_size, NULL, MAX_FLASH_SIZE);
        }
	if(GetMacroPropValue("flash_macros","global-used-flash-size",gb_used_flash_size) == 0)
        {
                gbl_used_flash_size = simple_strtoull(gb_used_flash_size, NULL, MAX_FLASH_SIZE);

        }

/* We don't support seeked reads. read should start from 0 */	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
	if (*offset != 0)
		return 0;
#else
	*eof=1;
	if (offset != 0)
		return 0;
#endif

	if (g_cached_fwinfo_buf == NULL)
	{
		/* FMH Firmware Info can be upto one erase block. So allocate */
		fmhinfo = kmalloc(gbl_erase_block_size,GFP_KERNEL);
		if (fmhinfo == NULL)
		{
			printk("Error: Unable to allocate Memory for FMH Info\n");
			return 0;
		}
		g_cached_fwinfo_buf=fmhinfo;
	}
	
	if(!firsttime)
	{
		///this is not the first time.so we just return what we have read again
		//instead of scanning again
		copy_size = (fwInfo_size > count) ? count : fwInfo_size;

		if ( copy_size  > 0 ) {		// line added to avoid crash if no fwinfo is available
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
	        if (copy_to_user(buf,g_cached_fwinfo_buf, copy_size))
	            return -EFAULT;
#else
	    	memcpy(buf,g_cached_fwinfo_buf,copy_size);
#endif
		}
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
		*offset+=copy_size;
#endif
		return copy_size;
	}
	else
	{
		//this is the first time
		firsttime = 0;
	}

#ifdef CONFIG_SPX_FEATURE_GLOBAL_DUAL_IMAGE_SUPPORT

         running_image = GetCurrentRunningImage();
         if(running_image == -1)
         {
             printk(KERN_ALERT "Unable to get current running image info\n");
             return 0;
         }

	
#endif

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36))
	/* This is a requirement. So don't change it */
	*start = buf;
#endif



	/* Find the SectorSize (erase size) of Flash */

	flinfo.read_flash = mtd_read_thunk;

	if (g_MTDSupport == 1)
	{
		/* Scan and Get FwInfo */
#ifdef CONFIG_SPX_FEATURE_GLOBAL_DUAL_IMAGE_SUPPORT
	unsigned long flashstartoffset;
	 
       flashstartoffset = gbl_used_flash_start - gbl_flash_start;

        if( running_image == IMAGE_2)
        {
#ifdef CONFIG_SPX_FEATURE_DEDICATED_SPI_FLASH_BANK
        	for (i=0;i< CONFIG_SPX_FEATURE_SECONDARY_IMAGE_SPI; i++){
#ifdef CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT
                if (broken_spi_banks == 1)
			flashstartoffset = (gbl_used_flash_start  - gbl_flash_start);
                else
#endif
                {
#ifdef CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT
		    flashstartoffset=FlashSize;
#else
		    flashstartoffset = gbl_used_flash_start - gbl_flash_start + gbl_used_flash_size;
#endif
                }
            }
#else
            flashstartoffset +=  gbl_used_flash_size;
#endif
        }

		fwInfo_size = ScanFirmwareModule(flashstartoffset,
				gbl_used_flash_size,
				gbl_erase_block_size,
				(unsigned char*)fmhinfo,
				&flinfo);

#else
		/* Reason for False Positive - while declaring the variables it is initialize with zero for avoiding to take garbage value or the size conflicts */
                /* coverity[ divide_by_zero : FALSE ] */
		fwInfo_size = ScanFirmwareModule(gbl_used_flash_start - gbl_flash_start,
				gbl_used_flash_size,
				gbl_erase_block_size,
				(unsigned char*)fmhinfo,
				&flinfo);
#endif

		if (fwInfo_size <= 0 )
		{
			printk(KERN_ALERT "Unable to get Firmware Information\n");
			return 0;
		}
	} 
	else
	{
		// These Macro will be extracted from .MAP file or it may come from .PRJ file.
		// These Macro will be used only when get_mtd_device fails
		if(fmhinfo ==NULL)
                        return 0;
#if defined (CONFIG_SPX_FEATURE_GLOBAL_BUILD_MAJOR) && \
	defined (CONFIG_SPX_FEATURE_GLOBAL_BUILD_MINOR) && \
	defined (CONFIG_SPX_FEATURE_GLOBAL_BUILD_NUMBER) && \
	defined (CONFIG_SPX_FEATURE_GLOBAL_BUILD_DATE) && \
	defined (CONFIG_SPX_FEATURE_GLOBAL_BUILD_TIME) && \
	defined (CONFIG_SPX_FEATURE_GLOBAL_BUILD_DESC) && \
	defined (CONFIG_SPX_FEATURE_GLOBAL_PRODUCT_ID)
	
		sprintf (fmhinfo, 	"FW_VERSION=%d.%d.%d\n"
						"FW_DATE=%s\n"
						"FW_BUILDTIME=%s\n"
						"FW_DESC=%s\n"
						"FW_PRODUCTID=%d\n", 
						CONFIG_SPX_FEATURE_GLOBAL_BUILD_MAJOR,
						CONFIG_SPX_FEATURE_GLOBAL_BUILD_MINOR,
						CONFIG_SPX_FEATURE_GLOBAL_BUILD_NUMBER,
						CONFIG_SPX_FEATURE_GLOBAL_BUILD_DATE,
						CONFIG_SPX_FEATURE_GLOBAL_BUILD_TIME,
						CONFIG_SPX_FEATURE_GLOBAL_BUILD_DESC,
						CONFIG_SPX_FEATURE_GLOBAL_PRODUCT_ID);
		fwInfo_size = strlen (buf);
#else
		// TODO: These hardcoded fwinfo will be removed once these information is extracted from .MAP file. 

		sprintf (fmhinfo, 	"FW_VERSION=0.0.0\n"
						"FW_DATE=Jan 01 2010\n"
						"FW_BUILDTIME=12:01:01 IST\n"
						"FW_DESC=WARNING : UNOFFICIAL BUILD!!\n"
						"FW_PRODUCTID=1\n");
		fwInfo_size = strlen (fmhinfo);
#endif
	}

	/* Set End of File if no more data */

	/* Return length of data */
	//remember the fwinfo
#if 0
	g_cached_fwinfo_buf = kmalloc(fwInfo_size,GFP_KERNEL);
	if(g_cached_fwinfo_buf == NULL)
	{
		printk(KERN_ERR "Error: Could not allocate memory structure for holding fwinfo\n");
		//always treat like first time therefore
		firsttime = 1;
	}
	else
	{
		memcpy(g_cached_fwinfo_buf,fmhinfo,fwInfo_size);
	}
#endif
if(fmhinfo==NULL)
      return 0;
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
	copy_size = (fwInfo_size > count) ? count : fwInfo_size;
	if (copy_size > 0)
	{
		if (copy_to_user(buf,fmhinfo, copy_size))
	        return -EFAULT;
	}

	*offset+=copy_size;
#else
	memcpy(buf,fmhinfo,copy_size);
#endif
    
	return copy_size;
}


#ifdef CONFIG_SPX_FEATURE_GLOBAL_DUAL_IMAGE_SUPPORT

/*
   FillDummyFWinfo fills the fwinfo with hardcoded values. 
 */ 
int FillDummyFWinfo(char *buf)
{
	sprintf (buf, 	"FW_VERSION=0.0.0\n"
				"FW_DATE=0\n"
				"FW_BUILDTIME=0\n"
				"FW_DESC=0\n"
				"FW_PRODUCTID=0\n");
	
return 1;
}

/*
   fwinfo1_read is a call back funtion to the fwinfo1 file. 
   For every read to fwinfo1 file, reads the image-1 fw info directly from mtd device.
*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
int
fwinfo1_read(struct file *file,  char __user *buf, size_t count, loff_t *offset)
#else
int
fwinfo1_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
#endif
{

	unsigned long fwInfo_size = 0;
	FLASH_INFO flinfo;
	int copy_size = 0;
	unsigned long   gbl_flash_start=0;
        unsigned long   gbl_used_flash_start=0;
        unsigned long   gbl_used_flash_size=0;
        unsigned long   gbl_erase_block_size=0;
        char gb_erase_block_size[MAX_FLASH_SIZE];
        char gb_flash_start[MAX_FLASH_SIZE];
        char gb_used_flash_size[MAX_FLASH_SIZE];
        char gb_used_flash_start[MAX_FLASH_SIZE];
        memset(gb_flash_start,0,sizeof(gb_flash_start));
        memset(gb_used_flash_size,0,sizeof(gb_used_flash_size));
        memset(gb_used_flash_start,0,sizeof(gb_used_flash_start));
        memset(gb_erase_block_size,0,sizeof(gb_erase_block_size));
        if(GetMacroPropValue("flash_macros","global-flash-start",gb_flash_start) == 0)
        {
                gbl_flash_start = simple_strtoull(gb_flash_start, NULL, MAX_FLASH_SIZE);
        }
        if(GetMacroPropValue("flash_macros","global-used-flash-size",gb_used_flash_size) == 0)
        {
                gbl_used_flash_size = simple_strtoull(gb_used_flash_size, NULL, MAX_FLASH_SIZE);
        }
	if(GetMacroPropValue("flash_macros","global-used-flash-start",gb_used_flash_start) == 0)
        {
                gbl_used_flash_start = simple_strtoull(gb_used_flash_start, NULL, MAX_FLASH_SIZE);
        }
	 if(GetMacroPropValue("flash_macros","global-erase-block-size",gb_erase_block_size) == 0)
        {
                gbl_erase_block_size = simple_strtoull(gb_erase_block_size, NULL, MAX_FLASH_SIZE);
        }

/* We don't support seeked reads. read should start from 0 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
        if (*offset != 0)
                return 0;
#else
        *eof=1;
        if (offset != 0)
                return 0;
#endif

        /* FMH Firmware Info can be upto one erase block. So allocate */
        if (g_cached_fwinfo1_buf == NULL)
        {
        	g_cached_fwinfo1_buf = kmalloc(gbl_erase_block_size,GFP_KERNEL);


        	if (g_cached_fwinfo1_buf == NULL)
        	{
        		printk("Error: Unable to allocate Memory for FMH Info\n");
        		return 0;
        	}
        }

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
	/* This is a requirement. So don't change it */
	*start = buf;

	/* Hack: Assuming all reads are using cat of proc entry and no   *
	 * lseeking random offsets, if offset is not 0, assume it is eof *
	 * If needed fix it later
	 */
	if (offset != 0)
	{
		*eof = 1;
		return 0;
	}
#endif

	/* Find the SectorSize (erase size) of Flash */

	flinfo.read_flash = mtd_read_thunk;

	if (g_MTDSupport == 1)
	{
		/* Scan and Get FwInfo */
		fwInfo_size = ScanFirmwareModule(gbl_used_flash_start - gbl_flash_start,
				gbl_used_flash_size,
				gbl_erase_block_size,
				(unsigned char*)g_cached_fwinfo1_buf,
				&flinfo);
		if ((signed long)fwInfo_size <= 0 )
		{
			printk(KERN_DEBUG "Firmware might be corrupted..\n");
			//FW might be corrupted, filling with dummy values.
			FillDummyFWinfo(g_cached_fwinfo1_buf);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
			*eof = 1;
#endif
			fwInfo_size = strlen (g_cached_fwinfo1_buf); 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
			if (copy_to_user(buf,g_cached_fwinfo1_buf, fwInfo_size))
				return -EFAULT;
			*offset+=fwInfo_size;
#else
			memcpy(buf,g_cached_fwinfo1_buf, fwInfo_size);
#endif
			return fwInfo_size;
		}
	}
	else
	{
		printk(KERN_ALERT "No MTD support\n");
	}
	/* Set End of File if no more data */
	/* Return length of data */
	//remember the fwinfo
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
	copy_size = (fwInfo_size > count) ? count : fwInfo_size;
	if (copy_size > 0)
	{
		if (copy_to_user(buf,g_cached_fwinfo1_buf, copy_size))
			return -EFAULT;
	}

        *offset+=copy_size;
#else
        memcpy(buf,g_cached_fwinfo1_buf, copy_size);
#endif
	return copy_size;
}


/*
   fwinfo2_read is a call back funtion to the fwinfo2 file. 
   For every read to fwinfo2 file, reads the image-2 fw info directly from mtd device.
*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
int
fwinfo2_read(struct file *file,  char __user *buf, size_t count, loff_t *offset)
#else
int
fwinfo2_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
#endif
{
	unsigned long fwInfo_size = 0;
	FLASH_INFO flinfo;
	int copy_size = 0;
	unsigned long   gbl_flash_start=0;
        unsigned long   gbl_used_flash_start=0;
        unsigned long   gbl_used_flash_size=0;
        unsigned long   gbl_erase_block_size=0;
        char gb_erase_block_size[MAX_FLASH_SIZE];
        char gb_flash_start[MAX_FLASH_SIZE];
        char gb_used_flash_size[MAX_FLASH_SIZE];
        char gb_used_flash_start[MAX_FLASH_SIZE];
        memset(gb_flash_start,0,sizeof(gb_flash_start));
        memset(gb_used_flash_size,0,sizeof(gb_used_flash_size));
        memset(gb_used_flash_start,0,sizeof(gb_used_flash_start));
        memset(gb_erase_block_size,0,sizeof(gb_erase_block_size));
        if(GetMacroPropValue("flash_macros","global-flash-start",gb_flash_start) == 0)
        {
                gbl_flash_start = simple_strtoull(gb_flash_start, NULL, MAX_FLASH_SIZE);
        }
	if(GetMacroPropValue("flash_macros","global-used-flash-size",gb_used_flash_size) == 0)
        {
                gbl_used_flash_size = simple_strtoull(gb_used_flash_size, NULL, MAX_FLASH_SIZE);
        }
	if(GetMacroPropValue("flash_macros","global-used-flash-start",gb_used_flash_start) == 0)
        {
                gbl_used_flash_start = simple_strtoull(gb_used_flash_start, NULL, MAX_FLASH_SIZE);
        }
	if(GetMacroPropValue("flash_macros","global-erase-block-size",gb_erase_block_size) == 0)
        {
                gbl_erase_block_size = simple_strtoull(gb_erase_block_size, NULL, MAX_FLASH_SIZE);
        }
/* We don't support seeked reads. read should start from 0 */
#if (LINUX_VERSION_CODE >=  KERNEL_VERSION(2,6,36))
        if (*offset != 0)
                return 0;
#else
        *eof=1;
        if (offset != 0)
                return 0;
#endif
#if (LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,36))
	/* This is a requirement. So don't change it */
	*start = buf;

	/* Hack: Assuming all reads are using cat of proc entry and no   *
	 * lseeking random offsets, if offset is not 0, assume it is eof *
	 * If needed fix it later
	 */
	if (offset != 0)
	{
		*eof = 1;
		return 0;
	}
#endif

	/* FMH Firmware Info can be upto one erase block. So allocate */
	if(g_cached_fwinfo2_buf == NULL)
	{
		g_cached_fwinfo2_buf = kmalloc(gbl_erase_block_size,GFP_KERNEL);

		if (g_cached_fwinfo2_buf == NULL)
		{
			printk("Error: Unable to allocate Memory for FMH Info\n");
			return 0;
		}
	}

	/* Find the SectorSize (erase size) of Flash */

	flinfo.read_flash = mtd_read_thunk;

	if (g_MTDSupport == 1)
	{
		/* Scan and Get FwInfo */
		unsigned long flashstartoffset;
#if defined(CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT) && defined(CONFIG_SPX_FEATURE_DEDICATED_SPI_FLASH_BANK)
		if (broken_spi_banks == 1)
			flashstartoffset = gbl_used_flash_start - gbl_flash_start;
		else
#endif
		{
#if defined (CONFIG_SPX_FEATURE_HW_FAILSAFE_BOOT) && defined(CONFIG_SPX_FEATURE_DEDICATED_SPI_FLASH_BANK)
                    flashstartoffset=FlashSize;
#else
              flashstartoffset = gbl_used_flash_start - gbl_flash_start + gbl_used_flash_size;     
#endif
		}
		
		fwInfo_size = ScanFirmwareModule(flashstartoffset,
				gbl_used_flash_size,
				gbl_erase_block_size,
				(unsigned char*)g_cached_fwinfo2_buf,
				&flinfo);
	
		if ((signed long)fwInfo_size <= 0 )
		{
			printk(KERN_DEBUG "Firmware might be corrupted..\n");
			//FW might be corrupted, filling with dummy values.
			FillDummyFWinfo(g_cached_fwinfo2_buf);
#if (LINUX_VERSION_CODE <  KERNEL_VERSION(2,6,36))
                        *eof = 1;
#endif
                        fwInfo_size = strlen (g_cached_fwinfo2_buf);
#if (LINUX_VERSION_CODE >=  KERNEL_VERSION(2,6,36))
						if (copy_to_user(buf,g_cached_fwinfo2_buf, fwInfo_size))
							return -EFAULT;
                        *offset+=fwInfo_size;
#else
                        memcpy(buf,g_cached_fwinfo2_buf, fwInfo_size);
#endif
			return fwInfo_size;		
		}
	} 
	else
	{
		printk(KERN_ALERT "No MTD support\n");
	}

	/* Set End of File if no more data */
	/* Return length of data */
	//remember the fwinfo
#if (LINUX_VERSION_CODE >=  KERNEL_VERSION(2,6,36))
	copy_size = (fwInfo_size > count) ? count : fwInfo_size;
	if (copy_size > 0)
	{
		if (copy_to_user(buf,g_cached_fwinfo2_buf, copy_size))
			return -EFAULT;
	}
	*offset+=copy_size;
#else
    memcpy(buf,g_cached_fwinfo2_buf, copy_size);
#endif
        return copy_size;
}

#endif

void free_info_cache(void)
{
    if(g_cached_fwinfo_buf != NULL)
        kfree(g_cached_fwinfo_buf);
    if(g_cached_fwinfo1_buf != NULL)
    	kfree(g_cached_fwinfo1_buf);
    if(g_cached_fwinfo2_buf != NULL)
    	kfree(g_cached_fwinfo2_buf);
    if(g_cached_flashbanksize_buf != NULL)
        kfree(g_cached_flashbanksize_buf);
}

/* ----------------Proc File System FwInfo with lock  ---------------------*/
static DEFINE_MUTEX(core_fwinfo_mlock);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,36))
int
fwinfo_read(struct file *file,  char __user *buf, size_t count, loff_t *offset)
{
        int n_return=0;

        mutex_lock(&core_fwinfo_mlock);
        n_return=read_fw_info(file,  buf, count, offset);
        mutex_unlock(&core_fwinfo_mlock);

        return n_return;
}
#else
int
fwinfo_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
        int n_return=0;

        mutex_lock(&core_fwinfo_mlock);
        n_return=read_fw_info(buf, start, offset, count, eof, data);
        mutex_unlock(&core_fwinfo_mlock);

        return n_return;
}
#endif

