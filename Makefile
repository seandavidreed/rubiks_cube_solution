CC = g++
CFLAGS = -Wall -Werror

rubikscube: main.o rubik.o
	$(CC) $(CFLAGS) -o bin/rubikscube build/main.o build/rubik.o

main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o

rubik.o: src/rubik.cpp
	$(CC) $(CFLAGS) -c src/rubik.cpp -o build/rubik.o

clean:
	$(RM) build/*.o bin/rubikscube