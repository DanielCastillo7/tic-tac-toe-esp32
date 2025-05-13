#include <stdio.h>
#include <stdlib.h>

int get_player_move(char player_symbol) {
    int move = -1;
    printf("Player %c, enter your move (0-8): ", player_symbol);
    scanf("%d", &move);

    // Validate move is between 0 and 8
    while (move < 0 || move > 8) {
        printf("Invalid move. Try again (0-8): ");
        scanf("%d", &move);
    }

    return move;
}