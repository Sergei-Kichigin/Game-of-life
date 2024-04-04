#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLS 80

int inputFromFile(int **board);
void printBoard(int **board);
void nextGeneration(int **board);

int **allocateDynamicMatrix1(int rows, int cols);

int main() {
    initscr();
    int **board = NULL;
    board = allocateDynamicMatrix1(ROWS, COLS);
    if (inputFromFile(board) == 0) {
        refresh();
        printw("...press any key to exit\n");
        getch();

        endwin();

        for (int i = 0; i < ROWS; i++) {
            free(board[i]);
        }

        free(board);
        return 0;
    }
    cbreak();

    scrollok(stdscr, TRUE);
    int game = 1;
    int delay = 100000;
    int paused = 0;
    nodelay(stdscr, TRUE);

    while (game) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            game = 0;
        } else if (ch == '=') {
            if (delay > 10000) {
                delay -= 100000;
            }
        } else if (ch == '-') {
            delay += 100000;
        } else if (ch == 'p' || ch == 'P') {
            paused = !paused;
        }

        if (!paused) {
            printBoard(board);
            nextGeneration(board);
            refresh();
            usleep(delay);
        }
    }

    endwin();

    for (int i = 0; i < ROWS; i++) {
        free(board[i]);
    }

    free(board);
    return 0;
}

int inputFromFile(int **board) {
    char filename[20];
    printw(">> input filename: ");
    scanw("%s", filename);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printw("cannot open file: %s\n", filename);
        return 0;
    }

    int flag = 1;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int c = fgetc(fp);
            if (c == EOF) {
                printw("cannot read file: %s\n", filename);
                flag = 0;
                break;
            }
            board[i][j] = (c == '0') ? 0 : 1;
        }

        int c = fgetc(fp);
        if (c == EOF || c != '\n') {
            printw("cannot read file: %s\n", filename);
            flag = 0;
            break;
        }
    }

    fclose(fp);
    return flag;
}

int **allocateDynamicMatrix1(int rows, int cols) {
    int **matrix = calloc(rows, sizeof(int *));
    if (matrix == NULL) {
        perror("n/a");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)calloc(cols, sizeof(int));
        if (matrix[i] == NULL) {
            perror("n/a");
            exit(1);
        }
    }
    return matrix;
}

void printBoard(int **board) {
    clear();
    for (int i = 0; i <= COLS + 1; i++) {
        printw("-");
    }
    printw("\n");

    for (int i = 0; i < ROWS; i++) {
        printw("|");
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 1) {
                printw("O");
            } else {
                printw(" ");
            }
        }
        printw("|");
        printw("\n");
    }

    for (int i = 0; i <= COLS + 1; i++) {
        printw("-");
    }

    usleep(100000);
}

void nextGeneration(int **board) {
    int newBoard[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int neighbors = 0;

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (x == 0 && y == 0) {
                        continue;  // find neighbors 0 0 != neighbor
                    }
                    int newX = (i + x + ROWS) % ROWS;
                    int newY = (j + y + COLS) % COLS;
                    neighbors += board[newX][newY];
                }
            }

            if (board[i][j] == 1 && (neighbors < 2 || neighbors > 3)) {
                newBoard[i][j] = 0;  // Клетка умирает от одиночества или перенаселения
            } else if (board[i][j] == 0 && neighbors == 3) {
                newBoard[i][j] = 1;  // Мертвая клетка становится живой, если у нее 3 соседа
            } else {
                newBoard[i][j] = board[i][j];  // Клетка остается в том же состоянии
            }
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = newBoard[i][j];
        }
    }
}
