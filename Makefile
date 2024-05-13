CFLAGS = -Wall -g -std=c17

SRC = main.c communication.c utils.c email_handle.c dynamic_string.c
OBJ = $(SRC:.c=.o)

EXE = fetchmail
LIBS += -lm -lc

$(EXE): $(OBJ)
	cc $(CFLAGS) -o $(EXE) $(OBJ) $(LIBS)

main.o = main.c

all: $(EXE)

clean:
	rm -f $(OBJ) $(EXE)
run: all
	@./$(EXE)