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
	-I$(OO)/include

LDFLAGS = -shared \
	-fuse-ld=lld \
	-Wl,--build-id=none \
	-Wl,--emit-relocs \
	-Wl,-T,$(OO)/link.x \
	-Wl,-e,module_start \
	-L$(OO)/lib \
	-lkernel \
	-lc

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET).elf $(LDFLAGS)
	$(OO)/bin/linux/create-fself -in=$(TARGET).elf -lib=$(TARGET).prx
	$(STRIP) $(TARGET).elf

clean:
	rm -f *.elf *.prx
