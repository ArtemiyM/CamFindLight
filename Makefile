CC=g++
CFLAGS=-Wall -O -pipe -lwiringPi `pkg-config --cflags --libs opencv`
LDFLAGS=-lm `pkg-config --libs opencv`

OBJ=ball_detection

all: clean
	$(CC) main.cpp -o $(OBJ) $(CFLAGS) $(LDFLAGS) 

clean:
	rm -f ./*.o ./ball_detection

fix:
	ci -l *.c Makefile
#	ci -l *.h 

# $Id: Makefile,v 1.2 2014/09/23 14:15:42 mechanoid Exp $
