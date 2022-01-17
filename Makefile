all:
	@ ##
	@ # Buildscruot for PickleOS.
	@ #
	@ # @Makefile
	@ # @version 1.0
	nasm -felf src/x86_64/kernel/kasm.S -o bin/kasm.o
	i686-elf-ld -Tlink.ld obj/kasm.o --oformat binary -o bin/kasm.bin
	nasm -fbin src/x86_64/boot/bootloader.S -o bin/bootloader.bin
	cat bin/bootloader.bin bin/kasm.bin > bin/SkydeOS.bin
	@ # Prepare the image.
	sudo dd if=/dev/zero of=SkydeOS.img bs=1024 count=1440
	@ # Put the OS stuff in the image.
	sudo dd if=bin/SkydeOS.bin of=SkydeOS.img
	sudo dd if=SkydeOS.img of=/dev/sdb


run:
	sudo qemu-system-x86_64 -hda /dev/sdb -monitor stdio
