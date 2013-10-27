CXX=clang++
CXXFLAGS=-std=c++11 -ggdb
LIBFLAGS=-shared -fPIC

synth: main.cpp libmodsynth.so
	$(CXX) $(CXXFLAGS) -o $@ $^

libmodsynth.so: synth.cpp
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) -o $@ $^

.PHONY: clean

clean: synth libmodsynth.so
	rm -f $^
