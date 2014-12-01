INCLUDE=$(PWD)/include/
BIN=$(PWD)/bin
LIBRARIES=m
CURRENT_DIR=$(PWD)
SOURCE = $(BIN)/btree_main.c $(BIN)/btree_ops.c $(BIN)/file_ops.c $(BIN)/queue.c
SOURCE2 = $(BIN)/bin_to_text.c $(BIN)/btree_ops.c $(BIN)/file_ops.c $(BIN)/queue.c

all: assn_4

debug:$(INCLUDE) $(SOURCE)
		gcc -std=c99 -Wall -g $(SOURCE) -o debug_assn4 -D DEBUG_PRINT -I $(INCLUDE) -l$(LIBRARIES)

assn_4:$(INCLUDE) $(SOURCE)
		gcc -std=c99 -Wall  $(SOURCE) -o assn_4  -I $(INCLUDE) -l$(LIBRARIES)

readbin:$(INCLUDE) $(SOURCE2)
		gcc -std=c99 -Wall -g $(SOURCE) -o readbin -D DEBUG_PRINT -I $(INCLUDE) -l$(LIBRARIES)
clean:
		rm assn_4 debug_assn4 index.bin
