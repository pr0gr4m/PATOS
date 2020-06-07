ARCH = armv7-a
MCPU = cortex-a8

CC = arm-linux-gnu-gcc
AS = arm-linux-gnu-as
LD = arm-linux-gnu-ld
OC = arm-linux-gnu-objcopy

LINKER_SCRIPT = ./patos.ld
MAP_FILE = build/patos.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

C_SRCS = $(wildcard boot/*.c)
C_OBJS = $(patsubst boot/%.c, build/%.o, $(C_SRCS))

INC_DIRS = -I include

patos = build/patos.axf
patos_bin = build/patos.bin

.PHONY: all clean run debug gdb

all: $(patos)

clean:
	@rm -rf build

run: $(patos)
	qemu-system-arm -M realview-pb-a8 -kernel $(patos)

debug: $(patos)
	qemu-system-arm -M realview-pb-a8 -kernel $(patos) -S -gdb tcp::1234,ipv4

gdb:
	arm-none-eabi-gdb

$(patos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(patos) $(ASM_OBJS) $(C_OBJS) -Map=$(MAP_FILE)
	$(OC) -O binary $(patos) $(patos_bin)

build/%.os: $(ASM_SRCS)
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) -c -g -o $@ $<

build/%.o: $(C_SRCS)
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) -c -g -o $@ $<
