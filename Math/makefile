CC=gcc
BIN=bin/
SRC=src/
COMMON_OPTS=-Iinc/ -o $@ $(DEBUG)
BIN_OPTS=$(COMMON_OPTS) -c $^
PROG_OPTS=$(COMMON_OPTS) $^
ALL_DEPENDS=$(BIN)main.o $(BIN)parser.o $(BIN)lex.o $(BIN)tok.o $(BIN)status.o $(BIN)string_t.o

interp: $(ALL_DEPENDS)
	$(CC) $(PROG_OPTS)

$(BIN)main.o: $(SRC)main.c
	$(CC) $(BIN_OPTS)

$(BIN)parser.o: $(SRC)parser.c
	$(CC) $(BIN_OPTS)

$(BIN)lex.o: $(SRC)lex.c
	$(CC) $(BIN_OPTS)

$(BIN)tok.o: $(SRC)tok.c
	$(CC) $(BIN_OPTS)

$(BIN)status.o: $(SRC)status.c
	$(CC) $(BIN_OPTS)

$(BIN)string_t.o: $(SRC)string_t.c
	$(CC) $(BIN_OPTS)

