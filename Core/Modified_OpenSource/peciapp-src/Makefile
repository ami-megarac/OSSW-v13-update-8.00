#------- Comment/uncomment the following line to enable/disable debugging---------
DEBUG = n

#---------------------- Change according to your files ----------------------------
SRC = peci.c peci_cmds.c		# Source File
TARGET = peciapp 			# Output file

CFLAGS = -I${SPXINC}/linux
CFLAGS += -I${SPXINC}/unix
CFLAGS += -I${SPXINC}/dbgout
CFLAGS += -I${SPXINC}/global
CFLAGS += -I${SPXINC}/dimmhelper

LIBS   += -L${SPXLIB}/dbgout -ldbgout
LIBS   += -L${SPXLIB}/dimmhelper -ldimmhelper

include ${TOOLDIR}/rules/Rules.make.apps

#----------------------------------------------------------------------------------------
