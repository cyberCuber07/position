build:
	g++ main.cpp -o _main src/*.cpp -lpthread `pkg-config --cflags --libs opencv` -lboost_system -lboost_filesystem -Wall

clear:
	rm _main

run:
	./_main ../predicted_images
