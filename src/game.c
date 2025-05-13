#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt_comm.h"

char board[3][3];

void print_board() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
        if (i < 2) printf("---|---|---\n");
    }
    printf("\n");
}

void reset_board() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = '1' + i * 3 + j;
}

int check_winner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return 1;
    return 0;
}

void play_turn(int player, char symbol) {
    int move;
    char msg[2];
    if (player == 1 || player == 2) {
        printf("Player %d (%c), enter move (1-9): ", player, symbol);
        scanf("%d", &move);
    } else {
        receive_move(msg); // From MQTT
        move = msg[0] - '0';
        printf("Auto Player move: %d\n", move);
    }
    move -= 1;
    int row = move / 3;
    int col = move % 3;
    board[row][col] = symbol;
    char buf[2]; buf[0] = symbol; buf[1] = '\0';
    publish_move(buf); // Publish to ESP32 or MQTT
}

int main() {
    mqtt_setup();

    int mode;
    printf("Select mode:\n");
    printf("1. 1-Player vs Bash\n");
    printf("2. 2-Player on terminal\n");
    printf("3. Auto Mode (C vs Bash)\n");
    printf("Enter choice: ");
    scanf("%d", &mode);

    reset_board();
    print_board();

    int turn = 0;
    while (turn < 9 && !check_winner()) {
        if (mode == 1) {
            play_turn(turn % 2 == 0 ? 1 : 3, turn % 2 == 0 ? 'X' : 'O'); // Player vs Bash
        } else if (mode == 2) {
            play_turn(turn % 2 + 1, turn % 2 == 0 ? 'X' : 'O'); // Human vs Human
        } else {
            play_turn(turn % 2 == 0 ? 0 : 3, turn % 2 == 0 ? 'X' : 'O'); // Auto C vs Bash
        }
        print_board();
        turn++;
    }

    if (check_winner())
        printf("Player %d wins!\n", turn % 2 == 0 ? 2 : 1);
    else
        printf("It's a draw!\n");

    return 0;
}