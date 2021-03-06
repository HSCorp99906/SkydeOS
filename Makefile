all:
	@ ##
	@ # Buildscruot for SkydeOS.
	@ #
	@ # @Makefile
	@ # @version 1.0
	nasm -felf src/x86_64/kernel/interrupts/syscalls.S -o obj/syscalls.o
	gcc -m32 -c src/x86_64/kernel/kmain.c -ffreestanding -fno-pie -mgeneral-regs-only -fstack-protector -o obj/kmain.o
	gcc -m32 -c src/x86_64/kernel/drivers/impl/VGA.c -ffreestanding -fno-pie -o obj/vga.o
	gcc -m32 -c src/x86_64/kernel/util/impl/strings.c -ffreestanding -fno-pie -o obj/strings.o
	gcc -m32 -c src/x86_64/kernel/drivers/impl/IO.c -ffreestanding -fno-pie -fstack-protector -o obj/io.o
	gcc -m32 -c src/x86_64/kernel/sys/impl/autostart.c -ffreestanding -fno-pie -fstack-protector -o obj/autostart.o
	gcc -m32 -c src/x86_64/kernel/drivers/impl/keyboard.c -ffreestanding -fno-pie -fstack-protector -o obj/keyboard.o
	gcc -m32 -c src/x86_64/kernel/sys/impl/kernelShell.c -ffreestanding -fno-pie -fstack-protector -o obj/kshell.o
	# gcc -m32 -c src/x86_64/kernel/hardware/impl/Interrupts.c -ffreestanding -fno-pie -o obj/interrupts.o
	nasm -felf src/x86_64/kernel/kasm.S -o obj/kasm.o
	i686-elf-ld -Tlink.ld obj/kasm.o obj/kmain.o obj/vga.o obj/strings.o obj/interrupts.o obj/syscalls.o \
		obj/io.o --oformat binary obj/autostart.o obj/keyboard.o obj/kshell.o -o bin/kernel.bin
	nasm -fbin src/x86_64/boot/bootloader.S -o bin/bootloader.bin
	cat bin/bootloader.bin bin/kernel.bin > bin/SkydeOS.bin
	@ # Prepare the image.
	sudo dd if=/dev/zero of=SkydeOS.img bs=1024 count=1440
	@ # Put the OS stuff in the image.
	sudo dd if=bin/SkydeOS.bin of=SkydeOS.img

burn:
	sudo dd if=SkydeOS.img of=/dev/sdb

run:
	sudo qemu-system-x86_64 -fda SkydeOS.img -monitor stdio

runusb:
	sudo qemu-system-x86_64 -hda /dev/sdb -monitor stdio
