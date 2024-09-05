#--------------------------------------------------------------------------------------------------------------------
#                 Makefile to build libcompnvdimm
#		Copyright (c) 1985-2021, American Megatrends International LLC.
#		3095 Satellite Boulevard Building 800, Suite 425 Duluth, Georgia 30096
#---------------------------------------------------------------------------------------------------------------------

#------- Comment/uncomment the following line to enable/disable debugging---------------
DEBUG = n
#---------------------------------------------------------------------------------------------------------------------
LIBRARY_NAME 	= libnvdimmpeci
#---------------------------------------------------------------------------------------------------------------------
#---------------------- Change according to your files ----------------------------------------------------
SRC     = common.c
SRC     += fis.c
SRC     += i3c_peci.c
SRC     += smbus_peci.c
SRC     += smbus_direct.c

CFLAGS += -I${SPXINC}/unix
CFLAGS += -I${SPXINC}/global
CFLAGS += -I${SPXINC}/nvdimmpeci4
CFLAGS += -I${SPXINC}/linux

LIBS   += -L${SPXLIB}/unix -lunix
LIBS   += -L${SPXLIB}/nvdimmpeci4 -lnvdimmpeci4
LIBS += -L$(SPXLIB)/dbgout -Wl,-rpath /usr/local/lib -ldbgout

include ${TOOLDIR}/rules/Rules.make.libs
