all: sokomethod
#包含的头文件的目录
INCLUDE = ./include
#输出文件的目录
OUTPUT = ./output
#源文件的目录
SRC = ./src
sokomethod: $(OUTPUT)/main.o $(OUTPUT)/sokopush.o $(OUTPUT)/soko_pos_operation.o
	gcc -std=c99 -o sokomethod $(OUTPUT)/main.o $(OUTPUT)/sokopush.o $(OUTPUT)/soko_pos_operation.o
$(OUTPUT)/main.o: $(SRC)/main.c $(INCLUDE)/sokopush.h
	gcc -std=c99 -I $(INCLUDE) -c $(SRC)/main.c -o $(OUTPUT)/main.o
$(OUTPUT)/sokopush.o: $(SRC)/sokopush.c $(INCLUDE)/sokopush.h
	gcc -std=c99 -I $(INCLUDE) -c $(SRC)/sokopush.c -o $(OUTPUT)/sokopush.o
$(OUTPUT)/soko_pos_operation.o: $(SRC)/soko_pos_operation.c $(INCLUDE)/sokopush.h $(INCLUDE)/soko_pos_operation.h
	gcc -std=c99 -I $(INCLUDE) -c $(SRC)/soko_pos_operation.c -o $(OUTPUT)/soko_pos_operation.o
