run: build
	./bin/main

build:
	mkdir -p bin
	gcc *.c -o bin/main