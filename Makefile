all:
	g++ -I/usr/include/python3.10 -lpython3.10 -std=c++11 -o analysis analysis.cpp -lstdc++fs

test:
	g++ -I/usr/include/python3.10 -lpython3.10 -g -std=c++11 -o analysis analysis.cpp -lstdc++fs

clean:
	rm analysis