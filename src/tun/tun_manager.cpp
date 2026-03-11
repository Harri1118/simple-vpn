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

int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
       return tun_alloc(dev);

    memset(&ifr, 0, sizeof(ifr));
    
    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TUN;
    if( *dev )
      std::snprintf(ifr.ifr_name, IFNAMSIZ, "%s", dev); 
    
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
       close(fd);
       return err;
    }
    strcpy(dev, ifr.ifr_name);
    return fd;
}

int tun_alloc_mq(char *dev, int queues, int *fds)
{
    struct ifreq ifr;
    int fd, err, i;

    if (!dev)
        return -1;

    memset(&ifr, 0, sizeof(ifr));
    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     *        IFF_MULTI_QUEUE - Create a queue of multiqueue device
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI | IFF_MULTI_QUEUE;
    strcpy(ifr.ifr_name, dev);

    for (i = 0; i < queues; i++) {
        if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
           goto err;
        err = ioctl(fd, TUNSETIFF, (void *)&ifr);
        if (err) {
           close(fd);
           goto err;
        }
        fds[i] = fd;
    }

    return 0;
err:
    for (--i; i >= 0; i--)
        close(fds[i]);
    return err;
}


int tun_set_queue(int fd, int enable)
{
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));

    if (enable)
       ifr.ifr_flags = IFF_ATTACH_QUEUE;
    else
       ifr.ifr_flags = IFF_DETACH_QUEUE;

    return ioctl(fd, TUNSETQUEUE, (void *)&ifr);
}


int initialize(){
	char tun_name = '1';
	char *tun_buff = &tun_name;
	std::cout << tun_alloc(tun_buff);
	return 0;
}

void read(){
	// Read from tun
}

void write(){
	// Write to tun
}
