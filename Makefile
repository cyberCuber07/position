build:
	g++ main.cpp -o main src/*.cpp `pkg-config --cflags --libs opencv`

clear:
	rm main

run:
	./main maps/c
