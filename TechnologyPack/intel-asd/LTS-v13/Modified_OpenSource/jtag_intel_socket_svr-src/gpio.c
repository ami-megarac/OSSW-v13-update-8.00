/*
Copyright (c) 2019, Intel Corporation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "gpio.h"

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
// clang-format off
#include <safe_str_lib.h>
// clang-format on

#ifdef SPX_BMC
#include <dlfcn.h>
#include "logging.h"

static const ASD_LogStream stream = ASD_LogStream_Pins;
static const ASD_LogOption option = ASD_LogOption_None;

#define EXTENDED_GPIO_START_NUM 30000
#endif

#define GPIO_EDGE_NONE_STRING "none"
#define GPIO_EDGE_RISING_STRING "rising"
#define GPIO_EDGE_FALLING_STRING "falling"
#define GPIO_EDGE_BOTH_STRING "both"

#define GPIO_DIRECTION_IN_STRING "in"
#define GPIO_DIRECTION_OUT_STRING "out"
#define GPIO_DIRECTION_HIGH_STRING "high"
#define GPIO_DIRECTION_LOW_STRING "low"
#define GPIO_LABEL_MAX_SIZE 10
#define BUFF_SIZE 48

STATUS gpio_export(int gpio, int* gpio_fd)
{
#ifndef SPX_BMC
    int fd;
    char buf[BUFF_SIZE];
    int ia[1];
    STATUS result = ST_OK;
    if (!gpio_fd)
        return ST_ERR;
    ia[0] = gpio;
    sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", ia, 1);
    *gpio_fd = open(buf, O_RDWR);
    if (*gpio_fd == -1)
    {
        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd >= 0)
        {
            sprintf_s(buf, sizeof(buf), "%d", ia, 1);
            if (write(fd, buf, strnlen_s(buf, BUFF_SIZE)) < 0)
            {
                result = ST_ERR;
            }
            else
            {
                sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", ia,
                          1);
                *gpio_fd = open(buf, O_RDWR);
                if (*gpio_fd == -1)
                    result = ST_ERR;
            }
            close(fd);
        }
        else
        {
            result = ST_ERR;
        }
    }
    return result;
#else
    int fd;
    char buf[BUFF_SIZE];
    STATUS result = ST_OK;
    void *handle = NULL;
    int (*PDK_GPIOExport)(int gpio, int *gpio_fd) = NULL;

    if (!gpio_fd)
        return ST_ERR;

    /* BMC GPIO */
    if (gpio < EXTENDED_GPIO_START_NUM)
    {
        sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);
        *gpio_fd = open(buf, O_RDWR);
        if (*gpio_fd == -1)
        {
            fd = open("/sys/class/gpio/export", O_WRONLY);
            if (fd >= 0)
            {
                sprintf_s(buf, sizeof(buf), "%d", gpio);
                if (write(fd, buf, strlen(buf)) < 0)
                {
                    result = ST_ERR;
                }
                else
                {
                    sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);
                    *gpio_fd = open(buf, O_RDWR);
                    if (*gpio_fd == -1)
                        result = ST_ERR;
                }
                close(fd);
            }
            else
            {
                result = ST_ERR;
            }
        }
    }
    /* Extended GPIO */
    else
    {
        handle = dlopen(ASDCFG_PATH, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
        if (handle == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in loading library %s\n", dlerror());
            return ST_ERR;
        }

        PDK_GPIOExport = dlsym(handle, "PDK_GPIOExport");
        if (PDK_GPIOExport == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting symbol %s\n", dlerror());
            dlclose(handle);
            return ST_ERR;
        }

        if (PDK_GPIOExport(gpio, gpio_fd) < 0)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in GPIO export\n");
            dlclose(handle);
            return ST_ERR;
        }

        dlclose(handle);
    }

    return result;
#endif
}

STATUS gpio_unexport(int gpio)
{
#ifndef SPX_BMC
    int fd;
    char buf[BUFF_SIZE];
    STATUS result = ST_OK;
    int ia[1];
    ia[0] = gpio;
    sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", ia, 1);
    fd = open(buf, O_WRONLY);
    if (fd >= 0)
    {
        // the gpio exists
        close(fd);
        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        int ia[1];
        ia[0] = gpio;

        if (fd >= 0)
        {
            sprintf_s(buf, sizeof(buf), "%d", ia, 1);
            if (write(fd, buf, strnlen_s(buf, BUFF_SIZE)) < 0)
            {
                result = ST_ERR;
            }
            close(fd);
        }
        else
        {
            result = ST_ERR;
        }
    }
    return result;
#else
    int fd;
    char buf[BUFF_SIZE];
    STATUS result = ST_OK;
    void *handle = NULL;
    int (*PDK_GPIOUnexport)(int gpio) = NULL;

    /* BMC GPIO */
    if (gpio < EXTENDED_GPIO_START_NUM)
    {
        sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);
        fd = open(buf, O_WRONLY);
        if (fd >= 0)
        {
            /* the gpio exists */
            close(fd);
            fd = open("/sys/class/gpio/unexport", O_WRONLY);

            if (fd >= 0)
            {
                sprintf_s(buf, sizeof(buf), "%d", gpio);
                if (write(fd, buf, strlen(buf)) < 0)
                {
                    result = ST_ERR;
                }
                close(fd);
            }
            else
            {
                result = ST_ERR;
            }
        }
    }
    /* Extended GPIO */
    else
    {
        handle = dlopen(ASDCFG_PATH, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
        if (handle == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in loading library %s\n", dlerror());
            return ST_ERR;
        }

        PDK_GPIOUnexport = dlsym(handle, "PDK_GPIOUnexport");
        if (PDK_GPIOUnexport == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting symbol %s\n", dlerror());
            dlclose(handle);
            return ST_ERR;
        }

        if (PDK_GPIOUnexport(gpio) < 0)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in GPIO unexport\n");
            dlclose(handle);
            return ST_ERR;
        }

        dlclose(handle);
    }

    return result;
#endif
}

#ifndef SPX_BMC 
STATUS gpio_get_value(int fd, int* value)
#else
STATUS gpio_get_value(int fd, int gpio, int *value)
#endif
{
#ifndef SPX_BMC 
    STATUS result = ST_ERR;
    char ch;

    if (value && fd >= 0)
    {
        if(lseek(fd, 0, SEEK_SET) < 0)
        {
            return result;
        }
        if(read(fd, &ch, 1) < 0)
        {
            return result;
        }
        *value = ch != '0' ? 1 : 0;
        result = ST_OK;
    }
    return result;
#else
    STATUS result = ST_ERR;
    char ch;
    void *handle = NULL;
    int (*PDK_GetGPIOData)(int fd, int gpio, int *value) = NULL;

    /* BMC GPIO */
    if (gpio < EXTENDED_GPIO_START_NUM)
    {
        if (value && fd >= 0)
        {
            /*Reason for False Positive - This Function without wrong*/
            /*coverity[check_return : FALSE]*/
            lseek(fd, 0, SEEK_SET);
            if(read(fd, &ch, 1)!=-1)
            *value = ch != '0' ? 1 : 0;
            result = ST_OK;
        }
    }
    /* Extended GPIO */
    else
    {
        handle = dlopen(ASDCFG_PATH, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
        if (handle == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in loading library %s\n", dlerror());
            return ST_ERR;
        }

        PDK_GetGPIOData = dlsym(handle, "PDK_GetGPIOData");
        if (PDK_GetGPIOData == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting symbol %s\n", dlerror());
            dlclose(handle);
            return ST_ERR;
        }

        if (PDK_GetGPIOData(fd, gpio, value) < 0)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting GPIO data\n");
            dlclose(handle);
            return ST_ERR;
        }

        result = ST_OK;
        dlclose(handle);
    }

    return result;
#endif
}

#ifndef SPX_BMC 
STATUS gpio_set_value(int fd, int value)
#else
STATUS gpio_set_value(int fd, int gpio, int value)
#endif
{
#ifndef SPX_BMC 
    STATUS result = ST_ERR;

    if (fd >= 0)
    {
        /*Reason for False Positive - This Function without wrong*/
        /*coverity[check_return : FALSE]*/
        lseek(fd, 0, SEEK_SET);
        ssize_t written = write(fd, value == 1 ? "1" : "0", sizeof(char));
        if (written == sizeof(char))
        {
            result = ST_OK;
        }
    }
    return result;
#else
    STATUS result = ST_ERR;
    void *handle = NULL;
    int (*PDK_SetGPIOData)(int fd, int gpio, int value) = NULL;

    /* BMC GPIO */
    if (gpio < EXTENDED_GPIO_START_NUM)
    {
        if (fd >= 0)
        {
            /*Reason for False Positive - This Function without wrong*/
            /*coverity[check_return : FALSE]*/
            lseek(fd, 0, SEEK_SET);
            ssize_t written = write(fd, value == 1 ? "1" : "0", sizeof(char));
            if (written == sizeof(char))
            {
                result = ST_OK;
            }
        }
    }
    /* Extended GPIO */
    else
    {
        handle = dlopen(ASDCFG_PATH, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
        if (handle == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in loading library %s\n", dlerror());
            return ST_ERR;
        }

        PDK_SetGPIOData = dlsym(handle, "PDK_SetGPIOData");
        if (PDK_SetGPIOData == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting symbol %s\n", dlerror());
            dlclose(handle);
            return ST_ERR;
        }

        if (PDK_SetGPIOData(fd, gpio, value) < 0)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in setting GPIO data\n");
            dlclose(handle);
            return ST_ERR;
        }

        result = ST_OK;
        dlclose(handle);
    }

    return result;
#endif
}

STATUS gpio_set_edge(int gpio, GPIO_EDGE edge)
{
#ifndef SPX_BMC
    int fd;
    char buf[BUFF_SIZE];
    int ia[1];
    ia[0] = gpio;
    STATUS result = ST_ERR;

    sprintf_s((buf), sizeof(buf), "/sys/class/gpio/gpio%d/edge", ia, 1);
    fd = open(buf, O_WRONLY);
    if (fd >= 0)
    {
        if (edge == GPIO_EDGE_NONE)
            write(fd, GPIO_EDGE_NONE_STRING,
                  strnlen_s(GPIO_EDGE_NONE_STRING, GPIO_LABEL_MAX_SIZE));
        else if (edge == GPIO_EDGE_RISING)
            write(fd, GPIO_EDGE_RISING_STRING,
                  strnlen_s(GPIO_EDGE_RISING_STRING, GPIO_LABEL_MAX_SIZE));
        else if (edge == GPIO_EDGE_FALLING)
            write(fd, GPIO_EDGE_FALLING_STRING,
                  strnlen_s(GPIO_EDGE_FALLING_STRING, GPIO_LABEL_MAX_SIZE));
        else if (edge == GPIO_EDGE_BOTH)
            write(fd, GPIO_EDGE_BOTH_STRING,
                  strnlen_s(GPIO_EDGE_BOTH_STRING, GPIO_LABEL_MAX_SIZE));
        close(fd);

        result = ST_OK;
    }
    return result;
#else
    int fd;
    char buf[BUFF_SIZE];
    STATUS result = ST_ERR;
    void *handle = NULL;
    int (*PDK_GPIOSetEdge)(int gpio, GPIO_EDGE edge) = NULL;

    /* BMC GPIO */
    if (gpio < EXTENDED_GPIO_START_NUM)
    {
        sprintf_s((buf), sizeof(buf), "/sys/class/gpio/gpio%d/edge", gpio);
        fd = open(buf, O_WRONLY);
        if (fd >= 0)
        {
            if (edge == GPIO_EDGE_NONE)
                write(fd, GPIO_EDGE_NONE_STRING, strlen(GPIO_EDGE_NONE_STRING));
            else if (edge == GPIO_EDGE_RISING)
                write(fd, GPIO_EDGE_RISING_STRING, strlen(GPIO_EDGE_RISING_STRING));
            else if (edge == GPIO_EDGE_FALLING)
                write(fd, GPIO_EDGE_FALLING_STRING,
                        strlen(GPIO_EDGE_FALLING_STRING));
            else if (edge == GPIO_EDGE_BOTH)
                write(fd, GPIO_EDGE_BOTH_STRING, strlen(GPIO_EDGE_BOTH_STRING));
            close(fd);
            result = ST_OK;
        }
    }
    /* Extended GPIO */
    else
    {
        handle = dlopen(ASDCFG_PATH, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
        if (handle == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in loading library %s\n", dlerror());
            return ST_ERR;
        }

        PDK_GPIOSetEdge = dlsym(handle, "PDK_GPIOSetEdge");
        if (PDK_GPIOSetEdge == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting symbol %s\n", dlerror());
            dlclose(handle);
            return ST_ERR;
        }

        if (PDK_GPIOSetEdge(gpio, edge) < 0)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in setting GPIO edge\n");
            dlclose(handle);
            return ST_ERR;
        }

        result = ST_OK;
        dlclose(handle);
    }

    return result;
#endif
}

STATUS gpio_set_direction(int gpio, GPIO_DIRECTION direction)
{
#ifndef SPX_BMC
    int fd;
    char buf[BUFF_SIZE];
    int ia[1];
    ia[0] = gpio;
    STATUS result = ST_ERR;
    sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", ia, 1);
    fd = open(buf, O_WRONLY);
    if (fd >= 0)
    {
        if (direction == GPIO_DIRECTION_IN)
            write(fd, GPIO_DIRECTION_IN_STRING,
                  strnlen_s(GPIO_DIRECTION_IN_STRING, GPIO_LABEL_MAX_SIZE));
        else if (direction == GPIO_DIRECTION_OUT)
            write(fd, GPIO_DIRECTION_OUT_STRING,
                  strnlen_s(GPIO_DIRECTION_OUT_STRING, GPIO_LABEL_MAX_SIZE));
        else if (direction == GPIO_DIRECTION_HIGH)
            write(fd, GPIO_DIRECTION_HIGH_STRING,
                  strnlen_s(GPIO_DIRECTION_HIGH_STRING, GPIO_LABEL_MAX_SIZE));
        else if (direction == GPIO_DIRECTION_LOW)
            write(fd, GPIO_DIRECTION_LOW_STRING,
                  strnlen_s(GPIO_DIRECTION_LOW_STRING, GPIO_LABEL_MAX_SIZE));
        close(fd);
        result = ST_OK;
    }
    return result;
#else
    int fd;
    char buf[BUFF_SIZE];
    STATUS result = ST_ERR;
    void *handle = NULL;
    int (*PDK_GPIOSetDirection)(int gpio, GPIO_DIRECTION direction) = NULL;

    /* BMC GPIO */
    if (gpio < EXTENDED_GPIO_START_NUM)
    {
        sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);
        fd = open(buf, O_WRONLY);
        if (fd >= 0)
        {
            if (direction == GPIO_DIRECTION_IN)
                write(fd, GPIO_DIRECTION_IN_STRING,
                        strlen(GPIO_DIRECTION_IN_STRING));
            else if (direction == GPIO_DIRECTION_OUT)
                write(fd, GPIO_DIRECTION_OUT_STRING,
                        strlen(GPIO_DIRECTION_OUT_STRING));
            else if (direction == GPIO_DIRECTION_HIGH)
                write(fd, GPIO_DIRECTION_HIGH_STRING,
                        strlen(GPIO_DIRECTION_HIGH_STRING));
            else if (direction == GPIO_DIRECTION_LOW)
                write(fd, GPIO_DIRECTION_LOW_STRING,
                        strlen(GPIO_DIRECTION_LOW_STRING));
            close(fd);
            result = ST_OK;
        }
    }
    /* Extended GPIO */
    else
    {
        handle = dlopen(ASDCFG_PATH, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
        if (handle == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in loading library %s\n", dlerror());
            return ST_ERR;
        }

        PDK_GPIOSetDirection = dlsym(handle, "PDK_GPIOSetDirection");
        if (PDK_GPIOSetDirection == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting symbol %s\n", dlerror());
            dlclose(handle);
            return ST_ERR;
        }

        if (PDK_GPIOSetDirection(gpio, direction) < 0)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in setting GPIO direction\n");
            dlclose(handle);
            return ST_ERR;
        }

        result = ST_OK;
        dlclose(handle);
    }

    return result;
#endif
}

STATUS gpio_set_active_low(int gpio, bool active_low)
{
#ifndef SPX_BMC
    int fd;
    char buf[BUFF_SIZE];
    STATUS result = ST_ERR;
    int ia[1];
    ia[0] = gpio;
    sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/active_low", ia, 1);
    fd = open(buf, O_WRONLY);
    if (fd >= 0)
    {
        if (write(fd, active_low ? "1" : "0", 1) == 1)
        {
            result = ST_OK;
        }
        close(fd);
    }
    return result;
#else
    int fd;
    char buf[BUFF_SIZE];
    STATUS result = ST_ERR;
    void *handle = NULL;
    int (*PDK_GPIOSetActiveLow)(int gpio, bool active_low) = NULL;

    /* BMC GPIO */
    if (gpio < EXTENDED_GPIO_START_NUM)
    {
        sprintf_s(buf, sizeof(buf), "/sys/class/gpio/gpio%d/active_low", gpio);
        fd = open(buf, O_WRONLY);
        if (fd >= 0)
        {
            if (write(fd, active_low ? "1" : "0", 1) == 1)
            {
                result = ST_OK;
            }
            close(fd);
        }
    }
    /* Extended GPIO */
    else
    {
        handle = dlopen(ASDCFG_PATH, RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE);
        if (handle == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in loading library %s\n", dlerror());
            return ST_ERR;
        }

        PDK_GPIOSetActiveLow = dlsym(handle, "PDK_GPIOSetActiveLow");
        if (PDK_GPIOSetActiveLow == NULL)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in getting symbol %s\n", dlerror());
            dlclose(handle);
            return ST_ERR;
        }

        if (PDK_GPIOSetActiveLow(gpio, active_low) < 0)
        {
            ASD_log(ASD_LogLevel_Error, stream, option, "Error in setting GPIO active low\n");
            dlclose(handle);
            return ST_ERR;
        }

        result = ST_OK;
        dlclose(handle);
    }

    return result;
#endif
}
