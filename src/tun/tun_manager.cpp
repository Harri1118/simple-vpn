#include <sys/ioctl.h>
#include <fcntl.h>   // For file operations like open()
#include <unistd.h>  // For file operations like close()
#include <stdio.h>   // For standard I/O operations (like perror)
#include <errno.h>   // For error handling
#include <iostream>
#include <iostream>

static int fcntlRef = open("/dev/net/tun", 00070 ,O_RDWR);

int initialize(){
	while(fcntlRef == 0){
		// Create tunnel here.
		std::cout << "Failed. Retrying...\n";
		fcntlRef = open("/dev/net/tun", O_RDWR);
	}
	std::cout << "OP Number: " << fcntlRef << "\n";
	
	//fcntl::open();
	return 0;
}

void read(){
	// Read from tun
}

void write(){
	// Write to tun
}
