all: Main

Main: main.cpp
	g++ -std=c++11 main.cpp -o main

clean:
	rm *.o *.gch Main