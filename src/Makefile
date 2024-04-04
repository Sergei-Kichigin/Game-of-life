
CC = gcc
CFLAGS = -Wall -Wextra -Werror 
LIBS = -lncurses

SRC_FILES = game_of_life.c
OBJ_FILES = $(SRC_FILES:.c=.o)

EXECUTABLE = Game_Of_Life

SRC_CODE_STYLE = ../materials/linters/.clang-format
VALG = valgrind --tool=memcheck --leak-check=yes --log-file=valgrind_output.txt
CPPCH = cppcheck --enable=all --suppress=missingIncludeSystem --suppress=invalidFunctionArg --force

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

valgrind: $(EXECUTABLE)
	$(VALG) ./$<

cpp_check: $(SRC_FILES)
	$(CPPCH) $^

clang_all:
	cp $(SRC_CODE_STYLE) .clang-format
	clang-format -i *.c
	rm -f .clang-format

clean:
	rm -f $(EXECUTABLE) *.o valgrind_output.txt

rebuild: clean all

.PHONY: all clean rebuild valgrind cpp_check clang_all
