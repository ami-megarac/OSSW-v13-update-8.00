/*
// Copyright (c) 2019 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <peci.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include "dimm.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcpp"
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#include <linux/peci-ioctl.h>
#pragma GCC diagnostic pop
// To support distinct handles for multiple peci_Open() calls - limited to 64
#define PECI_I3C_HANDLE_CNT 64
#define PECI_I3C_HANDLE_MAX INT_MAX
#define PECI_I3C_HANDLE_MIN (PECI_I3C_HANDLE_MAX - PECI_I3C_HANDLE_CNT + 1)

int* peci_i3c_fds[PECI_I3C_HANDLE_CNT];

#define PID_INSTANCE_ID_SHIFT 12
#define PID_INSTANCE_ID_MASK 0xf
#define I3C_PID_TO_CPU(x)                                                      \
    (((x) >> PID_INSTANCE_ID_SHIFT) & PID_INSTANCE_ID_MASK)

EPECIStatus peci_GetDIB_seq(uint8_t target, uint64_t* dib, int peci_fd);

char* peci_device_list[2];
#define DEV_NAME_SIZE 64
/*-------------------------------------------------------------------------
 * This funcion sets the name of the PECI device file to use.
 * If the PECI device name is null try "/dev/peci-default",
 * if "/dev/peci-default" does not exist, fall back to "/dev/peci-0"
 *------------------------------------------------------------------------*/
void peci_SetDevName(char* peci_dev)
{
    static char peci_name_new[DEV_NAME_SIZE] = {0};

    if (peci_dev)
    {
        strncpy(peci_name_new, peci_dev, sizeof(peci_name_new));
        peci_name_new[DEV_NAME_SIZE - 1] = '\0';
        peci_device_list[0] = peci_name_new;
        peci_device_list[1] = NULL;
        syslog(LOG_INFO, "PECI set dev name to %s\n", peci_device_list[0]);
    }
    else
    {
        peci_device_list[0] = "/dev/peci-default";
        peci_device_list[1] = "/dev/peci-0";
        syslog(LOG_INFO, "PECI set dev names to %s, %s\n", peci_device_list[0],
               peci_device_list[1]);
    }
}

/*-------------------------------------------------------------------------
 * This function initializes PECI device name when a shared library
 * is loaded, typically during program startup.
 *------------------------------------------------------------------------*/
static void init() __attribute__((constructor));
static void init()
{
    // By default PECI_DEV is not defined in the environment,
    // so this will call peci_SetDevName(NULL) and initialize
    // PECI device name to defaults.
    peci_SetDevName(getenv("PECI_DEV"));
}

/*-------------------------------------------------------------------------
 * This function unlocks the peci interface
 *------------------------------------------------------------------------*/
void peci_Unlock(int peci_fd)
{
    /* Reason for False Positive - the variable peci_fd <= PECI_I3C_HANDLE_MAX, when break it, this possible for peci_fd >= PECI_I3C_HANDLE_MAX */
    /* coverity[result_independent_of_operands : FALSE] */
    if (peci_fd >= PECI_I3C_HANDLE_MIN && peci_fd <= PECI_I3C_HANDLE_MAX)
    {
        int fd_index = PECI_I3C_HANDLE_MAX - peci_fd;

        for (int i = 0; i < MAX_CPUS; i++)
        {
            int fd = peci_i3c_fds[fd_index][i];

            if (fd < 0)
            {
                continue;
            }

            if (close(fd) != 0)
            {
                syslog(LOG_ERR, "PECI device failed to unlock.\n");
            }
        }
        free(peci_i3c_fds[fd_index]);
        peci_i3c_fds[fd_index] = NULL;
    }
    else
    {
        if (close(peci_fd) != 0)
        {
            syslog(LOG_ERR, "PECI device failed to unlock.\n");
        }
    }
}

static int peci_chardev_filter(const struct dirent* d)
{
    if (strncmp(d->d_name, "peci-", 5) == 0)
    {
        return 1;
    }

    return 0;
}

int peci_i3c_chardev_to_cpu(const char* name)
{
    struct stat st;
    char path[38];
    char* lpath;
    size_t lpath_size = PATH_MAX;
    char* subpath;
    int dirfd;
    int chardev_dirfd;
    int pidfd;
    ssize_t ret;
    long long int pid;
    char pid_buf[20]; // %llx + \0

    dirfd = open("/dev", O_DIRECTORY);
    if (dirfd < 0)
    {
        return -errno;
    }

    ret = fstatat(dirfd, name, &st, 0);
    if (ret)
    {
        goto err;
    }

    snprintf(path, sizeof(path), "/sys/dev/char/%d:%d", major(st.st_rdev),
             minor(st.st_rdev));

    lpath = malloc(lpath_size);
    if (!lpath)
    {
        ret = -ENOMEM;
        goto err;
    }

    ret = readlink(path, lpath, lpath_size);
    if (ret < 0)
    {
        goto err_free;
    }

    /* Reason for false positive - string buf is always null terminated */
    /* coverity[string_null : FALSE] */
    subpath = strstr(lpath, "peci-i3c");
    if (!subpath)
    {
        ret = -EINVAL;
        goto err_free;
    }
    strncpy(subpath, "pid", lpath_size - (size_t)(subpath - lpath));

    chardev_dirfd = open("/sys/dev/char", O_DIRECTORY);
    if (chardev_dirfd < 0)
    {
        ret = -errno;
        goto err_free;
    }

    pidfd = openat(chardev_dirfd, lpath, O_RDONLY);
    if (pidfd < 0)
    {
        ret = -errno;
        goto err_close_chardev;
    }
    ret = read(pidfd, pid_buf, sizeof(pid_buf));
    if (ret < 0)
    {
        goto err_close_pidfd;
    }

    pid = strtoll(pid_buf, NULL, 16);

    ret = I3C_PID_TO_CPU(pid);

err_close_pidfd:
    close(pidfd);
err_close_chardev:
    close(chardev_dirfd);
err_free:
    free(lpath);
err:
    close(dirfd);

    return ret;
}

static int peci_i3c_open(char* name, int idx)
{
    int dirfd;
    int cpu;

    dirfd = open("/dev", O_DIRECTORY);
    if (dirfd < 0)
    {
        return -errno;
    }

    cpu = peci_i3c_chardev_to_cpu(name);
    if (cpu < 0)
    {
        close(dirfd);
        return cpu;
    }

    if (peci_i3c_fds[idx][cpu] == -1)
    {
        peci_i3c_fds[idx][cpu] = openat(dirfd, name, O_RDWR | O_CLOEXEC);
    }

    close(dirfd);

    return 0;
}

static int peci_device_i3c_open(void)
{
    struct dirent** namelist;
    int if_any = 0;
    int idx = -1;
    int n;

    for (int i = 0; i < PECI_I3C_HANDLE_CNT; i++)
    {
        if (!peci_i3c_fds[i])
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        return -1;
    }

    peci_i3c_fds[idx] = malloc(MAX_CPUS * sizeof(int));
    if (!peci_i3c_fds[idx])
    {
        return -1;
    }

    memset(peci_i3c_fds[idx], -1, MAX_CPUS * sizeof(int));

    n = scandir("/dev", &namelist, &peci_chardev_filter, alphasort);
    if (n == -1)
    {
        goto err_free;
    }

    while (n--)
    {
        if (peci_i3c_open(namelist[n]->d_name, idx) == 0)
        {
            if_any = 1;
        }
        free(namelist[n]);
    }
    free(namelist);

    if (!if_any)
    {
        goto err_free;
    }

    return PECI_I3C_HANDLE_MAX - idx;

err_free:
    free(peci_i3c_fds[idx]);
    peci_i3c_fds[idx] = NULL;
    return -1;
}

static int peci_device_open(char* peci_device)
{
    if (strncmp(peci_device, "/dev/peci-i3c", DEV_NAME_SIZE) == 0)
    {
        return peci_device_i3c_open();
    }
    return open(peci_device, O_RDWR | O_CLOEXEC);
}

/*-------------------------------------------------------------------------
 * This function attempts to lock the peci interface with the specified
 * timeout and returns a file descriptor if successful.
 *------------------------------------------------------------------------*/
EPECIStatus peci_Lock(int* peci_fd, int timeout_ms)
{
    struct timespec sRequest = {0};
    sRequest.tv_sec = 0;
    sRequest.tv_nsec = PECI_TIMEOUT_RESOLUTION_MS * 1000 * 1000;
    int timeout_count = 0;
    char* peci_device = peci_device_list[0];

    if (NULL == peci_fd)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Open the PECI driver with the specified timeout
    *peci_fd = peci_device_open(peci_device);
    if (*peci_fd == -1 && errno == ENOENT && peci_device_list[1])
    {
        peci_device = peci_device_list[1];
        *peci_fd = peci_device_open(peci_device);
    }
    switch (timeout_ms)
    {
        case PECI_NO_WAIT:
            break;
        case PECI_WAIT_FOREVER:
            while (-1 == *peci_fd)
            {
                nanosleep(&sRequest, NULL);
                *peci_fd = peci_device_open(peci_device);
            }
            break;
        default:
            while (-1 == *peci_fd && timeout_count < timeout_ms)
            {
                nanosleep(&sRequest, NULL);
                timeout_count += PECI_TIMEOUT_RESOLUTION_MS;
                *peci_fd = peci_device_open(peci_device);
            }
    }

    if (-1 == *peci_fd)
    {
        syslog(LOG_ERR, " >>> PECI Device Busy <<< \n");
        return PECI_CC_DRIVER_ERR;
    }

    return PECI_CC_SUCCESS;
}

/*-------------------------------------------------------------------------
 * This function closes the peci interface
 *------------------------------------------------------------------------*/
static void peci_Close(int peci_fd)
{
    peci_Unlock(peci_fd);
}

/*-------------------------------------------------------------------------
 * This function opens the peci interface and returns a file descriptor
 *------------------------------------------------------------------------*/
static EPECIStatus peci_Open(int* peci_fd)
{
    if (NULL == peci_fd)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Lock the PECI driver with a default timeout
    return peci_Lock(peci_fd, PECI_TIMEOUT_MS);
}

/*-------------------------------------------------------------------------
 * This function issues peci commands to peci driver
 *------------------------------------------------------------------------*/
static EPECIStatus HW_peci_issue_cmd(unsigned int cmd, char* cmdPtr,
                                     int peci_fd)
{
    if (cmdPtr == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    int cpu = cmdPtr[0] - MIN_CLIENT_ADDR;
    int fd;

    /* Reason for False Positive - the variable peci_fd <= PECI_I3C_HANDLE_MAX, when break it, this possible for peci_fd >= PECI_I3C_HANDLE_MAX */
    /* coverity[result_independent_of_operands : FALSE] */
    if (peci_fd >= PECI_I3C_HANDLE_MIN && peci_fd <= PECI_I3C_HANDLE_MAX)
    {
        fd = peci_i3c_fds[PECI_I3C_HANDLE_MAX - peci_fd][cpu];
    }
    else
    {
        fd = peci_fd;
    }

    if (ioctl(fd, cmd, cmdPtr) != 0)
    {
        if (errno == ETIMEDOUT)
        {
            return PECI_CC_TIMEOUT;
        }
        return PECI_CC_DRIVER_ERR;
    }

    return PECI_CC_SUCCESS;
}

/*-------------------------------------------------------------------------
 * Find the specified PCI bus number value
 *------------------------------------------------------------------------*/
EPECIStatus FindBusNumber(uint8_t u8Bus, uint8_t u8Cpu, uint8_t* pu8BusValue)
{
    uint8_t u8CpuBus0[] = {
        PECI_PCI_BUS0_CPU0,
        PECI_PCI_BUS0_CPU1,
    };
    uint8_t u8Bus0 = 0;
    uint8_t u8Offset = 0;
    EPECIStatus ret = PECI_CC_SUCCESS;
    uint8_t u8Reg[4] = {0};
    uint8_t cc = 0;

    // First check for valid inputs
    // Check cpu and bus numbers, only support buses [5:0]
    if ((u8Bus > 5) || (u8Cpu >= (sizeof(u8CpuBus0) / sizeof(uint8_t))) ||
        (pu8BusValue == NULL))
    {
        return PECI_CC_INVALID_REQ;
    }

    // Get the Bus 0 value for the requested CPU
    u8Bus0 = u8CpuBus0[u8Cpu];

    // Next check that the bus numbers are valid
    // CPUBUSNO_VALID register - Above registers valid? - B(0) D5 F0 offset
    // D4h
    ret = peci_RdPCIConfig(u8Cpu, u8Bus0, PECI_PCI_CPUBUSNO_DEV,
                           PECI_PCI_CPUBUSNO_FUNC, PECI_PCI_CPUBUSNO_VALID,
                           u8Reg, &cc);
    if (ret != PECI_CC_SUCCESS)
    {
        return ret;
    }
    // BIOS will set bit 31 of CPUBUSNO_VALID when the bus numbers are valid
    if ((u8Reg[3] & 0x80) == 0)
    {
        return PECI_CC_HW_ERR;
    }

    // Bus numbers are valid so read the correct offset for the requested
    // bus CPUBUSNO register - CPU Internal Bus Numbers [3:0] - B(0) D5 F0
    // offset CCh CPUBUSNO_1 register - CPU Internal Bus Numbers [5:4] -
    // B(0) D5 F0 offset D0h
    u8Offset = u8Bus <= 3 ? PECI_PCI_CPUBUSNO : PECI_PCI_CPUBUSNO_1;
    ret = peci_RdPCIConfig(u8Cpu, u8Bus0, PECI_PCI_CPUBUSNO_DEV,
                           PECI_PCI_CPUBUSNO_FUNC, u8Offset, u8Reg, &cc);
    if (ret != PECI_CC_SUCCESS)
    {
        return ret;
    }

    // Now return the bus value for the requested bus
    *pu8BusValue = u8Reg[u8Bus % 4];

    // Unused bus numbers are set to zero which is only valid for bus 0
    // so, return an error for any other bus set to zero
    if (*pu8BusValue == 0 && u8Bus != 0)
    {
        return PECI_CC_CPU_NOT_PRESENT;
    }

    return PECI_CC_SUCCESS;
}

/*-------------------------------------------------------------------------
 * This function checks the CPU PECI interface
 *------------------------------------------------------------------------*/
EPECIStatus peci_Ping(uint8_t target)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_Ping_seq(target, peci_fd);

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential Ping with the provided
 * peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_Ping_seq(uint8_t target, int peci_fd)
{
    EPECIStatus ret = PECI_CC_SUCCESS;
    struct peci_ping_msg cmd = {0};

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    ret = HW_peci_issue_cmd(PECI_IOC_PING, (char*)&cmd, peci_fd);

    return ret;
}

/*-------------------------------------------------------------------------
 * This function gets PECI device information
 *------------------------------------------------------------------------*/
EPECIStatus peci_GetDIB(uint8_t target, uint64_t* dib)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (dib == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_GetDIB_seq(target, dib, peci_fd);

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential GetDIB with the provided
 * peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_GetDIB_seq(uint8_t target, uint64_t* dib, int peci_fd)
{
    struct peci_get_dib_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;
    cmd.addr = target;

    if (dib == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    ret = HW_peci_issue_cmd(PECI_IOC_GET_DIB, (char*)&cmd, peci_fd);

    if (ret == PECI_CC_SUCCESS)
    {
        *dib = cmd.dib;
    }

    return ret;
}

/*-------------------------------------------------------------------------
 * This function get PECI Thermal temperature
 * Expressed in signed fixed point value of 1/64 degrees celsius
 *------------------------------------------------------------------------*/
EPECIStatus peci_GetTemp(uint8_t target, int16_t* temperature)
{
    int peci_fd = -1;
    struct peci_get_temp_msg cmd = {0};

    if (temperature == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;

    EPECIStatus ret = HW_peci_issue_cmd(PECI_IOC_GET_TEMP, (char*)&cmd,
                                        peci_fd);

    if (ret == PECI_CC_SUCCESS)
    {
        *temperature = cmd.temp_raw;
    }

    peci_Close(peci_fd);

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides read access to the package configuration
 * space within the processor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPkgConfig(uint8_t target, uint8_t u8Index, uint16_t u16Value,
                             uint8_t u8ReadLen, uint8_t* pPkgConfig,
                             uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdPkgConfig_dom(target, 0, u8Index, u16Value, u8ReadLen,
                                pPkgConfig, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to the package configuration
 * space within the processor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPkgConfig_dom(uint8_t target, uint8_t domainId,
                                 uint8_t u8Index, uint16_t u16Value,
                                 uint8_t u8ReadLen, uint8_t* pPkgConfig,
                                 uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPkgConfig == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_RdPkgConfig_seq_dom(target, domainId, u8Index, u16Value,
                                   u8ReadLen, pPkgConfig, peci_fd, cc);

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdPkgConfig with the provided
 * peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPkgConfig_seq(uint8_t target, uint8_t u8Index,
                                 uint16_t u16Value, uint8_t u8ReadLen,
                                 uint8_t* pPkgConfig, int peci_fd, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdPkgConfig_seq_dom(target, 0, u8Index, u16Value, u8ReadLen,
                                    pPkgConfig, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdPkgConfig with the provided
 * peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPkgConfig_seq_dom(uint8_t target, uint8_t domainId,
                                     uint8_t u8Index, uint16_t u16Value,
                                     uint8_t u8ReadLen, uint8_t* pPkgConfig,
                                     int peci_fd, uint8_t* cc)
{
    struct peci_rd_pkg_cfg_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPkgConfig == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Support PECI 4.0 read lengths
    if (u8ReadLen != 1 && u8ReadLen != 2 && u8ReadLen != 4 && u8ReadLen != 8)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.pkg_config) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.index = u8Index;  // RdPkgConfig index
    cmd.param = u16Value; // Config parameter value
    cmd.rx_len = u8ReadLen;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_RD_PKG_CFG, (char*)&cmd, peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pPkgConfig, cmd.pkg_config, u8ReadLen);
    }

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides write access to the package configuration
 * space within the processor
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrPkgConfig(uint8_t target, uint8_t u8Index, uint16_t u16Param,
                             const void* pPkgData, uint8_t u8WriteLen,
			     uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrPkgConfig_dom(target, 0, u8Index, u16Param, pPkgData,
                                u8WriteLen, cc);
}

/*-------------------------------------------------------------------------
 * This function provides write access to the package configuration
 * space within the processor in the specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrPkgConfig_dom(uint8_t target, uint8_t domainId,
                                 uint8_t u8Index, uint16_t u16Param,
                                 const void* pPkgData, uint8_t u8WriteLen,
                                 uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (cc == NULL || pPkgData == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_WrPkgConfig_seq_dom(target, domainId, u8Index, u16Param,
                                   pPkgData, u8WriteLen, peci_fd, cc);

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential WrPkgConfig with the provided
 * peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrPkgConfig_seq(uint8_t target, uint8_t u8Index,
                                 uint16_t u16Param, const void* pPkgData,
                                 uint8_t u8WriteLen, int peci_fd, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrPkgConfig_seq_dom(target, 0, u8Index, u16Param, pPkgData,
                                    u8WriteLen, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential WrPkgConfig with the provided
 * peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrPkgConfig_seq_dom(uint8_t target, uint8_t domainId,
                                     uint8_t u8Index, uint16_t u16Param,
                                     const void* pPkgData, uint8_t u8WriteLen,
                                     int peci_fd, uint8_t* cc)
{
    struct peci_wr_pkg_cfg_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (cc == NULL || pPkgData == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the write length must be a byte, word, or dword
    if ((u8WriteLen != 1) && (u8WriteLen != 2) && (u8WriteLen != 4) &&
		    (u8WriteLen != 8))
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.index = u8Index;  // RdPkgConfig index
    cmd.param = u16Param; // parameter value
    cmd.tx_len = u8WriteLen;
    memcpy(cmd.pkg_data, pPkgData, u8WriteLen);
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_WR_PKG_CFG, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides read access to Model Specific Registers
 * defined in the processor doc.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdIAMSR(uint8_t target, uint8_t threadID, uint16_t MSRAddress,
                         uint64_t* u64MsrVal, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdIAMSR_dom(target, 0, threadID, MSRAddress, u64MsrVal, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to Model Specific Registers
 * defined in the processor doc in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdIAMSR_dom(uint8_t target, uint8_t domainId, uint8_t threadID,
                             uint16_t MSRAddress, uint64_t* u64MsrVal,
                             uint8_t* cc)
{
    int peci_fd = -1;
    struct peci_rd_ia_msr_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (u64MsrVal == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.thread_id = threadID; // request byte for thread ID
    cmd.address = MSRAddress; // MSR Address
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_RD_IA_MSR, (char*)&cmd, peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        *u64MsrVal = cmd.value;
    }

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides read access to the PCI configuration space at
 * the requested PCI configuration address.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfig(uint8_t target, uint8_t u8Bus, uint8_t u8Device,
                             uint8_t u8Fcn, uint16_t u16Reg, uint8_t* pPCIData,
                             uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdPCIConfig_dom(target, 0, u8Bus, u8Device, u8Fcn, u16Reg,
                                pPCIData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to the PCI configuration space at
 * the requested PCI configuration address in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfig_dom(uint8_t target, uint8_t domainId,
                                 uint8_t u8Bus, uint8_t u8Device, uint8_t u8Fcn,
                                 uint16_t u16Reg, uint8_t* pPCIData,
                                 uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPCIData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_RdPCIConfig_seq_dom(target, domainId, u8Bus, u8Device, u8Fcn,
                                   u16Reg, pPCIData, peci_fd, cc);

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdPCIConfig with the provided
 * peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfig_seq(uint8_t target, uint8_t u8Bus,
                                 uint8_t u8Device, uint8_t u8Fcn,
                                 uint16_t u16Reg, uint8_t* pPCIData,
                                 int peci_fd, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdPCIConfig_seq_dom(target, 0, u8Bus, u8Device, u8Fcn, u16Reg,
                                    pPCIData, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdPCIConfig with the provided
 * peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfig_seq_dom(uint8_t target, uint8_t domainId,
                                     uint8_t u8Bus, uint8_t u8Device,
                                     uint8_t u8Fcn, uint16_t u16Reg,
                                     uint8_t* pPCIData, int peci_fd,
                                     uint8_t* cc)
{
    struct peci_rd_pci_cfg_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPCIData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the PCI data
    if (sizeof(cmd.pci_config) < 4)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.bus = u8Bus;
    cmd.device = u8Device;
    cmd.function = u8Fcn;
    cmd.reg = u16Reg;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_RD_PCI_CFG, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pPCIData, cmd.pci_config, 4);
    }

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides read access to the local PCI configuration space
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfigLocal(uint8_t target, uint8_t u8Bus,
                                  uint8_t u8Device, uint8_t u8Fcn,
                                  uint16_t u16Reg, uint8_t u8ReadLen,
                                  uint8_t* pPCIReg, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdPCIConfigLocal_dom(target, 0, u8Bus, u8Device, u8Fcn, u16Reg,
                                     u8ReadLen, pPCIReg, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to the local PCI configuration space in
 * the specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfigLocal_dom(uint8_t target, uint8_t domainId,
                                      uint8_t u8Bus, uint8_t u8Device,
                                      uint8_t u8Fcn, uint16_t u16Reg,
                                      uint8_t u8ReadLen, uint8_t* pPCIReg,
                                      uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPCIReg == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_RdPCIConfigLocal_seq_dom(target, domainId, u8Bus, u8Device,
                                        u8Fcn, u16Reg, u8ReadLen, pPCIReg,
                                        peci_fd, cc);

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdPCIConfigLocal with the provided
 * peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfigLocal_seq(uint8_t target, uint8_t u8Bus,
                                      uint8_t u8Device, uint8_t u8Fcn,
                                      uint16_t u16Reg, uint8_t u8ReadLen,
                                      uint8_t* pPCIReg, int peci_fd,
                                      uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdPCIConfigLocal_seq_dom(target, 0, u8Bus, u8Device, u8Fcn,
                                         u16Reg, u8ReadLen, pPCIReg, peci_fd,
                                         cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdPCIConfigLocal with the provided
 * peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdPCIConfigLocal_seq_dom(uint8_t target, uint8_t domainId,
                                          uint8_t u8Bus, uint8_t u8Device,
                                          uint8_t u8Fcn, uint16_t u16Reg,
                                          uint8_t u8ReadLen, uint8_t* pPCIReg,
                                          int peci_fd, uint8_t* cc)
{
    struct peci_rd_pci_cfg_local_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPCIReg == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the read length must be a byte, word, or dword
    if (u8ReadLen != 1 && u8ReadLen != 2 && u8ReadLen != 4)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.pci_config) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.bus = u8Bus;
    cmd.device = u8Device;
    cmd.function = u8Fcn;
    cmd.reg = u16Reg;
    cmd.rx_len = u8ReadLen;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_RD_PCI_CFG_LOCAL, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pPCIReg, cmd.pci_config, u8ReadLen);
    }

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides write access to the local PCI configuration space
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrPCIConfigLocal(uint8_t target, uint8_t u8Bus,
                                  uint8_t u8Device, uint8_t u8Fcn,
                                  uint16_t u16Reg, uint8_t DataLen,
                                  uint32_t DataVal, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrPCIConfigLocal_dom(target, 0, u8Bus, u8Device, u8Fcn, u16Reg,
                                     DataLen, DataVal, cc);
}

/*-------------------------------------------------------------------------
 * This function provides write access to the local PCI configuration space in
 * the specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrPCIConfigLocal_dom(uint8_t target, uint8_t domainId,
                                      uint8_t u8Bus, uint8_t u8Device,
                                      uint8_t u8Fcn, uint16_t u16Reg,
                                      uint8_t DataLen, uint32_t DataVal,
                                      uint8_t* cc)
{
    int peci_fd = -1;
    struct peci_wr_pci_cfg_local_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    // Per the PECI spec, the write length must be a byte, word, or dword
    if (DataLen != 1 && DataLen != 2 && DataLen != 4)
    {
        peci_Close(peci_fd);
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.bus = u8Bus;
    cmd.device = u8Device;
    cmd.function = u8Fcn;
    cmd.reg = u16Reg;
    cmd.tx_len = DataLen;
    cmd.value = DataVal;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_WR_PCI_CFG_LOCAL, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This internal function is the common interface for RdEndPointConfig to PCI in
 * the specified domain
 *------------------------------------------------------------------------*/
static EPECIStatus peci_RdEndPointConfigPciCommon_dom(
    uint8_t target, uint8_t domainId, uint8_t u8MsgType, uint8_t u8Seg,
    uint8_t u8Bus, uint8_t u8Device, uint8_t u8Fcn, uint16_t u16Reg,
    uint8_t u8ReadLen, uint8_t* pPCIData, int peci_fd, uint8_t* cc)
{
    struct peci_rd_end_pt_cfg_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPCIData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.msg_type = u8MsgType;
    cmd.params.pci_cfg.seg = u8Seg;
    cmd.params.pci_cfg.bus = u8Bus;
    cmd.params.pci_cfg.device = u8Device;
    cmd.params.pci_cfg.function = u8Fcn;
    cmd.params.pci_cfg.reg = u16Reg;
    cmd.rx_len = u8ReadLen;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_RD_END_PT_CFG, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pPCIData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides read access to the PCI configuration space at
 * the requested PCI configuration address.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPci(uint8_t target, uint8_t u8Seg,
                                     uint8_t u8Bus, uint8_t u8Device,
                                     uint8_t u8Fcn, uint16_t u16Reg,
                                     uint8_t u8ReadLen, uint8_t* pPCIData,
                                     uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdEndPointConfigPci_dom(target, 0, u8Seg, u8Bus, u8Device,
                                        u8Fcn, u16Reg, u8ReadLen, pPCIData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to the PCI configuration space at
 * the requested PCI configuration address in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPci_dom(uint8_t target, uint8_t domainId,
                                         uint8_t u8Seg, uint8_t u8Bus,
                                         uint8_t u8Device, uint8_t u8Fcn,
                                         uint16_t u16Reg, uint8_t u8ReadLen,
                                         uint8_t* pPCIData, uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPCIData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_RdEndPointConfigPci_seq_dom(target, domainId, u8Seg, u8Bus,
                                           u8Device, u8Fcn, u16Reg, u8ReadLen,
                                           pPCIData, peci_fd, cc);
    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdEndPointConfig to PCI with the provided
 * peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPci_seq(uint8_t target, uint8_t u8Seg,
                                         uint8_t u8Bus, uint8_t u8Device,
                                         uint8_t u8Fcn, uint16_t u16Reg,
                                         uint8_t u8ReadLen, uint8_t* pPCIData,
                                         int peci_fd, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdEndPointConfigPci_seq_dom(target, 0, u8Seg, u8Bus, u8Device,
                                            u8Fcn, u16Reg, u8ReadLen, pPCIData,
                                            peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdEndPointConfig to PCI with the provided
 * peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPci_seq_dom(uint8_t target, uint8_t domainId,
                                             uint8_t u8Seg, uint8_t u8Bus,
                                             uint8_t u8Device, uint8_t u8Fcn,
                                             uint16_t u16Reg, uint8_t u8ReadLen,
                                             uint8_t* pPCIData, int peci_fd,
                                             uint8_t* cc)
{
    if (pPCIData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the read length must be a byte, word, or dword
    if (u8ReadLen != 1 && u8ReadLen != 2 && u8ReadLen != 4)
    {
        return PECI_CC_INVALID_REQ;
    }

    return peci_RdEndPointConfigPciCommon_dom(
        target, domainId, PECI_ENDPTCFG_TYPE_PCI, u8Seg, u8Bus, u8Device, u8Fcn,
        u16Reg, u8ReadLen, pPCIData, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to the Local PCI configuration space at
 * the requested PCI configuration address.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPciLocal(uint8_t target, uint8_t u8Seg,
                                          uint8_t u8Bus, uint8_t u8Device,
                                          uint8_t u8Fcn, uint16_t u16Reg,
                                          uint8_t u8ReadLen, uint8_t* pPCIData,
                                          uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdEndPointConfigPciLocal_dom(target, 0, u8Seg, u8Bus, u8Device,
                                             u8Fcn, u16Reg, u8ReadLen, pPCIData,
                                             cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to the Local PCI configuration space at
 * the requested PCI configuration address in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPciLocal_dom(uint8_t target, uint8_t domainId,
                                              uint8_t u8Seg, uint8_t u8Bus,
                                              uint8_t u8Device, uint8_t u8Fcn,
                                              uint16_t u16Reg,
                                              uint8_t u8ReadLen,
                                              uint8_t* pPCIData, uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pPCIData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_RdEndPointConfigPciLocal_seq_dom(
        target, domainId, u8Seg, u8Bus, u8Device, u8Fcn, u16Reg, u8ReadLen,
        pPCIData, peci_fd, cc);
    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdEndPointConfig to PCI Local with the
 * provided peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPciLocal_seq(uint8_t target, uint8_t u8Seg,
                                              uint8_t u8Bus, uint8_t u8Device,
                                              uint8_t u8Fcn, uint16_t u16Reg,
                                              uint8_t u8ReadLen,
                                              uint8_t* pPCIData, int peci_fd,
                                              uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdEndPointConfigPciLocal_seq_dom(
        target, 0, u8Seg, u8Bus, u8Device, u8Fcn, u16Reg, u8ReadLen, pPCIData,
        peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdEndPointConfig to PCI Local with the
 * provided peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigPciLocal_seq_dom(
    uint8_t target, uint8_t domainId, uint8_t u8Seg, uint8_t u8Bus,
    uint8_t u8Device, uint8_t u8Fcn, uint16_t u16Reg, uint8_t u8ReadLen,
    uint8_t* pPCIData, int peci_fd, uint8_t* cc)
{
    if (pPCIData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the read length must be a byte, word, or dword
    if (u8ReadLen != 1 && u8ReadLen != 2 && u8ReadLen != 4)
    {
        return PECI_CC_INVALID_REQ;
    }

    return peci_RdEndPointConfigPciCommon_dom(
        target, domainId, PECI_ENDPTCFG_TYPE_LOCAL_PCI, u8Seg, u8Bus, u8Device,
        u8Fcn, u16Reg, u8ReadLen, pPCIData, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to PCI MMIO space at
 * the requested PCI configuration address.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigMmio(uint8_t target, uint8_t u8Seg,
                                      uint8_t u8Bus, uint8_t u8Device,
                                      uint8_t u8Fcn, uint8_t u8Bar,
                                      uint8_t u8AddrType, uint64_t u64Offset,
                                      uint8_t u8ReadLen, uint8_t* pMmioData,
                                      uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdEndPointConfigMmio_dom(target, 0, u8Seg, u8Bus, u8Device,
                                         u8Fcn, u8Bar, u8AddrType, u64Offset,
                                         u8ReadLen, pMmioData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides read access to PCI MMIO space at
 * the requested PCI configuration address in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigMmio_dom(uint8_t target, uint8_t domainId,
                                          uint8_t u8Seg, uint8_t u8Bus,
                                          uint8_t u8Device, uint8_t u8Fcn,
                                          uint8_t u8Bar, uint8_t u8AddrType,
                                          uint64_t u64Offset, uint8_t u8ReadLen,
                                          uint8_t* pMmioData, uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pMmioData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_RdEndPointConfigMmio_seq_dom(
        target, domainId, u8Seg, u8Bus, u8Device, u8Fcn, u8Bar, u8AddrType,
        u64Offset, u8ReadLen, pMmioData, peci_fd, cc);
    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdEndPointConfig to PCI MMIO with the
 * provided peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigMmio_seq(
    uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
    uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType, uint64_t u64Offset,
    uint8_t u8ReadLen, uint8_t* pMmioData, int peci_fd, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_RdEndPointConfigMmio_seq_dom(
        target, 0, u8Seg, u8Bus, u8Device, u8Fcn, u8Bar, u8AddrType, u64Offset,
        u8ReadLen, pMmioData, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential RdEndPointConfig to PCI MMIO with the
 * provided peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_RdEndPointConfigMmio_seq_dom(
    uint8_t target, uint8_t domainId, uint8_t u8Seg, uint8_t u8Bus,
    uint8_t u8Device, uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType,
    uint64_t u64Offset, uint8_t u8ReadLen, uint8_t* pMmioData, int peci_fd,
    uint8_t* cc)
{
    struct peci_rd_end_pt_cfg_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pMmioData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the read length must be a byte, word, dword, or qword
    if (u8ReadLen != 1 && u8ReadLen != 2 && u8ReadLen != 4 && u8ReadLen != 8)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.msg_type = PECI_ENDPTCFG_TYPE_MMIO;
    cmd.params.mmio.seg = u8Seg;
    cmd.params.mmio.bus = u8Bus;
    cmd.params.mmio.device = u8Device;
    cmd.params.mmio.function = u8Fcn;
    cmd.params.mmio.bar = u8Bar;
    cmd.params.mmio.addr_type = u8AddrType;
    cmd.params.mmio.offset = u64Offset;
    cmd.rx_len = u8ReadLen;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_RD_END_PT_CFG, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pMmioData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential peci_WrEndPointConfig to PCI EndPoint with
 * the provided peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointConfig_seq(uint8_t target, uint8_t u8MsgType,
                                      uint8_t u8Seg, uint8_t u8Bus,
                                      uint8_t u8Device, uint8_t u8Fcn,
                                      uint16_t u16Reg, uint8_t DataLen,
                                      uint32_t DataVal, int peci_fd,
                                      uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrEndPointConfig_seq_dom(target, 0, u8MsgType, u8Seg, u8Bus,
                                         u8Device, u8Fcn, u16Reg, DataLen,
                                         DataVal, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential peci_WrEndPointConfig to PCI EndPoint with
 * the provided peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointConfig_seq_dom(uint8_t target, uint8_t domainId,
                                          uint8_t u8MsgType, uint8_t u8Seg,
                                          uint8_t u8Bus, uint8_t u8Device,
                                          uint8_t u8Fcn, uint16_t u16Reg,
                                          uint8_t DataLen, uint32_t DataVal,
                                          int peci_fd, uint8_t* cc)
{
    struct peci_wr_end_pt_cfg_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the write length must be a byte, word, or dword
    if (DataLen != 1 && DataLen != 2 && DataLen != 4)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.msg_type = u8MsgType;
    cmd.params.pci_cfg.seg = u8Seg;
    cmd.params.pci_cfg.bus = u8Bus;
    cmd.params.pci_cfg.device = u8Device;
    cmd.params.pci_cfg.function = u8Fcn;
    cmd.params.pci_cfg.reg = u16Reg;
    cmd.tx_len = DataLen;
    cmd.value = DataVal;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_WR_END_PT_CFG, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides write access to the EP local PCI configuration space
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointPCIConfigLocal(uint8_t target, uint8_t u8Seg,
                                          uint8_t u8Bus, uint8_t u8Device,
                                          uint8_t u8Fcn, uint16_t u16Reg,
                                          uint8_t DataLen, uint32_t DataVal,
                                          uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrEndPointPCIConfigLocal_dom(
        target, 0, u8Seg, u8Bus, u8Device, u8Fcn, u16Reg, DataLen, DataVal, cc);
}

/*-------------------------------------------------------------------------
 * This function provides write access to the EP local PCI configuration space
 * in the specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointPCIConfigLocal_dom(uint8_t target, uint8_t domainId,
                                              uint8_t u8Seg, uint8_t u8Bus,
                                              uint8_t u8Device, uint8_t u8Fcn,
                                              uint16_t u16Reg, uint8_t DataLen,
                                              uint32_t DataVal, uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    ret = peci_WrEndPointConfig_seq_dom(
        target, domainId, PECI_ENDPTCFG_TYPE_LOCAL_PCI, u8Seg, u8Bus, u8Device,
        u8Fcn, u16Reg, DataLen, DataVal, peci_fd, cc);
    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides write access to the EP local PCI configuration space
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointPCIConfig(uint8_t target, uint8_t u8Seg,
                                     uint8_t u8Bus, uint8_t u8Device,
                                     uint8_t u8Fcn, uint16_t u16Reg,
                                     uint8_t DataLen, uint32_t DataVal,
                                     uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrEndPointPCIConfig_dom(target, 0, u8Seg, u8Bus, u8Device,
                                        u8Fcn, u16Reg, DataLen, DataVal, cc);
}

/*-------------------------------------------------------------------------
 * This function provides write access to the EP local PCI configuration space
 * in the specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointPCIConfig_dom(uint8_t target, uint8_t domainId,
                                         uint8_t u8Seg, uint8_t u8Bus,
                                         uint8_t u8Device, uint8_t u8Fcn,
                                         uint16_t u16Reg, uint8_t DataLen,
                                         uint32_t DataVal, uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_WrEndPointConfig_seq_dom(
        target, domainId, PECI_ENDPTCFG_TYPE_PCI, u8Seg, u8Bus, u8Device, u8Fcn,
        u16Reg, DataLen, DataVal, peci_fd, cc);
    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides write access to PCI MMIO space at
 * the requested PCI configuration address.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointConfigMmio(uint8_t target, uint8_t u8Seg,
                                      uint8_t u8Bus, uint8_t u8Device,
                                      uint8_t u8Fcn, uint8_t u8Bar,
                                      uint8_t u8AddrType, uint64_t u64Offset,
                                      uint8_t u8DataLen, uint64_t u64DataVal,
                                      uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrEndPointConfigMmio_dom(target, 0, u8Seg, u8Bus, u8Device,
                                         u8Fcn, u8Bar, u8AddrType, u64Offset,
                                         u8DataLen, u64DataVal, cc);
}

/*-------------------------------------------------------------------------
 * This function provides write access to PCI MMIO space at
 * the requested PCI configuration address in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointConfigMmio_dom(uint8_t target, uint8_t domainId,
                                          uint8_t u8Seg, uint8_t u8Bus,
                                          uint8_t u8Device, uint8_t u8Fcn,
                                          uint8_t u8Bar, uint8_t u8AddrType,
                                          uint64_t u64Offset, uint8_t u8DataLen,
                                          uint64_t u64DataVal, uint8_t* cc)
{
    int peci_fd = -1;
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }
    ret = peci_WrEndPointConfigMmio_seq_dom(
        target, domainId, u8Seg, u8Bus, u8Device, u8Fcn, u8Bar, u8AddrType,
        u64Offset, u8DataLen, u64DataVal, peci_fd, cc);
    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function allows sequential WrEndPointConfig to PCI MMIO with the
 * provided peci file descriptor.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointConfigMmio_seq(
    uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
    uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType, uint64_t u64Offset,
    uint8_t u8DataLen, uint64_t u64DataVal, int peci_fd, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_WrEndPointConfigMmio_seq_dom(
        target, 0, u8Seg, u8Bus, u8Device, u8Fcn, u8Bar, u8AddrType, u64Offset,
        u8DataLen, u64DataVal, peci_fd, cc);
}

/*-------------------------------------------------------------------------
 * This function allows sequential WrEndPointConfig to PCI MMIO with the
 * provided peci file descriptor in the specified domain.
 *------------------------------------------------------------------------*/
EPECIStatus peci_WrEndPointConfigMmio_seq_dom(
    uint8_t target, uint8_t domainId, uint8_t u8Seg, uint8_t u8Bus,
    uint8_t u8Device, uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType,
    uint64_t u64Offset, uint8_t u8DataLen, uint64_t u64DataVal, int peci_fd,
    uint8_t* cc)
{
    struct peci_wr_end_pt_cfg_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the read length must be a byte, word, dword, or qword
    if (u8DataLen != 1 && u8DataLen != 2 && u8DataLen != 4 && u8DataLen != 8)
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.msg_type = PECI_ENDPTCFG_TYPE_MMIO;
    cmd.params.mmio.seg = u8Seg;
    cmd.params.mmio.bus = u8Bus;
    cmd.params.mmio.device = u8Device;
    cmd.params.mmio.function = u8Fcn;
    cmd.params.mmio.bar = u8Bar;
    cmd.params.mmio.addr_type = u8AddrType;
    cmd.params.mmio.offset = u64Offset;
    cmd.tx_len = u8DataLen;
    cmd.value = u64DataVal;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_WR_END_PT_CFG, (char*)&cmd, peci_fd);
    *cc = cmd.cc;

    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides crashdump discovery data over PECI
 *------------------------------------------------------------------------*/
EPECIStatus peci_CrashDump_Discovery(uint8_t target, uint8_t subopcode,
                                     uint8_t param0, uint16_t param1,
                                     uint8_t param2, uint8_t u8ReadLen,
                                     uint8_t* pData, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_CrashDump_Discovery_dom(target, 0, subopcode, param0, param1,
                                        param2, u8ReadLen, pData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides crashdump discovery data over PECI in the specified
 * domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_CrashDump_Discovery_dom(uint8_t target, uint8_t domainId,
                                         uint8_t subopcode, uint8_t param0,
                                         uint16_t param1, uint8_t param2,
                                         uint8_t u8ReadLen, uint8_t* pData,
                                         uint8_t* cc)
{
    int peci_fd = -1;
    struct peci_crashdump_disc_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the read length must be a byte, word, or qword
    if (u8ReadLen != 1 && u8ReadLen != 2 && u8ReadLen != 8)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.subopcode = subopcode;
    cmd.param0 = param0;
    cmd.param1 = param1;
    cmd.param2 = param2;
    cmd.rx_len = u8ReadLen;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_CRASHDUMP_DISC, (char*)&cmd, peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides crashdump GetFrame data over PECI
 *------------------------------------------------------------------------*/
EPECIStatus peci_CrashDump_GetFrame(uint8_t target, uint16_t param0,
                                    uint16_t param1, uint16_t param2,
                                    uint8_t u8ReadLen, uint8_t* pData,
                                    uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_CrashDump_GetFrame_dom(target, 0, param0, param1, param2,
                                       u8ReadLen, pData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides crashdump GetFrame data over PECI in the specified
 * domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_CrashDump_GetFrame_dom(uint8_t target, uint8_t domainId,
                                        uint16_t param0, uint16_t param1,
                                        uint16_t param2, uint8_t u8ReadLen,
                                        uint8_t* pData, uint8_t* cc)
{
    int peci_fd = -1;
    struct peci_crashdump_get_frame_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Per the PECI spec, the read length must be a qword or dqword
    if (u8ReadLen != 8 && u8ReadLen != 16)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.param0 = param0;
    cmd.param1 = param1;
    cmd.param2 = param2;
    cmd.rx_len = u8ReadLen;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_CRASHDUMP_GET_FRAME, (char*)&cmd, peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides telemetry discovery data over PECI
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_Discovery(uint8_t target, uint8_t subopcode,
                                     uint8_t param0, uint16_t param1,
                                     uint8_t param2, uint8_t u8ReadLen,
                                     uint8_t* pData, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_Telemetry_Discovery_dom(target, 0, subopcode, param0, param1,
                                        param2, u8ReadLen, pData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides telemetry discovery data over PECI in the specified
 * domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_Discovery_dom(uint8_t target, uint8_t domainId,
                                         uint8_t subopcode, uint8_t param0,
                                         uint16_t param1, uint8_t param2,
                                         uint8_t u8ReadLen, uint8_t* pData,
                                         uint8_t* cc)
{
    int peci_fd = -1;
    struct peci_telemetry_disc_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.subopcode = subopcode;
    cmd.param0 = param0;
    cmd.param1 = param1;
    cmd.param2 = param2;
    cmd.rx_len = u8ReadLen;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_TELEMETRY_DISC, (char*)&cmd, peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides telemetry sample data over PECI
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_GetTelemSample(uint8_t target, uint16_t index,
                                          uint16_t sample, uint8_t u8ReadLen,
                                          uint8_t* pData, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_Telemetry_GetTelemSample_dom(target, 0, index, sample,
                                             u8ReadLen, pData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides telemetry sample data over PECI in the specified
 * domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_GetTelemSample_dom(uint8_t target, uint8_t domainId,
                                              uint16_t index, uint16_t sample,
                                              uint8_t u8ReadLen, uint8_t* pData,
                                              uint8_t* cc)
{
    int peci_fd = -1;
    struct peci_telemetry_get_telem_sample_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.index = index;
    cmd.sample = sample;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_TELEMETRY_GET_TELEM_SAMPLE, (char*)&cmd,
                            peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides config watcher read over PECI
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_ConfigWatcherRd(uint8_t target, uint16_t watcher,
                                           uint16_t offset, uint8_t u8ReadLen,
                                           uint8_t* pData, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_Telemetry_ConfigWatcherRd_dom(target, 0, watcher, offset,
                                              u8ReadLen, pData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides config watcher read over PECI in the specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_ConfigWatcherRd_dom(uint8_t target, uint8_t domainId,
                                               uint16_t watcher,
                                               uint16_t offset,
                                               uint8_t u8ReadLen,
                                               uint8_t* pData, uint8_t* cc)
{
    int peci_fd;
    struct peci_telemetry_config_watcher_msg cmd = {0};
    EPECIStatus ret;

    if (pData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.watcher = watcher;
    cmd.offset = offset;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_TELEMETRY_CONFIG_WATCHER_RD, (char*)&cmd,
                            peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides config watcher write over PECI
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_ConfigWatcherWr(uint8_t target, uint16_t watcher,
                                           uint16_t offset, uint8_t u8DataLen,
                                           uint8_t* pData, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_Telemetry_ConfigWatcherWr_dom(target, 0, watcher, offset,
                                              u8DataLen, pData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides config watcher write over PECI in the specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_ConfigWatcherWr_dom(uint8_t target, uint8_t domainId,
                                               uint16_t watcher,
                                               uint16_t offset,
                                               uint8_t u8DataLen,
                                               uint8_t* pData, uint8_t* cc)
{
    int peci_fd;
    struct peci_telemetry_config_watcher_msg cmd = {0};
    EPECIStatus ret;

    if (pData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8DataLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.watcher = watcher;
    cmd.offset = offset;
    memcpy(cmd.data, pData, u8DataLen);
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_TELEMETRY_CONFIG_WATCHER_WR, (char*)&cmd,
                            peci_fd);
    *cc = cmd.cc;

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 * This function provides telemetry crashlog sample data over PECI
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_GetCrashlogSample(uint8_t target, uint16_t index,
                                             uint16_t sample, uint8_t u8ReadLen,
                                             uint8_t* pData, uint8_t* cc)
{
    //  Default to domain ID 0
    return peci_Telemetry_GetCrashlogSample_dom(target, 0, index, sample,
                                                u8ReadLen, pData, cc);
}

/*-------------------------------------------------------------------------
 * This function provides telemetry crashlog sample data over PECI in the
 * specified domain
 *------------------------------------------------------------------------*/
EPECIStatus peci_Telemetry_GetCrashlogSample_dom(
    uint8_t target, uint8_t domainId, uint16_t index, uint16_t sample,
    uint8_t u8ReadLen, uint8_t* pData, uint8_t* cc)
{
    int peci_fd = -1;
    struct peci_telemetry_get_crashlog_sample_msg cmd = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (pData == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The PECI buffer must be large enough to hold the requested data
    if (sizeof(cmd.data) < u8ReadLen)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    cmd.addr = target;
    cmd.index = index;
    cmd.sample = sample;
    cmd.domain_id = domainId;

    ret = HW_peci_issue_cmd(PECI_IOC_TELEMETRY_GET_CRASHLOG_SAMPLE, (char*)&cmd,
                            peci_fd);
    *cc = cmd.cc;
    if (ret == PECI_CC_SUCCESS)
    {
        memcpy(pData, cmd.data, u8ReadLen);
    }
    else
    {
        ret = PECI_CC_DRIVER_ERR;
    }

    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 *  This function provides raw PECI command access
 *------------------------------------------------------------------------*/
EPECIStatus peci_raw(uint8_t target, uint8_t u8ReadLen, const uint8_t* pRawCmd,
                     const uint32_t cmdSize, uint8_t* pRawResp,
                     uint32_t respSize)
{
    int peci_fd = -1;
    if (u8ReadLen && pRawResp == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Open(&peci_fd) != PECI_CC_SUCCESS)
    {
        return PECI_CC_DRIVER_ERR;
    }

    EPECIStatus ret = peci_raw_seq(target, u8ReadLen, pRawCmd, cmdSize,
                                   pRawResp, respSize, peci_fd);
    peci_Close(peci_fd);
    return ret;
}

/*-------------------------------------------------------------------------
 *  This function provides sequential raw PECI command access
 *------------------------------------------------------------------------*/
EPECIStatus peci_raw_seq(uint8_t target, uint8_t u8ReadLen,
                         const uint8_t* pRawCmd, const uint32_t cmdSize,
                         uint8_t* pRawResp, uint32_t respSize, int peci_fd)
{
    struct peci_xfer_msg cmd = {0};
    uint8_t u8TxBuf[PECI_BUFFER_SIZE] = {0};
    uint8_t u8RxBuf[PECI_BUFFER_SIZE] = {0};
    EPECIStatus ret = PECI_CC_SUCCESS;

    if (u8ReadLen && pRawResp == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The target address must be in the valid range
    if (target < MIN_CLIENT_ADDR || target > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    // Check for valid buffer sizes
    if (cmdSize > PECI_BUFFER_SIZE || respSize < u8ReadLen ||
        u8ReadLen >
            (PECI_BUFFER_SIZE - 1)) // response buffer is data + 1 status byte
    {
        return PECI_CC_INVALID_REQ;
    }

    cmd.addr = target;
    cmd.tx_len = (uint8_t)cmdSize;
    cmd.rx_len = u8ReadLen;

    memcpy(u8TxBuf, pRawCmd, cmdSize);

    cmd.tx_buf = u8TxBuf;
    cmd.rx_buf = u8RxBuf;
    ret = HW_peci_issue_cmd(PECI_IOC_XFER, (char*)&cmd, peci_fd);

    if (ret == PECI_CC_SUCCESS || ret == PECI_CC_TIMEOUT)
    {
        memcpy(pRawResp, u8RxBuf, u8ReadLen);
    }

    return ret;
}

/*-------------------------------------------------------------------------
 * This function returns the CPUID (Model and stepping) for the given PECI
 * client address
 *------------------------------------------------------------------------*/
EPECIStatus peci_GetCPUID(const uint8_t clientAddr, CPUModel* cpuModel,
                          uint8_t* stepping, uint8_t* cc)
{
    EPECIStatus ret = PECI_CC_SUCCESS;
    uint32_t cpuid = 0;

    if (cpuModel == NULL || stepping == NULL || cc == NULL)
    {
        return PECI_CC_INVALID_REQ;
    }

    // The client address must be in the valid range
    if (clientAddr < MIN_CLIENT_ADDR || clientAddr > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }

    if (peci_Ping(clientAddr) != PECI_CC_SUCCESS)
    {
        return PECI_CC_CPU_NOT_PRESENT;
    }

    ret = peci_RdPkgConfig(clientAddr, PECI_MBX_INDEX_CPU_ID,
                           PECI_PKG_ID_CPU_ID, sizeof(uint32_t),
                           (uint8_t*)&cpuid, cc);

    // Separate out the model and stepping (bits 3:0) from the CPUID
    *cpuModel = cpuid & 0xFFFFFFF0;
    *stepping = (uint8_t)(cpuid & 0x0000000F);
    return ret;
}
/*-------------------------------------------------------------------------
 * This function allows GetDIMMTemp with the provided
 * Socket, IMC, Channel and Slot.
 *------------------------------------------------------------------------*/
EPECIStatus peci_GetDIMMTemp(uint8_t target, uint8_t imc, uint8_t channel, uint8_t slot, uint8_t ts, float *temp)
{
    EPECIStatus ret = PECI_CC_SUCCESS;
     float ltemp=0;
	uint8_t ltarget= MIN_CLIENT_ADDR+target;
	
    // The target address must be in the valid range
    if (ltarget < MIN_CLIENT_ADDR || ltarget > MAX_CLIENT_ADDR)
    {
        return PECI_CC_INVALID_REQ;
    }
	//printf("\n inside peci_GetDIMMTemp call\n");
    ret = DimmGetTSTemp(target, imc, channel,slot,ts,&ltemp);

    if (ret == PECI_CC_SUCCESS)
    {
        *temp = ltemp;
    }

    return ret;
}