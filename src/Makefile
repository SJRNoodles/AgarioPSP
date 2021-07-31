TARGET = Agar
OBJS = main.o \
gfx.o \
glib2d.o


CFLAGS = 
CXXFLAHS = $(CFLAGS) -std=c++14 -fno-rtti
ASFLAGS = $(CFLAGS)

#PSP
BUILD_PRX = 1
PSP_FW_VERSION = 500
PSP_LARGE_MEMORY = 1
LIBS = -lpng -ljpeg -lz -lpspgu -lm -lpspvram

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Agario
PSP_EBOOT_ICON = ICON0.PNG

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak