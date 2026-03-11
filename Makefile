CXX = g++
CXXXFLAGS = -std=c++20 -Wall

program: main.o src/crypto/crypto_engine.o src/tun/tun_manager.o
	$(CXX) $(CXXFLAGS) main.o src/crypto/crypto_engine.o src/tun/tun_manager.o -o program
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp src/crypto/crypto_engine.cpp

crypto_engine.o: src/crypto/crypto.cpp
	$(CXX) $(CXXFLAGS) -c src/crypto/crypto_engine.cpp

tun_manager.o: src/tun/tun_manager.cpp
	$(CXX) $(CXXFLAGS) -c src/tun/tun_manager.cpp
clean:
	rm -f *.o program
	rm -f main.o
	rm -f crypto_engine.o
	rm -f tun_manager.o
