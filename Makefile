all:
	g++  -std=c++11 -o analysis analysis.cpp -lstdc++fs -I/usr/include/python3.10 -lpython3.10

debug:
	g++ -I/usr/include/python3.10 -lpython3.10 -g -std=c++11 -o analysis analysis.cpp -lstdc++fs

clean:
	rm analysis
