ARCH = armv7-a
MCPU = cortex-a8

CC = arm-linux-gnu-gcc
AS = arm-linux-gnu-as
LD = arm-linux-gnu-ld
OC = arm-linux-gnu-objcopy

LINKER_SCRIPT = ./patos.ld

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.o, $(ASM_SRCS))

INC_DIRS = include

patos = build/patos.axf
patos_bin = build/patos.bin

.PHONY: all clean run debug gdb

all: $(patos)

clean:
	@rm -fr build

run: $(patos)
	qemu-system-arm -M realview-pb-a8 -kernel $(patos)

debug: $(patos)
	qemu-system-arm -M realview-pb-a8 -kernel $(patos) -S -gdb tcp::1234,ipv4

gdb:
	arm-none-eabi-gdb

$(patos): $(ASM_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(patos) $(ASM_OBJS)
	$(OC) -O binary $(patos) $(patos_bin)

build/%.o: boot/%.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) -I $(INC_DIRS) -c -g -o $@ $<
