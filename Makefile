picodb: main.o linenoise.o picodebugger.o
	$(CXX) -o picodb main.o linenoise.o picodebugger.o

main.o: main.cpp
	$(CXX) -Wall -W -Os -g -c main.cpp -std=c++11

picodebugger.o: picodebugger.cpp
	$(CXX) -Wall -W -Os -g -c picodebugger.cpp -std=c++11

linenoise.o: linenoise.c
	$(CC) -Wall -W -Os -g -c linenoise.c 
