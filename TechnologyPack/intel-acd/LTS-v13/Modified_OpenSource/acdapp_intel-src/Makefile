#------- Comment/uncomment the following line to enable/disable debugging---------
DEBUG = n

#---------------------- Change according to your files ----------------------------
SRC	+= CrashdumpSections/TorDump.c \
       CrashdumpSections/BigCore.c \
       CrashdumpSections/utils.c\
       CrashdumpSections/OemData.c \
       CrashdumpSections/base64Encode.c \
       CrashdumpSections/Crashlog.c \
       CrashdumpSections/flow.c \
       CrashdumpSections/logger.c \
       CrashdumpSections/validator.c \
       CrashdumpSections/inputparser.c \
       CrashdumpSections/cmdprocessor.c \
       CrashdumpSections/crashdump.c 
SRCCPP +=  crashdump.cpp  \
	   utils_dbusplus.cpp \
           utils_triage.cpp

TARGET = Crashdump 			# Output file

LDFLAGS += -lpthread

CFLAGS += -DBUILD_RAW
CFLAGS += -DBUILD_JSON
CFLAGS += -I${SPXINC}/global
CFLAGS += -I${SPXINC}/dbgout
CFLAGS += -I${SPXINC}
CFLAGS += -I${SPXINC}/peci
CFLAGS += -I${SPXINC}/libpeci
#CFLAGS += -I${SPXINC}/peci4
CFLAGS += -I${SPXINC}/checksum
CFLAGS += -I${SPXINC}/cjson2
CFLAGS += -IcrashDump
CFLAGS += -DSPX_BMC_ACD
CFLAGS +=  -Wno-missing-field-initializers
CFLAGS +=  -Wno-unused-but-set-variable
CFLAGS +=  -Wno-unused-parameter
CFLAGS +=  -Wno-unused-function
CFLAGS +=  -Wno-sign-compare
CFLAGS +=  -Wno-unused-variable
CFLAGS +=  -Wno-write-strings
CFLAGS +=  -Wno-psabi
#CFLAGS += -std=gnu99

#EXTRA_CFLAGS += -I$(PWD)/include

CXXFLAGS += -std=c++11
CXXFLAGS += -std=gnu++17
#CXXFLAGS += -std=c++1z

# Add the Corresponding library
LIBS  += -L${SPXLIB}/libcjson2 -lcjson2
LIBS += -L${SPXLIB}/dbgout -ldbgout
#LIBS += -L${SPXLIB}/checksum -lchecksum
LIBS += -L${SPXLIB}/peci -lpeci
#LIBS    += -lpeci   -lstdc++fs
#LIBS+= -L$(SPXLIB)/dbgout -Wl,-rpath /usr/local/lib -ldbgout
#LIBS+= -L${SPXLIB}/gpio -lgpio
LIBS+= -lpthread -ldl
LIBS+= -lrt
#LIBS+= -ldl
LIBS+= -lstdc++
LIBS+= -lstdc++fs

include ${TOOLDIR}/rules/Rules.make.apps

#----------------------------------------------------------------------------------------

