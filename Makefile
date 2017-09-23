picodb: main.o third_party/linenoise.o picodebugger.o breakpoint.o
	$(CXX) -o picodb main.o third_party/linenoise.o picodebugger.o breakpoint.o -lelf++ -ldwarf++ -L./third_party/libelfin/elf -L./third_party/libelfin/dwarf

main.o: main.cpp
	$(CXX) -Wall -W -Os -g -c main.cpp -std=c++1y

picodebugger.o: picodebugger.cpp
	$(CXX) -Wall -W -Os -g -c picodebugger.cpp -std=c++1y

breakpoint.o: breakpoint.cpp
	$(CXX) -Wall -W -Os -g -c breakpoint.cpp -std=c++1y

linenoise.o: linenoise.c
	$(CC) -Wall -W -Os -g -c third_party/linenoise.c 

clean: 
	rm *.o
