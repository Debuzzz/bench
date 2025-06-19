CC = gcc
CFLAGS = -O3 -Wall -Wextra
TARGET = bin/mmul_bench
SRC = main.c

.PHONY: all clean

all: $(TARGET)

bin:
	mkdir -p bin

$(TARGET): $(SRC) | bin
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf bin
	deactivate || true
	rm -rf .venv