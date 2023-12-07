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
void jebi_tail(int i, int nx, int ny);
int selected = 0; // player 0부터 시작
int fail;

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

void jebi_init(void) {
    map_init(5, (n_alive * 3) + 2);
    int x, y, n = 0;
    for (int i = 0; i < n_player; i++) {

        x = 2;
        y = 2 + (n * 3);

        PLAYER* p = &player[i];
        if (p->is_alive == true) {
            n++;
            px[i] = x;
            py[i] = y;

            back_buf[px[i]][py[i]] = '?';  // (0 .. n_player-1)
        }
        if (i == 0 && p->is_alive == true) {
            back_buf[px[0]][py[0]] = '@';
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

bool jebi_placable2(int col) {
    if (back_buf[col] == ' ') {
        return false;
    }
    return true;
}

// back_buf[][]에 기록
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
        // 움직여서 놓일 자리
        int nx, ny;
        nx = px[0];
        ny = py[0] + dy[dir];
        if (!jebi_placable(nx, ny)) {
            return;
        }

        // '@'를 이동
        back_buf[px[0]][py[0]] = '?';
        px[0] = nx;
        py[0] = ny;
        back_buf[px[0]][py[0]] = '@';
    }
    else {
        if (py[0] == py[selected]) {
            // 제비를 뽑았으므로 뽑은 제비 제거
            back_buf[px[0]][py[selected]] = ' ';
            if (selected == fail) {
                jebi_dialog(selected, "탈락!");
                player[selected].is_alive = false; // 플레이어 상태 업데이트
                n_alive--;
                if (player[0].is_alive != TRUE) {
                    selected = 0; // 다시 처음부터
                }
                else {
                    selected++;
                }
                jebi_init();
                system("cls");
                fail = randint(0, n_alive - 1); // 다음 라운드의 fail이 될 제비 미리 결정
            }
            else {
                jebi_dialog(selected, "통과!");
                selected++; // 다음 플레이어
                if (selected >= n_alive) {
                    selected = 0; // 다시 처음부터
                    jebi_init();
                    system("cls");
                    fail = randint(0, n_alive - 1); // 다음 라운드의 fail이 될 제비 미리 결정
                }
            }
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
            printf("-");
        }

        gotoxy(6, 2);
        printf("| %d player %s |", p, message);

        gotoxy(7, 3);
        for (int j = 0; j < 16; j++) {
            printf("-");
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
    int fail = randint(0, n_alive - 1); // 각 라운드 시작 시 미리 fail이 될 제비 결정

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

        if (key == K_SPACE) {
            if (selected == fail) {
                jebi_dialog(selected, "탈락!");
                player[selected].is_alive = false; // 플레이어 상태 업데이트
                back_buf[px[0]][py[selected]] = ' '; // 제비 목록에서 제비 제거
                n_alive--;
                selected = 0; // 다시 처음부터
                jebi_init();
                system("cls");
                s = 0;
                fail = randint(0, n_alive - 1); // 다음 라운드의 fail이 될 제비 미리 결정
            }
            else {
                jebi_dialog(selected, "통과!");
                selected++; // 다음 플레이어
                if (selected >= n_alive) {
                    selected = 0; // 다시 처음부터
                    jebi_init();
                    system("cls");
                    s = 0;
                    fail = randint(0, n_alive - 1); // 다음 라운드의 fail이 될 제비 미리 결정
                }
            }
        }
        else if (key != K_UNDEFINED) {
            jebi_manual(key); // '@' 이동
        }

        display();
        Sleep(100);
        tick += 10;
        if (n_alive <= 1) {
            break;
        }
    }
}