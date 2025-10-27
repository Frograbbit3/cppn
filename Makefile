main:
	@mkdir -p bin
	@g++ main.cpp -Iinclude -o bin/main.o --std=c++23 -fmodules-ts  -lSDL2 -lSDL2main
	@chmod +x bin/main.o

run:
	@make main
	@./bin/main.o