CXX = g++
CXXFLAGS = -Wall -std=c++11
ALL=main Mode_generator
OBJECT=main.o Mode_generator.o
all: main modegen

main: main.o
	$(CXX) $(CXXFLAGS) $^ -o $@
	mv $@ graphgen
	cp graphgen /usr/bin/

modegen: Mode_generator.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%*.o: %*.cpp
	$(CXX) $(CFLAGS) $^ -o $@

.PHONY:
	clean
clean:
	rm -rf $(ALL) $(OBJECT)
	rm /usr/bin/graphgen
