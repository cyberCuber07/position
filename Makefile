build:
	g++ main.cpp -o main src/*.cpp `pkg-config --cflags --libs opencv` -lboost_system -lboost_filesystem -Wall

clear:
	rm main

run:
	./main ../predicted_images/IMG_6298.txt
