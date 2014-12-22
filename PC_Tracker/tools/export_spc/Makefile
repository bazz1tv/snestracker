all: ../bin/export_spc

../bin/export_spc: main.o
	g++ main.o -o ../bin/export_spc

main.o: main.cpp
	g++ -c main.cpp

factorial.o: factorial.cpp
	g++ -c factorial.cpp

hello.o: hello.cpp
	g++ -c hello.cpp

clean:
	rm -rf *o ../bin/export_spc