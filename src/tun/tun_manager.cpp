#include <sys/ioctl.h>
#include <fcntl.h>   // For file operations like open()
#include <unistd.h>  // For file operations like close()
#include <stdio.h>   // For standard I/O operations (like perror)
#include <errno.h>   // For error handling
#include <iostream>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <cstring>
#include <format>
#include <string>
#include <linux/string.h>

int initialize(){
char dev[IFNAMSIZ] = "tun%d";
struct ifreq ifr;
int fd, err;
if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) 
	return -1; 
/* Flags: IFF_TUN - TUN device (no Ethernet headers) * IFF_TAP - TAP device * * IFF_NO_PI - Do not provide packet information */ 
ifr.ifr_flags = IFF_TUN | IFF_NO_PI; 
if( *dev ){ 
	std::strncpy(ifr.ifr_name, dev, IFNAMSIZ); 
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
} 
if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){ 
	close(fd);	
	return err; 
} 
strcpy(dev, ifr.ifr_name);
std::cout << fd << "\n";
return fd;
}

void read(){
	// Read from tun
}

void write(){
	// Write to tun
}
