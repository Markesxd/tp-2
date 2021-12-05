all:
	gcc main.c -o main

debug:
	gcc -g main.c -o main

clear:
	rm main
