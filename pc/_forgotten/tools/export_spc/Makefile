all: ../bin/export_spc

../bin/export_spc: main.o
	g++ main.o -o ../bin/export_spc

main.o: main.cpp
	g++ -c main.cpp



clean:
	rm -rf *o ../bin/export_spc