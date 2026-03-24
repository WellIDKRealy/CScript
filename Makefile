run: main
	./test.c

main: main.c
	gcc main.c -o main -O0 -g

install: main
	sudo cp main /usr/local/bin/CScript

.PHONY: run install
