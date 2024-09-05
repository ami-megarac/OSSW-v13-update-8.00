#----------------------------------------------------------------------
#                         Makefile to build libpeci
#                 Copyright (c) 2008 American Megatrends Inc 
#-----------------------------------------------------------------------

#------- Comment/uncomment the following line to enable/disable debugging------
DEBUG 		= n
#------------------------------------------------------------------------------

#---------------------- Change according to your files ------------------------
LIBRARY_NAME	= libnvdimmpeci4
SRC		=  peci.c

CFLAGS += -I${SPXINC}/peci
CFLAGS += -I${SPXINC}/global
CFLAGS += -I${SPXINC}/checksum
CFLAGS += -I${SPXINC}/unix

LIBS+= -L${SPXLIB}/checksum -lchecksum
LIBS   += -L${SPXLIB}/unix -lunix


include ${TOOLDIR}/rules/Rules.make.libs
#------------------------------------------------------------------------------
