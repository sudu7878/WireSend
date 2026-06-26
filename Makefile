#Makefile ver0.0.1

BUILD = builds/TestBuilds

	
all:
	g++ -Iheaders -std=c++17 src/CommunMod.cpp src/api.cpp src/ClientMod.cpp src/ServerMod.cpp src/UserHandler.cpp -o wires -L. nimlib.so


	
