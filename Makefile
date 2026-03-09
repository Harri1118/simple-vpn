CXX = g++
CXXXFLAGS = -std=c++20 -Wall

program: main.o src/crypto/crypto_engine.o
	$(CXX) $(CXXFLAGS) main.o src/crypto/crypto_engine.o -o program
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp src/crypto/crypto_engine.cpp

crypto_engine.o: src/crypto/crypto.cpp
	$(CXX) $(CXXFLAGS) -c src/crypto/crypto_engine.cpp

clean:
	rm -f *.o program
	rm -f main.o
	rm -f crypto_engine.o
