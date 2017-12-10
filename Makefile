.PHONY: control main

all: control main

control:
	gcc -o control control.c

main:
	gcc -o main main.c

clean:
	rm main
	rm control
