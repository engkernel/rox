include config.make

FILES = ./build/kernel/kernel.asm.o ./build/kernel/kernel.o ./build/helper/string.o ./build/helper/console.o ./build/mm/frame.o ./build/mm/kheap.o ./build/mm/memory.o ./build/helper/errors.o ./build/io/io.asm.o
CFLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
INCLUDE = -I./src

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin
	
	
./bin/kernel.bin: $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./build/kernelfull.o	
	$(CC) $(CFLAGS) -T ./src/linker.ld -o $@ -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	$(ASM) -f bin $< -o $@

./build/kernel/kernel.asm.o: ./src/kernel/kernel.asm
	@mkdir -p $(@D)
	$(ASM) -f elf $< -o $@

./build/kernel/kernel.o: ./src/kernel/kernel.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@

./build/helper/string.o: ./src/helper/string.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@

./build/helper/console.o: ./src/helper/console.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@
	
./build/mm/frame.o: ./src/mm/frame.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@

./build/mm/kheap.o: ./src/mm/kheap.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@

./build/mm/memory.o: ./src/mm/memory.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@

./build/helper/errors.o: ./src/helper/errors.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@

./build/io/io.asm.o: ./src/io/io.asm
	@mkdir -p $(@D)
	$(ASM) -f elf $< -o $@
	

clean:
	rm -rf $(FILES)
	rm -rf ./kernelfull.o
	rm ./bin/kernel.bin
	rm ./bin/boot.bin
	rm ./bin/os.bin
