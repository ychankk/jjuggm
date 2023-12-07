#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP      0
#define DIR_DOWN   1
#define DIR_LEFT   2
#define DIR_RIGHT   3
#define DIR_SPACE   4

void jebi_init(void);
void jebi_manual(key_t key);
void jebi_dialog(int p, char message[]);
void live_init(void);
void death_init(void);
int selected = 0;
int deathcount;
bool live[PLAYER_MAX];
int now=0;
int fail=0;

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];

void jebi_init(void) {
    map_init(5, (n_alive * 3) + 2);
    int x, y, n = 0;

    for (int i = 0; i < n_alive; i++) {

        x = 2;
        y = 2 + (i * 3);

        if (i == 0) {
            px[0] = x;
            py[0] = y;
            back_buf[px[0]][py[0]] = '@';
        }
        else {
            px[i] = x;
            py[i] = y;
            back_buf[px[0]][py[0]] = '?';
        }
    }

    tick = 0;
}
void live_init(void) {
    map_init(5, (n_alive * 3) + 2);
    int x, y;
    for (int i = 0; i < n_alive; i++) {

        x = 2;
        y = 2 + (i * 3);
        if (i == 0) {
            px[0] = x;
            py[0] = y;
            back_buf[px[0]][py[0]] = '@';
        }
        else {
            px[i] = x;
            py[i] = y;
            back_buf[px[i]][py[i]] = '?';
        }
    }
}
void death_init(void) {
    map_init(5, (n_alive * 3) + 2);
    int x, y;
    fail = randint(0, n_alive - 1);
    if (n_alive > 1) {
        fail = randint(0, n_alive - 1);
    }
    for (int i = 0; i < n_alive; i++) {

        x = 2;
        y = 2 + (i * 3);
        if (i == 0) {
            px[0] = x;
            py[0] = y;
            back_buf[px[0]][py[0]] = '@';
        }
        else {
            px[i] = x;
            py[i] = y;
            back_buf[px[i]][py[i]] = '?';
        }
    }

    tick = 0;
}


bool jebi_placable(int row, int col) {
    if (row < 0 || row >= N_ROW ||
        col < 0 || col >= N_COL ||
        back_buf[row][col] == ' ') {
        return false;
    }
    return true;
}
bool jebi_placable3(int row, int col) {
    if (row < 0 || row >= N_ROW ||
        col < 0 || col >= N_COL ||
        back_buf[row][col] == '?') {
        return false;
    }
    return true;
}

bool jebi_placable2(int col) {
    if (back_buf[col] == ' ') {
        return false;
    }
    return true;
}

void jebi_manual(key_t key) {
    static int dy[4] = { 0, 0, -3, 3 };

    int dir;
    switch (key) {
    case K_SPACE: dir = DIR_SPACE; break;
    case K_LEFT: dir = DIR_LEFT; break;
    case K_RIGHT: dir = DIR_RIGHT; break;
    default: return -1;
    }

    if (dir != 4) {
        int nx, ny;
        nx = px[0];
        ny = py[0] + dy[dir];
        if (!jebi_placable(nx, ny)) {
            return;
        }
        else {
            if (dir == 2) {
                now--;
            }
            if (dir == 3) {
                now++;
            }
        }
        
        if (jebi_placable3) {
            back_buf[px[0]][py[0]] = '?';
            px[0] = nx;
            py[0] = ny;
            back_buf[px[0]][py[0]] = '@';
        }
        
    }
    else {
            back_buf[px[0]][py[0]] = ' ';
            if (now == fail) {
                jebi_dialog(selected, "탈락!");
                player[selected].is_alive = false;
                n_alive--;
                now = 0;
                if (player[0].is_alive) {
                    selected = 0; // 다시 처음부터
                }
                else {
                    for (int i = 0; i < n_player; i++) {
                        if (player[i].is_alive == TRUE) {
                            selected = i;
                            break;
                        }
                    }
                }
                death_init();
                system("cls");
                
            }
            else {
                jebi_dialog(selected, "통과!");
                selected++; // 다음 플레이어
                now = 0;
                live_init();
                system("cls");
            }
        
    }


    return 0;

}

void jebi_dialog(int p, char message[]) {
    int i, j;
    size_t messageLength = strlen(message);

    for (int i = 1; i >= 0; i--) {
        gotoxy(5, 3);

        for (int j = 0; j < 16; j++) {
            printf("*");
        }

        gotoxy(6, 2);
        printf("* %d player %s *", p, message);

        gotoxy(7, 3);
        for (int j = 0; j < 16; j++) {
            printf("*");
        }

        Sleep(300);
    }
}

void jebi(void) {
    jebi_init();
    system("cls");
    display();
    int jebi[PLAYER_MAX] = { 0 };
    int y, s = 0, n = 0;
    fail = randint(0, n_alive - 1); // 각 라운드 시작 시 미리 fail이 될 제비 결정

    while (1) {
        display();
        if (s == 0) {
            for (int i = 0; i < n_alive; i++) {
                y = 2 + (i * 3);
                jebi[i] = y;
                if (i == 0) back_buf[px[0]][y] = '@';
                else back_buf[px[0]][y] = '?';
                s = 1;
            }
        }
        key_t key = get_key();
        if (key == K_QUIT) {
            break;
        }

        
        else if (key != K_UNDEFINED) {
            jebi_manual(key);
        }
        display();
        Sleep(100);
        tick += 10;
        if (n_alive <= 1) {
            break;
        }
    }
}
