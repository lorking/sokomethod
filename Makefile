all: sokomethod
#包含的头文件的目录
INCLUDE = ./include
#输出文件的目录
OUTPUT = ./output
#源文件的目录
SRC = ./src
sokomethod: $(OUTPUT)/main.o $(OUTPUT)/sokopush.o
	gcc -std=c99 -o sokomethod $(OUTPUT)/main.o $(OUTPUT)/sokopush.o
$(OUTPUT)/main.o: $(SRC)/main.c $(INCLUDE)/sokopush.h
	gcc -std=c99 -I $(INCLUDE) -c $(SRC)/main.c -o $(OUTPUT)/main.o
$(OUTPUT)/sokopush.o: $(SRC)/sokopush.c
	gcc -std=c99 -I $(INCLUDE) -c $(SRC)/sokopush.c -o $(OUTPUT)/sokopush.o
