TARGET = netdump
SRC = main.cpp

OO = /workspaces/Gta-net-dump/OpenOrbis-PS4-Toolchain/bin/linux/OpenOrbis/PS4Toolchain

CC = clang++
STRIP = strip

CFLAGS = -O2 -fPIC -std=c++17 \
	-fno-exceptions \
	-fno-rtti \
	-fno-stack-protector \
	-fno-ident \
	-nostdlib \
	-nodefaultlibs \
	-ffreestanding \
	-I$(OO)/include

LDFLAGS = -shared \
	-fuse-ld=lld \
	-Wl,--build-id=none \
	-Wl,--no-undefined \
	-L$(OO)/lib \
	-lkernel

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET).elf $(LDFLAGS)
	$(OO)/bin/linux/create-fself -in=$(TARGET).elf -lib=$(TARGET).prx
	$(STRIP) $(TARGET).elf

clean:
	rm -f *.elf *.prx
