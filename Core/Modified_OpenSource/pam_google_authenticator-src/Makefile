#DEBUG = n

LIBRARY_NAME = pam_custom_google

SRC += base32.c
SRC += base32_prog.c
SRC += hmac.c
SRC += pam_google_authenticator.c
SRC += sha1.c
SRC += util.c

CFLAGS += -I./
CFLAGS += -I${SPXINC}/user/include
CFLAGS += -I${SPXINC}/global
CFLAGS += -I${SPXINC}/unix
CFLAGS += -I${TARGETDIR}/usr/include
CFLAGS  +=  -I${SPXINC}/safesystem

LIBS += -L${SPXLIB}/unix -lunix
LIBS += -L${SPXLIB}/safesystem -lsafesystem

include ${TOOLDIR}/rules/Rules.make.libs
