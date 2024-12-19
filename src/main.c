#include "include/main.h"

void setup_window()
{
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
}

void draw_borders(WINDOW *win)
{
    int x, y;
    getmaxyx(win, y, x);

    // Gambar batas atas dan bawah
    for (int i = 0; i < x; i++)
    {
        mvwaddch(win, 0, i, '-');
        mvwaddch(win, y - 1, i, '-');
    }

    // Gambar batas samping
    for (int i = 0; i < y; i++)
    {
        mvwaddch(win, i, 0, '|');
        mvwaddch(win, i, x - 1, '|');
    }
}

int read_highscore()
{
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    int highscore = 0;
    if (file)
    {
        fscanf(file, "%d", &highscore);
        fclose(file);
    }
    return highscore;
}

void save_highscore(int score)
{
    FILE *file = fopen(HIGHSCORE_FILE, "w");
    if (file)
    {
        fprintf(file, "%d", score);
        fclose(file);
    }
}

void game_over(int score)
{
    int highscore = read_highscore();
    if (score > highscore)
    {
        highscore = score;
        save_highscore(highscore);
    }

    for (int blink = 0; blink < 5; blink++)
    {
        clear();
        if (blink % 2 == 0)
        {
            for (int i = 0; i < 5; i++)
            {
                mvprintw(LINES / 2 - 3 + i, (COLS - 50) / 2, "%s", game_over_char[i]);
            }
            mvprintw(LINES / 2 + 3, COLS / 2 - 10, "Final Score: %d", score);
            mvprintw(LINES / 2 + 4, COLS / 2 - 15, "Highest Score: %d", highscore);
            mvprintw(LINES / 2 + 5, COLS / 2 - 15, "Press any key to exit...");
        }
        refresh();
        usleep(500000);
    }
    getch();
    endwin();
    exit(0);
}

int main()
{
    setup_window();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Inisialisasi objek
    Object player = {max_x / 2, max_y - 2};
    Object ball = {rand() % (max_x - 2) + 1, 1};

    int score = 0;
    int ch;
    int blink_highscore = 0;

    // Seed untuk random
    srand(time(0));

    // Tampilkan skor tertinggi saat memulai
    int highscore = read_highscore();
    mvprintw(0, 2, "Highest Score: %d", highscore);
    refresh();
    // usleep(2000000);

    while (1)
    {
        clear();

        // Gambar batas game
        draw_borders(stdscr);

        // Gambar pemain
        mvprintw(player.y, player.x, "===");

        // Gambar bola
        mvprintw(ball.y, ball.x, "%c", BALL_SYMBOL);

        // Gambar skor
        mvprintw(0, 2, "Score: %d", score);
        if (score > highscore)
        {
            blink_highscore = !blink_highscore;
            if (blink_highscore)
            {
                attron(A_BLINK);
            }
            mvprintw(0, 20, "Highest Score: %d", score);
            if (blink_highscore)
            {
                attroff(A_BLINK);
            }
        }
        else
        {
            mvprintw(0, 20, "Highest Score: %d", highscore);
        }

        refresh();
        timeout(50);

        ch = getch();
        if (ch == KEY_LEFT && player.x > 1)
        {
            player.x -= 3;
        }
        else if (ch == KEY_RIGHT && player.x < max_x - PLAYER_WIDTH - 1)
        {
            player.x += 3;
        }

        usleep(DELAY);
        ball.y++;

        // Deteksi tabrakan
        if (ball.y == player.y && ball.x >= player.x && ball.x < player.x + PLAYER_WIDTH)
        {
            score++;
            ball.y = 1;

            // Pastikan jarak spawn bola tidak lebih dari 50px
            int new_ball_x;
            do
            {
                new_ball_x = rand() % (max_x - 2) + 1;
            } while (abs(new_ball_x - ball.x) > 50);
            ball.x = new_ball_x;
        }

        // Cek jika bola jatuh
        if (ball.y >= max_y - 1)
        {
            game_over(score);
        }
    }

    endwin();
    return 0;
}
