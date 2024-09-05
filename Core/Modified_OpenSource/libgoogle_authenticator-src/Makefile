#------- Comment/uncomment the following line to enable/disable debugging---------
DEBUG = n
#---------------------------------------------------------------------------------
LIBRARY_NAME = libgoogle_auth

SRC     +=      base32.c
#SRC     +=      base32_prog.c
SRC     +=      google-authenticator.c
SRC     +=      hmac.c
SRC     +=      sha1.c
SRC     +=      util.c

TARGET  =   google-authenticator 		# Output file

#CFLAGS += -Wall -Wextra -Werror -DRUPIFY -std=gnu99

CFLAGS += -I${SPXINC}/global
CFLAGS += -I${SPXINC}/unix
CFLAGS += -I.

LIBS   += -L${SPXLIB}/unix -lunix
LIBS   += -ldl

CFLAGS  +=  -I${SPXINC}/safesystem
LIBS    +=  -L${SPXLIB}/safesystem -lsafesystem

#include ${TOOLDIR}/rules/Rules.make.apps
include ${TOOLDIR}/rules/Rules.make.libs
