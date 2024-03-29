all: sokomethod
#包含的头文件的目录
INCLUDE = ./include
#输出文件的目录
OUTPUT = ./output
#源文件的目录
SRC = ./src
sokomethod: $(OUTPUT)/main.o $(OUTPUT)/sokopush.o $(OUTPUT)/soko_pos_operation.o $(OUTPUT)/soko_map_operation.o
	gcc -g `pkg-config --cflags --libs gtk+-2.0` -std=c99 -o sokomethod $(OUTPUT)/main.o $(OUTPUT)/sokopush.o $(OUTPUT)/soko_pos_operation.o $(OUTPUT)/soko_map_operation.o
$(OUTPUT)/main.o: $(SRC)/main.c $(INCLUDE)/sokopush.h $(INCLUDE)/debug.h
	gcc -g `pkg-config --cflags --libs gtk+-2.0` -std=c99 -I $(INCLUDE) -c $(SRC)/main.c -o $(OUTPUT)/main.o
$(OUTPUT)/sokopush.o: $(SRC)/sokopush.c $(INCLUDE)/sokopush.h $(INCLUDE)/debug.h
	gcc -g -std=c99 -I $(INCLUDE) -c $(SRC)/sokopush.c -o $(OUTPUT)/sokopush.o
$(OUTPUT)/soko_pos_operation.o: $(SRC)/soko_pos_operation.c $(INCLUDE)/sokopush.h $(INCLUDE)/soko_pos_operation.h $(INCLUDE)/debug.h
	gcc -g -std=c99 -I $(INCLUDE) -c $(SRC)/soko_pos_operation.c -o $(OUTPUT)/soko_pos_operation.o
$(OUTPUT)/soko_map_operation.o: $(SRC)/soko_map_operation.c $(INCLUDE)/sokopush.h $(INCLUDE)/soko_pos_operation.h $(INCLUDE)/debug.h
	gcc -g -std=c99 -I $(INCLUDE) -c $(SRC)/soko_map_operation.c -o $(OUTPUT)/soko_map_operation.o
