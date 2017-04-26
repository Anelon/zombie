a.out: zombie.cc clazzes.h
	g++ -std=c++11 zombie.cc -lncurses
clean:
	rm a.out
