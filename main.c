#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DELAY 120000 // Kecepatan blok jatuh (mikrodetik)

typedef struct
{
    int x, y;
} Block;

void draw_borders(WINDOW *screen)
{
    int x, y, i;

    getmaxyx(screen, y, x);

    // 4 corners
    mvwprintw(screen, 0, 0, "+");
    mvwprintw(screen, y - 1, 0, "+");
    mvwprintw(screen, 0, x - 1, "+");
    mvwprintw(screen, y - 1, x - 1, "+");

    // sides
    for (i = 1; i < (y - 1); i++)
    {
        mvwprintw(screen, i, 0, "|");
        mvwprintw(screen, i, x - 1, "|");
    }

    // top and bottom
    for (i = 1; i < (x - 1); i++)
    {
        mvwprintw(screen, 0, i, "-");
        mvwprintw(screen, y - 1, i, "-");
    }
}

int main()
{
    int parent_x, parent_y, new_x, new_y;
    int score_size = 3;

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);  // Mengaktifkan input tombol panah
    nodelay(stdscr, TRUE); // Non-blocking input

    // Set up initial windows
    getmaxyx(stdscr, parent_y, parent_x);
    WINDOW *field = newwin(parent_y - score_size, parent_x, 0, 0);
    WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0);

    draw_borders(field);
    draw_borders(score);

    // Inisialisasi game
    int player_x = parent_x / 2;
    int player_y = parent_y - score_size - 2;
    int ch, game_over = 0, score_value = 0;
    Block block = {rand() % (parent_x - 2) + 1, 1};

    srand(time(0)); // Seed random

    while (!game_over)
    {
        getmaxyx(stdscr, new_y, new_x);

        // Perbarui ukuran jendela jika terminal berubah
        if (new_y != parent_y || new_x != parent_x)
        {
            parent_x = new_x;
            parent_y = new_y;

            wresize(field, new_y - score_size, new_x);
            wresize(score, score_size, new_x);
            mvwin(score, new_y - score_size, 0);

            wclear(stdscr);
            wclear(field);
            wclear(score);

            draw_borders(field);
            draw_borders(score);

            // Reset posisi pemain
            player_x = parent_x / 2;
            player_y = parent_y - score_size - 2;
        }

        // Input pemain
        ch = getch();
        if (ch == KEY_LEFT && player_x > 1)
        {
            player_x -= 2; // Pergerakan lebih cepat
        }
        else if (ch == KEY_RIGHT && player_x < parent_x - 2)
        {
            player_x += 2; // Pergerakan lebih cepat
        }

        // Pergerakan blok
        block.y++;
        if (block.y >= parent_y - score_size - 1)
        {
            // Gagal menangkap
            game_over = 1;
        }

        // Deteksi tangkapan
        if (block.y == player_y && block.x <= player_x)
        {
            block.y = 1;
            block.x = rand() % (parent_x - 2) + 1;
            score_value++; // Tambahkan skor
        }

        // Gambar jendela permainan
        wclear(field);
        draw_borders(field);
        mvwprintw(field, player_y, player_x, "==="); // Pemain
        mvwprintw(field, block.y, block.x, "#");     // Blok jatuh

        // Gambar jendela skor
        wclear(score);
        draw_borders(score);
        mvwprintw(score, 1, 1, "Score: %d", score_value);

        // Refresh semua jendela
        wrefresh(field);
        wrefresh(score);

        usleep(DELAY); // Tunggu sebelum frame berikutnya
    }

    // Game over
    wclear(field);
    draw_borders(field);
    mvwprintw(field, parent_y / 2 - score_size, parent_x / 2 - 6, "GAME OVER!");
    mvwprintw(field, parent_y / 2 - score_size + 1, parent_x / 2 - 8, "Final Score: %d", score_value);
    mvwprintw(field, parent_y / 2 - score_size + 3, parent_x / 2 - 12, "Press any key to exit...");
    wrefresh(field);
    getch();

    endwin();
    return 0;
}
