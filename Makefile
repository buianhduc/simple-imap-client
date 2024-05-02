CFLAGS = -Wall -g -std=c17

SRC = main.c communication.c utils.c dynamicString.c
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