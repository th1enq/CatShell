run: build
	./bin/main

build:
	mkdir -p bin
	gcc src/*.c -o bin/main