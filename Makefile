example: example.cpp messagequeue.hpp
	g++ example.cpp -o example -std=c++11 -lpthread

.PHONY: clean
clean:
	rm -f example
