#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/**
 * Constants
 */
#define DELAY 80000
#define PLAYER_WIDTH 3
#define BALL_SYMBOL 'O'
#define HIGHSCORE_FILE "highscore.txt"

const char game_over_char[][500] = {
    "  ____                         ___                 _ ",
    " / ___| __ _ _ __ ___   ___   / _ \__   _____ _ __| |",
    "| |  _ / _` | '_ ` _ \ / _ \ | | | \ \ / / _ \ '__| |",
    "| |_| | (_| | | | | | |  __/ | |_| |\ V /  __/ |  |_|",
    " \____|\__,_|_| |_| |_|\___|  \___/  \_/ \___|_|  (_)",
};

/**
 * Variables and types
 */

typedef struct
{
    int x, y;
} Object;

/**
 * Prototypes
 */

void setup_window();
void draw_borders(WINDOW *win);
int read_highscore();
void save_highscore(int score);
void game_over(int score);