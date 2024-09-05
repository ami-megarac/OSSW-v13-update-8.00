#include <fcntl.h>
#include <thread>
#include "infrastructure/server.h"
#include "classes/system.h"
#include "classes/device.h"
#include "classes/csJtag.h"
#include "classes/cpuDebug.h"
#include "classes/gpuDebug.h"
#include "classes/relay.h"
#include "classes/private.h"
#include "classes/lpc.h"
#include "classes/mux.h"
#include "classes/wombatUvdMux.h"
#include "hw/system_hw.h"
#include "hw/header_hw.h"
#include "hw/jtag_hw.h"
#include "hw/csjtag_hw.h"
#include "hw/triggers_hw.h"
#include "hw/hdt_hw.h"
#include "hw/relay_hw.h"
#include "hw/gpuDebug_hw.h"
#include "hw/lpcPostCode_hw.h"
#include "hw/lpcRomEmu_hw.h"
#include "hw/mux_hw.h"

#include "infrastructure/socketHandler.h"

#include <pthread.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

extern "C"
{
	#include "iniparser.h"
	#include "parse-ex.h"
	#include "gpioifc.h"
	#include "featuredef.h"
	#include "EINTR_wrappers.h"
	#include "AmdRemDbgCfg.h"
}

#define JTAG_DRIVER_BASE "/dev/debug_jtag"
#define YAAP_CONFIG      "/conf/YAAPConfig"
#define YAAP_FIFO_REQQ    "/var/YaapFifoReq_Q"
#define YAAP_FIFO_RESQ    "/var/YaapFifoRes_Q"
#define MAX_BUF 40

#define PSP_SOFT_FUSE_NOTIFY   110      // GPION6

extern unsigned int JTAG_get_idcode(int fd);
using namespace yaap;

const std::string hdtSelName = "HDT_SEL";
const std::string hdtDbgReqName = "HDT_DBREQ_L";
const std::string PwrOkName = "MON_POST_COMPLETE";

const std::string powerButtonName = "ASSERT_PWR_BTN_L";
const std::string resetButtonName = "ASSERT_RST_BTN_L";
const std::string warmResetButtonName = "ASSERT_WARM_RST_BTN_L";
const std::string fpgaReservedButtonName = "ASSERT_SOC_RST_BTN_L";
const std::string jtagTRSTName = "JTAG_TRST_N";

bool is2PSystem = false;
int g_iGPIOSoftFuseNotify;

#ifndef CONSUMER
#define CONSUMER "Consumer"
#endif

typedef struct
{
	unsigned int major;
	unsigned int minor;
	unsigned int pointver;
}OS_FIRMWARE;

struct local_gpio_data{
    unsigned char local_pin_number;
}ldata;


static OS_FIRMWARE gOsFirmware;

static int init_config_file(char *File)
{
   static INI_HANDLE ini = NULL;
   if(ini == NULL)
   {
     ini=IniLoadFile(File);
     if(ini == NULL)
       return -1;

   	  gOsFirmware.major=IniGetSInt(ini, (char*)"Firmware", (char*)"majorVer", 0);
   	  gOsFirmware.minor=IniGetSInt(ini, (char*)"Firmware", (char*)"minorVer", 0);
   	  gOsFirmware.pointver=IniGetSInt(ini, (char*)"Firmware", (char*)"pointVer", 0);	

   	  if(gOsFirmware.major==0&&gOsFirmware.minor==0&&gOsFirmware.pointver==0)
   	  {
		  IniSetSInt(ini, (char*)"Firmware", (char*)"majorVer", MAJORVER);
		  IniSetSInt(ini, (char*)"Firmware", (char*)"minorVer", MINORVER);
		  IniSetSInt(ini, (char*)"Firmware", (char*)"pointVer", POINTVER);

		  gOsFirmware.major=IniGetSInt(ini, (char*)"Firmware", (char*)"majorVer", MAJORVER);
   	      gOsFirmware.minor=IniGetSInt(ini, (char*)"Firmware", (char*)"minorVer", MINORVER);
   	      gOsFirmware.pointver=IniGetSInt(ini, (char*)"Firmware", (char*)"pointVer", POINTVER);	
   	  }

   	  printf("nMajor=%x nMinor=%x nPointver=%x\n",gOsFirmware.major,gOsFirmware.minor,gOsFirmware.pointver);

   }

  IniSaveFile(ini,File);
  IniCloseFile(ini);
  ini = NULL;
  return 0;
}

class AMIMainManager
{
	private:
		int jtagfd;
		pthread_t pthread_id;	

	public:
		int bTaskLoop;
		AMIMainManager()
		{
			jtagfd=-1;
			pthread_id=-1;
			bTaskLoop=0;
		}

		~AMIMainManager()
		{
			jtagfd=-1;
			bTaskLoop=0;
		}

		void register_jtagfd(int fd)
		{
			jtagfd=fd;
		}
		
		int get_jtag_fd()
		{
			return jtagfd;
		}

		static void *recv_process(void *arg) 
		{
				AMIMainManager *pAMIMainManager= (AMIMainManager*)arg;	
        		int fd_r = -1,fd_w=-1;
        		int ret = 0;
        		unsigned char RequestCmd;
    			struct timeval timeout;
    			fd_set readfds;

    			prctl(PR_SET_NAME, __FUNCTION__, 0, 0, 0);
        		if (access(YAAP_FIFO_REQQ, F_OK) != 0)
        		{
                		ret = mkfifo(YAAP_FIFO_REQQ, 0777);
                		if (ret < 0) {
                        	printf("Couldn't mkfifo %s with flags 0777: %s\n",YAAP_FIFO_REQQ, strerror(errno));
                			goto FAIL;
						}
        		}
       
        		if (access(YAAP_FIFO_RESQ, F_OK) != 0)
        		{
                		ret = mkfifo(YAAP_FIFO_RESQ, 0777);
                		if (ret < 0) {
                        	printf("Couldn't mkfifo %s with flags 0777: %s\n",YAAP_FIFO_RESQ, strerror(errno));
                			goto FAIL;
						}
        		}
		
				fd_r = open(YAAP_FIFO_REQQ, O_RDWR | O_NONBLOCK);
        		if (fd_r < 0) {
                	printf("Couldn't open %s with flags O_RDONLY: %s\n",YAAP_FIFO_REQQ, strerror(errno));
        			goto FAIL;
				}
       
				fd_w = open(YAAP_FIFO_RESQ, O_RDWR | O_NONBLOCK);
        		if (fd_w < 0) {
                	printf("Couldn't open %s with flags O_RDWR: %s\n",YAAP_FIFO_RESQ, strerror(errno));
               		goto FAIL;
        		}
 
				pAMIMainManager->bTaskLoop=true;	

				while (pAMIMainManager->bTaskLoop)
        		{
        			FD_ZERO(&readfds);
        			FD_SET(fd_r, &readfds);
        			timeout.tv_sec  = 10;
        			timeout.tv_usec = 0;
        			if(select(fd_r+1, &readfds, NULL, NULL, &timeout)>0)
					{
				
						if (FD_ISSET(fd_r, &readfds))
        				{
                			ret = read(fd_r, &RequestCmd, sizeof(RequestCmd));
							if(ret>0)
							{
								if(RequestCmd==0)//Get IDCODE Command
								{
									unsigned int idcode=0;
									int ret=0;
									idcode=JTAG_get_idcode(pAMIMainManager->get_jtag_fd());
									ret=write(fd_w, &idcode, sizeof(idcode));//Response IDCODE to client.
									if(ret!=sizeof(idcode))
									{
										printf("From Yaap server write idcode:%08x to %s failed\n",idcode,YAAP_FIFO_RESQ);
									}	
								}		
							}
						}		
					}
				}
	
			FAIL:
				if(fd_w>0) close(fd_w);
				if(fd_r>0) close(fd_r);	
				if (access(YAAP_FIFO_REQQ, F_OK) == 0)
				{
					unlink(YAAP_FIFO_REQQ);
				}
				
				if (access(YAAP_FIFO_RESQ, F_OK) == 0)
				{
					unlink(YAAP_FIFO_RESQ);
				}
			
				pthread_exit(NULL);
		}

		int start_ipc_task()
		{
			int ret=0;
			
			if(bTaskLoop)
			{
				printf("IPC task already running...\n");
			}

			ret=pthread_create(&pthread_id, NULL, recv_process, (void*) this);
			if(ret!=0)
			{
				printf("Create start_ipc_task failed\n");
				return -1;
			}
			
			pthread_detach(pthread_id);	

			return 0;
		}
};

int start_yaap_server()
{
    uint32_t default_tck_freq; // 5 MHz
    uint32_t preTck = 5;
    uint32_t postTck = 5;
    bool jtag_sw_mode = false;
	AMIMainManager gAMIMainManager;
	int iJtagDeviceNumber;
	int iPortNumber;
	char sJtagDeviceName[64];
	
	iJtagDeviceNumber = GetMacrodefine_getint("CONFIG_SPX_FEATURE_AMDRemotedbg_JTAG_DEVICE_ID", 1);
	default_tck_freq = GetMacrodefine_getint("CONFIG_SPX_FEATURE_AMDRemotedbg_JTAG_FREQ", 5000000);
	iPortNumber = GetMacrodefine_getint("CONFIG_SPX_FEATURE_AMDRemotedbg_PORT_NUMBER", 6123);
	
	if( (iJtagDeviceNumber < 0) || (iJtagDeviceNumber > 1) )
	{
		iJtagDeviceNumber = 1;
	}
	printf( "Yaap: Used Jtag Device Number: %d\n Default tck Freq: %d, Use Port num: %d\n", iJtagDeviceNumber, default_tck_freq, iPortNumber );
	
	snprintf( sJtagDeviceName, sizeof(sJtagDeviceName), "%s%d", JTAG_DRIVER_BASE, iJtagDeviceNumber );
	
	HDT_JTAG_TRST(0);
	int driver = open (sJtagDeviceName, O_RDWR);    
	if (driver < 0)
	{  
	    printf("open fail <0\n");
	    return -1;
	}

	gAMIMainManager.register_jtagfd(driver);
   	init_config_file((char*)YAAP_CONFIG);
   	
    bmc::SystemHw systemHw(driver);

	// Instantiate YAAP classes.  Note that HDT expects specific names for the classes it interacts with.
	classes::system yaap_system("system", &systemHw);
	classes::device yaap_device("device", &systemHw);
    classes::yaapPrivate yaap_private("private");

    bmc::JtagHw cpuDebugJtagHw(driver, default_tck_freq, jtag_sw_mode);
    bmc::HeaderHw cpuDebugHeaderHw(driver);
    bmc::TriggersHw triggersHw;
    bmc::HdtHw hdtHw(driver);


    bmc::csJtagHw cs1JtagHw(driver);
    bmc::csJtagHw cs2JtagHw(driver);
    bmc::HeaderHw cs1HeaderHw(driver);
    bmc::HeaderHw cs2HeaderHw(driver);


    bmc::RelayHw relay0Hw(driver,0);
    bmc::RelayHw relay1Hw(driver,1);
    bmc::RelayHw relay2Hw(driver,2);
    bmc::RelayHw relay3Hw(driver,3);

    bmc::LpcPostCodeHw lpcPostCodeHw;
    bmc::LpcRomEmulationHw lpcRomEmuHw;

    bmc::MuxHw mux0Hw;
    classes::cpuDebug yaap_cpuDebug("cpuDebug", &cpuDebugJtagHw, &cpuDebugHeaderHw, &triggersHw, &hdtHw, preTck, postTck );
	classes::csJtag yaap_csJtag1("csJtag1", &cs1JtagHw, &cs1HeaderHw);
	classes::csJtag yaap_csJtag2("csJtag2", &cs2JtagHw, &cs2HeaderHw);
	classes::relay yaap_relay0("relay0", &relay0Hw);
	classes::relay yaap_relay1("relay1", &relay1Hw);
	classes::relay yaap_relay2("relay2", &relay2Hw);
	classes::relay yaap_relay3("relay3", &relay3Hw);
	classes::lpc yaap_lpc("lpc", &lpcPostCodeHw, &lpcRomEmuHw);
	classes::wombatUvdMux uvdMux("wombatUvdMux", &mux0Hw);

	gAMIMainManager.start_ipc_task();
	
	printf( "YAAP Server start at: %d\n", iPortNumber );
	
	return startServer(iPortNumber,DEBUG_LEVEL);
}

int main(int argc, char *argv[])
{

   	return start_yaap_server();

}
