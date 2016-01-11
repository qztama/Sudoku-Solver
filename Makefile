#Qingwei Zeng, qizeng@ucdavis.edu

ss.out: ss.o puzzle.o
	g++ -Wall -o ss.out ss.o puzzle.o

puzzle.o: puzzle.cpp puzzle.h
	g++ -Wall -g -c puzzle.cpp

ss.o: ss.cpp puzzle.h
	g++ -Wall -g -c ss.cpp

clean:
	rm -f ss.out puzzle.o ss.o
