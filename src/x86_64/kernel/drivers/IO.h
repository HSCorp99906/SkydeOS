#ifndef IO_H
#define IO_H

unsigned char portByteIn(unsigned short port);
void portByteOut(unsigned short port, unsigned char data);
void io_wait();

#endif
