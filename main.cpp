#include <iostream>
#include <thread>
#include <chrono>
#include "src/crypto/crypto_engine.h"
#include "src/tun/tun_manager.h"
//#include "src/crypto/crypto_engine.h" // Include the header to call the function

void threadFunction(){
	std::cout << "Hello from inner thread\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::thread::id this_id = std::this_thread::get_id();
}

int main() {
	initialize();
	std::thread newThread(threadFunction);
	std::thread newThread2(threadFunction);
	std::cout << "Hello from main thread\n";
	newThread.join();
	newThread2.join();
    return 0;
}

