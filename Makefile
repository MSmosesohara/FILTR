CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -lncurses -lpanel
SRC = src/main.c src/panel_left.c src/panel_right.c src/highlight.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = ncurses-log-viewer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean