# SkydeOS
A hobby OS made by Ian Moffett.

### Kernel
SkydeOS uses KessKernel which is a kernel
that I am currently developing.

#### Syscalls

interrupt to IDT entry 0x79
which is a syscall dispatcher.

You would put the service number in 
EAX.

```
int 0x79 
```

Current services available:

- 0 - Test syscall.

### Interrupts for drivers


sys_restart

```
int 0x2A
```

red_screen.
Used for errors.

```
int 0xC8
```

### How do I get it working?

```
git clone https://github.com/HSCorp99906/SkydeOS ~/SkydeOS
cd ~/SkydeOS
make
make run
```

## Contact
Ian Moffett - ian@kesscoin.com\
Discord: ```[HSC] GreyHat#0070```
