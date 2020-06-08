ARCH = armv7-a
MCPU = cortex-a8

TARGET = rvpb

CC = arm-linux-gnu-gcc
AS = arm-linux-gnu-as
LD = arm-linux-gnu-ld
OC = arm-linux-gnu-objcopy

LINKER_SCRIPT = ./patos.ld
MAP_FILE = build/patos.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

VPATH = boot		\
	hal/$(TARGET)	\
	lib

C_SRCS = $(notdir $(wildcard boot/*.c))
C_SRCS += $(notdir $(wildcard hal/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard lib/*.c))
C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))

INC_DIRS = -I include		\
	-I hal			\
	-I hal/$(TARGET)	\
	-I lib

CFLAGS = -c -g -std=c11

patos = build/patos.axf
patos_bin = build/patos.bin

.PHONY: all clean run debug gdb

all: $(patos)

clean:
	@rm -rf build

run: $(patos)
	qemu-system-arm -M realview-pb-a8 -kernel $(patos) -nographic

debug: $(patos)
	qemu-system-arm -M realview-pb-a8 -kernel $(patos) -S -gdb tcp::1234,ipv4

gdb:
	arm-none-eabi-gdb

$(patos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(patos) $(ASM_OBJS) $(C_OBJS) -Map=$(MAP_FILE)
	$(OC) -O binary $(patos) $(patos_bin)

build/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) $(CFLAGS) -o $@ $<

build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) $(CFLAGS) -o $@ $<
