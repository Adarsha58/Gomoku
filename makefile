all: gobang

gobang:: gobang.cpp
	g++ -o gobang gobang.cpp

clean:
	rm gobang *.o
