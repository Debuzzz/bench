CC = gcc
CFLAGS = -O3 -Wall -Wextra
TARGET = bin/mmul_bench
SRC = main.c

DOCKER_IMAGE = bench_env
DOCKER_TAG = latest

.PHONY: all clean docker notebook latex

all: docker $(TARGET) notebook latex

bin:
	mkdir -p bin

$(TARGET): $(SRC) | bin
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

docker:
	docker build -t $(DOCKER_IMAGE):$(DOCKER_TAG) .

notebook:
	docker run --rm -v $(PWD):/workspace $(DOCKER_IMAGE):$(DOCKER_TAG) \
		bash -c "cd /workspace && jupyter nbconvert --to html --execute notebook.ipynb"

latex:
	docker run --rm -v $(PWD):/workspace $(DOCKER_IMAGE):$(DOCKER_TAG) \
		bash -c "cd /workspace/latex && pdflatex rapport.tex"

clean:
	rm -rf bin
	rm -f notebook.html
	rm -f data/*.csv data/*.png
	rm -f latex/*.aux latex/*.log latex/*.out latex/*.pdf