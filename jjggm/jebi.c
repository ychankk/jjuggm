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
int selected = 0; // player 0���� ����
int fail;

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�

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

// back_buf[][]�� ���
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
        // �������� ���� �ڸ�
        int nx, ny;
        nx = px[0];
        ny = py[0] + dy[dir];
        if (!jebi_placable(nx, ny)) {
            return;
        }

        // '@'�� �̵�
        back_buf[px[0]][py[0]] = '?';
        px[0] = nx;
        py[0] = ny;
        back_buf[px[0]][py[0]] = '@';
    }
    else {
        if (py[0] == py[selected]) {
            // ���� �̾����Ƿ� ���� ���� ����
            back_buf[px[0]][py[selected]] = ' ';
            if (selected == fail) {
                jebi_dialog(selected, "Ż��!");
                player[selected].is_alive = false; // �÷��̾� ���� ������Ʈ
                n_alive--;
                if (player[0].is_alive != TRUE) {
                    selected = 0; // �ٽ� ó������
                }
                else {
                    selected++;
                }
                jebi_init();
                system("cls");
                fail = randint(0, n_alive - 1); // ���� ������ fail�� �� ���� �̸� ����
            }
            else {
                jebi_dialog(selected, "���!");
                selected++; // ���� �÷��̾�
                if (selected >= n_alive) {
                    selected = 0; // �ٽ� ó������
                    jebi_init();
                    system("cls");
                    fail = randint(0, n_alive - 1); // ���� ������ fail�� �� ���� �̸� ����
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
    int fail = randint(0, n_alive - 1); // �� ���� ���� �� �̸� fail�� �� ���� ����

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
                jebi_dialog(selected, "Ż��!");
                player[selected].is_alive = false; // �÷��̾� ���� ������Ʈ
                back_buf[px[0]][py[selected]] = ' '; // ���� ��Ͽ��� ���� ����
                n_alive--;
                selected = 0; // �ٽ� ó������
                jebi_init();
                system("cls");
                s = 0;
                fail = randint(0, n_alive - 1); // ���� ������ fail�� �� ���� �̸� ����
            }
            else {
                jebi_dialog(selected, "���!");
                selected++; // ���� �÷��̾�
                if (selected >= n_alive) {
                    selected = 0; // �ٽ� ó������
                    jebi_init();
                    system("cls");
                    s = 0;
                    fail = randint(0, n_alive - 1); // ���� ������ fail�� �� ���� �̸� ����
                }
            }
        }
        else if (key != K_UNDEFINED) {
            jebi_manual(key); // '@' �̵�
        }

        display();
        Sleep(100);
        tick += 10;
        if (n_alive <= 1) {
            break;
        }
    }
}