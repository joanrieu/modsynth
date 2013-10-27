synth: synth.cpp
	clang++ -std=c++11 -ggdb -o $@ $^

.PHONY: play clean

play: synth
	./synth | aplay -f FLOAT_LE -r 44100 -c 2

clean:
	rm -f synth
