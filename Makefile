all: compile link

compile: main.o

main.o: main.cpp
	g++ -c main.cpp -Isrc/include

link: main.o
	g++ main.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

clean:
	rm main.o main.exe