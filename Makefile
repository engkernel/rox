include config.make

FILES = ./build/kernel.asm.o ./build/kernel.o
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
INCLUDES = -I./src

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin
	
	
./bin/kernel.bin: $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./build/kernelfull.o	
	$(CC) $(FLAGS) -T ./src/linker.ld -o $@ -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot.asm
	$(ASM) -f bin $< -o $@

./build/kernel.asm.o: ./src/kernel.asm
	$(ASM) -f elf $< -o $@

./build/kernel.o: ./src/kernel.c
	$(CC) $(INCLUDE) $(FLAGS) -std=gnu99 -c $< -o $@

clean:
	rm -rf $(FILES)
	rm -rf ./kernelfull.o
	rm ./bin/kernel.bin
	rm ./bin/boot.bin
	rm ./bin/os.bin
