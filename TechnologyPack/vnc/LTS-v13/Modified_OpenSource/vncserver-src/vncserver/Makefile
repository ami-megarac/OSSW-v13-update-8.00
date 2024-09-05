#-----------------------------------------------------------------------------
DEBUG = n
#-------------------------------------------------------------------------

#------------------------------------------------------------------------------
TARGET  = vncserver
#------------------------------------------------------------------------------

SRC		+= 	vncserver.c
SRC     += 	hid.c


LIBS    += -L${CURDIR}/../build -lvncserver
LIBS	+= -L${SPXLIB}/video_vnc -lvideo_vnc
LIBS	+= -L${SPXLIB}/usbgadget_vnc -lusbgadget_vnc
LIBS	+=	-lpthread
LIBS	+=	-ldl

include ${TOOLDIR}/rules/Rules.make.apps

