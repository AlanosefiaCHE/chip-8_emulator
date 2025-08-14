INCLUDES= -I ./include
FLAGS= -g

OBJECTS= ./build/chip8memory.o ./build/chip8stack.o ./build/chip8keyboard.o
all:	${OBJECTS}
	gcc  ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/main

./build/chip8memory.o: ./src/chip8memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8memory.c -c -o ./build/chip8memory.o
	
./build/chip8stack.o: ./src/chip8stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8stack.c -c -o ./build/chip8stack.o

./build/chip8keyboard.o: ./src/chip8keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8keyboard.c -c -o ./build/chip8keyboard.o

clean:
	@read -p "Weet je zeker dat je alles wilt verwijderen in ./build? [y/N] " ans && \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		rm -f ./build/* ./bin/*; \
		echo "Verwijderd!"; \
	else \
		echo "Afgebroken."; \
	fi
