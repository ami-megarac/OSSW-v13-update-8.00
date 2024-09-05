#
# Copyright (c) 2017, Intel Corporation
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name of Intel Corporation nor the names of its contributors
#      may be used to endorse or promote products derived from this software
#      without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
ifeq ($(apptype),server)
SRC =      asd_main.c  asd_msg.c authenticate.c auth_none.c auth_pam.c config.c \
		   ext_network.c ext_tcp.c ext_tls.c gpio.c i2c_handler.c i3c_handler.c i3c_debug_handler.c spp_handler.c\
		   i2c_msg_builder.c jtag_handler.c logging.c \
		   session.c ipmi.c target_handler.c vprobe_handler.c \
		   asd_target_api.c asd_server_interface.c \
		   asd_target_interface.c asd_server_api.c \
		   safec/ignore_handler_s.c safec/memcpy_s.c safec/memcmp_s.c safec/mem_primitives_lib.c \
		   safec/memset_s.c safec/safe_mem_constraint.c safec/safe_str_constraint.c \
		   safec/sprintf_s.c safec/strnlen_s.c safec/strcpy_s.c safec/strcmp_s.c

TARGET = RemoteDebugServer 			# Output file
endif

ifeq ($(apptype),testapp)
SRC = jtag_handler.c logging.c jtag_test.c \
		   safec/ignore_handler_s.c safec/memcpy_s.c safec/memcmp_s.c safec/mem_primitives_lib.c \
		   safec/memset_s.c safec/safe_mem_constraint.c safec/safe_str_constraint.c \
		   safec/sprintf_s.c safec/strnlen_s.c safec/strcpy_s.c safec/strcmp_s.c
TARGET = jtag_test
endif

ADTM_VERSION := $(shell date +"%c" | tr ' :' '__')

CFLAGS += -I.
CFLAGS += -I${TARGETDIR}/usr/include
CFLAGS += -I${SPXINC}
CFLAGS += -I${SPXINC}/global
CFLAGS += -I${SPXINC}/ipmihalapi
CFLAGS += -I${SPXINC}/featuredefine
CFLAGS += -I${SPXINC}/ipmi
CFLAGS += -I${SPXINC}/pdk
CFLAGS += -I${SPXINC}/unix
CFLAGS += -I${SPXINC}/pdk
CFLAGS += -I${SPXINC}/hostname
CFLAGS += -I${SPXINC}/network
CFLAGS += -I${SPXINC}/dbgout
CFLAGS += -I${SPXINC}/libipmi
CFLAGS += -I${SPXINC}/jtag_intel
CFLAGS += -Isafec
CFLAGS += -I${SPXINC}/linux

CFLAGS += -D_GNU_SOURCE -DSPX_BMC -Wno-type-limits -Wno-missing-field-initializers -Wno-sign-compare -Wno-unused-parameter -fno-strict-aliasing -std=c99
CFLAGS += -Wno-missing-braces
CPPFLAGS += -std=gnu++0x -O3

LIBS += -lpthread -ldl -lm
LIBS += -L${SPXLIB}/dbgout -ldbgout
LIBS += -L$(SPXLIB)/ipmiparams -lipmiparams
LIBS += -L$(SPXLIB)/ipmi -lipmi
LIBS += -L$(SPXLIB)/gpio -lgpio
LIBS += -L${SPXLIB}/unix -lunix
LIBS += ${IMAGE_TREE}/lib/${MULTI_ARCH_LIB}/libpam.so.0
LIBS    += ${IMAGE_TREE}/usr/lib/${MULTI_ARCH_LIB}/libcrypto.so
LIBS    += ${IMAGE_TREE}/usr/lib/${MULTI_ARCH_LIB}/libssl.so
LIBS    += ${IMAGE_TREE}/usr/lib/${MULTI_ARCH_LIB}/libz.so


include ${TOOLDIR}/rules/Rules.make.apps

#ifeq ($(CONFIG_SPX_FEATURE_NO_STRICT_CC_ANY),NO)
CFLAGS += -Wno-error
#endif

