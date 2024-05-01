CFLAGS = -Wall -g -std=c11

SRC = main.c
OBJ = $(SRC:.c=.o)

EXE = fetchmail
LIBS += -lm

$(EXE): $(OBJ)
	cc $(CFLAGS) -o $(EXE) $(OBJ) $(LIBS)

main.o = main.c

all: $(EXE)

clean:
	rm -f $(OBJ) $(EXE)
run: all
	@./$(EXE)