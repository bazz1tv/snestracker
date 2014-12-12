SHELL := /bin/bash

all:
	gcc brr_encode.c -o brr_encode
	./brr_encode piano.wav snes_side/sound.brr
	gcc size.c -o size
	cd snes_side; make 
