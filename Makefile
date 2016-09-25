# the variable CC is the compiler to use
CC=g++ -std=c++11
# compiler flags
CFLAGS= -g -c -Wall
# linker flags
LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc

all: encode decode filter fancyBoard

encode: steganography/encode.o
	$(CC) $(LDFLAGS) steganography/encode.o -o steganography/encode

decode: steganography/decode.o
	$(CC) $(LDFLAGS) steganography/decode.o -o steganography/decode

filter: filter/filter.o filter/kernel.o
	$(CC) $(LDFLAGS) filter/filter.o filter/kernel.o -o filter/filter

fancyBoard: fancyBoard/fancyBoard.o
	$(CC) $(LDFLAGS) fancyBoard/fancyBoard.o -o fancyBoard/fancyBoard

encode.o: steganography/encode.cpp
	$(CFLAGS) steganography/encode.cpp -o steganography/encode.o

decode.o: steganography/decode.cpp
	$(CC) $(CFLAGS) steganography/decode.cpp -o steganography/decode.o

filter.o: filter/filter.cpp
	$(CC) $(CFLAGS) filter/filter.cpp -o filter/filter.o

kernel.o: filter/kernel.cpp
	$(CC) $(CFLAGS) filter/kernel.cpp -o filter/kernel.o

fancyBoard.o: fancyBoard/fancyBoard.cpp
	$(CC) $(CFLAGS) fancyBoard/fancyBoard.cpp -o fancyBoard/fancyBoard.o

clean:
	rm -f steganography/*o filter/*o fancyBoard/*o steganography/encode steganography/decode filter/filter fancyBoard/fancyBoard core
