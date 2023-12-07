/*
�������
> ������ ������ġ
> 0���� ������ �÷��̾�� ����� ���������� �̵�
> �÷��̾� 0���� ������ ��ǥ�� ��ĥ��� ������ ȹ��
(��ȯ���� �� ����ִ� �������� ���������ϴµ� ����, �������� �� ������ �ΰ� �����ϴµ� ����)
> 0�� �÷��̾ ��Ż/ȸ��/���� ������ �� ����
*/


#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h> 
#include <math.h>
#include <conio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

typedef struct {
	int x, y;
	ITEM item;
}ITEM_INFO;

void night_init(void); 
void night_move_manual(key_t key); 
void night_move_random(int i, int dir);
void night_move_tail(int i, int nx, int ny);
float dist_ptp(int, int); // �÷��̾ �Ÿ� ���
void do_rob(int, int); // ��Ż �Լ�
void do_conciliate(int, int); // ȸ�� �Լ�
void make_item(); // ������ ��ġ
void get_item(int, int); // ������ ȹ��, ���� �Լ�

ITEM_INFO item_info[ITEM_MAX];
int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];// �� �÷��̾� ��ġ, �̵� �ֱ�
int ix[ITEM_MAX], iy[ITEM_MAX];//�� �������� ��ġ
char item_display[ITEM_MAX]; // ���� �ٸ� �������� 'I'�� ��µǰ� �Ϸ���
char temp_message[100];

void night_init(void) {
	map_init(15, 40); // 15x30 ũ�� amp����
	int x, y;
	for (int i = 0; i < n_player; i++) { // �÷��̾��� ��ġ�� �������� �����ִ� �κ�
		// ���� �ڸ��� ������ �ٽ� ����
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y)); // do-while���� ���� �������� ���ٸ� ���ߵ���.
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 500); // �̵� �ֱ� ���� �������� ����

		back_buf[px[i]][py[i]] = '0' + i;  // (0 .. n_player-1)
	}
	for (int i = 0; i < n_player - 1; i++) {
		ITEM_INFO info;
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y));
		ix[i] = x;
		iy[i] = y;
		info.x = x;
		info.y = y;
		item_info[i] = info;

		back_buf[x][y] = 'I';
	}

	tick = 0;
}

void night_move_manual(key_t key) {
	// �� �������� ������ �� x, y�� delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // ������ ����(0~3)
	switch (key) {
		case K_UP: dir = DIR_UP; break; // dir�� 0�� ��
		case K_DOWN: dir = DIR_DOWN; break; // " 1�� "
		case K_LEFT: dir = DIR_LEFT; break;
		case K_RIGHT: dir = DIR_RIGHT; break;
		default: return;
	}

	// �������� ���� �ڸ�
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	night_move_tail(0, nx, ny);
}

void night_move_random(int player, int dir) {
	int p = player;  // �̸��� ��...
	int nx, ny;  // �������� ������ ���� �ڸ�

	// ������ ������ ���� ���� ���ٰ� ����(���� ������ ����)	

	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
		
	} while (!placable(nx, ny));

	night_move_tail(p, nx, ny);
}

void night_move_tail(int plr, int nx, int ny) { // 
	int p = plr;  // �̸��� ��...

	if (px[p] == nx && py[p] == ny) return;
	
	else {
		back_buf[nx][ny] = back_buf[px[p]][py[p]];
		back_buf[px[p]][py[p]] = ' ';
		px[p] = nx;
		py[p] = ny;
	}


}

float dist_ptp(int i, int j) {
	int dx, dy, xx, yy;
	double distance = 0;
	dx = px[i] - px[j];
	dy = py[i] - py[j];
	xx = dx * dx;
	yy = dy * dy;

	distance = (float)sqrt(xx + yy);
	return (float)distance;

}

void do_rob(int p1, int p2) {
	dialog(" ��Ż�õ�! ");
	if ( (player[p1].str+player[p1].item.str_buf)*0.01 > (player[p2].str + player[p2].item.str_buf)*0.01 ) {
		dialog(" �� ��! ");
		ITEM x = player[p1].item;
		player[p1].item = player[p2].item;
		player[p2].item = x; // p1,p2�� ������ ��ȯ
		if (player[p1].stamina > 40) {
			player[p1].stamina -= 40;
		}
		else {
			player[p1].stamina = 0;
		}

	}
	else if((player[p1].str + player[p1].item.str_buf) * 0.01 <= (player[p2].str + player[p2].item.str_buf) * 0.01) {
		dialog(" �� ��! ");
		if (player[p1].stamina < 60) {
			player[p1].stamina = 0;
		}
		else {
			player[p1].stamina -= 60;
		}
	}
}

void do_conciliate(int p1, int p2) {
	dialog(" ȸ���õ�! ");
	if ((player[p1].intel + player[p1].item.intel_buf) * 0.01 > (player[p2].intel + player[p2].item.intel_buf) * 0.01) {
		dialog(" �� ��! ");
		ITEM x = player[p1].item;
		player[p1].item = player[p2].item;
		player[p2].item = x; // p1,p2�� ������ ��ȯ
		if (player[p1].stamina > 20) {

			player[p1].stamina -= 20;
		}
		else {
			player[p1].stamina = 0;
		}

	}
	else if((player[p1].intel + player[p1].item.intel_buf) * 0.01 <= (player[p2].intel + player[p2].item.intel_buf) * 0.01) {
		dialog(" �� ��! ");

		if (player[p1].stamina < 40) {
			player[p1].stamina = 0;
		}
		else {
			player[p1].stamina -= 40;
		}
	}

}

void make_item() {
	int x, y;
	for (int i = 0; i < n_player - 1; i++) {
		ITEM_INFO info;
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y));
		ix[i] = x;
		iy[i] = y;
		info.x = x;
		info.y = y;
		item_info[i] = info;

		back_buf[x][y] = 'I';
	}

}

void get_item(int a, int b) {
	player[a].item = item[b];
	player[a].hasitem = true;
	ix[b] = 0;
	iy[b] = 0;
}


void nightgame(void) {
	night_init();
	system("cls");

	display();

	while (1) {
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			night_move_manual(key);
		}
		
		// player 1 ���ʹ� �������� ������(8����)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				night_move_random(i, -1);
			}
		}

		// 0���� �����۰� ��ǥ�� ��ĥ���, 
		for (int i = 0; i < ITEM_MAX; i++) {
			if ( px[0] == ix[i] && py[0] == iy[i] ) {
				if (player[0].hasitem == false) {
					get_item(0, i);
					dialog(" get item! ");
				} 
				else if( player[0].hasitem == true ) {
					dialog(" want change item? Y/N ");
					switch (key) {
					case K_Y:
						get_item(0, i);
						dialog(" change item! ");
						break;
					case K_N:
						dialog(" not change! ");
						ix[i] = 0;
						iy[i] = 0;
						break;
					}
				}
			}
		}

		// ������ �÷��̾ �����۰� ��ǥ�� ��ĥ���
		for (int j = 1; j < n_player; j++) {
			for (int i = 0; i < ITEM_MAX; i++) {
				if (px[j] == ix[i] && py[j] == iy[i]) {
					if (player[j].hasitem == false) {
						get_item(j, i);
					}
					else if (player[j].hasitem == true) {
						int key = randint(0, 1);
						switch(key) {
						case 0:
							get_item(j, i);
							break;
						case 1:
							ix[j] = 0;
							iy[j] = 0;
							break;
						}
					}
				}
			}
		}


		// ������ �÷��̾ �ٸ� �÷��̾�� ������ ��
		/*for (int i = 1; i < n_player; i++) {
			for (int j = 2; j < n_player; j++) {
				if ((float)dist_ptp(i, j) < (float)sqrt(2)) {
					int key[2] = {0,1};

					if (key[randint(-1,1)] == 0) {
						if (player[0].stamina > 0) {
							do_rob(i, j);
						}
					}
					else if (key[randint(-1,1)] == 1) {
					}

				}
			}

		}*/

		//0���� �÷��̾�� ������ ��
		for (int i = 1; i < n_player; i++) {
			if(player[0].hasitem==true && player[i].hasitem == true) {
				if (dist_ptp(0, i) < sqrt(2)) {
					dialog(" (1)��Ż  (2)ȸ��  (3)����  ");
					if (player[0].stamina > 0) {
						switch (key) {
						case K_ROB:
							
							do_rob(0, i);
							
							break;

						case  K_CON:
							
							do_conciliate(0, i);
							
							break;
						case K_IGNOR:
							dialog(" ���� ");
							break;

						}
					}
				}
			}
		}



		display();
		Sleep(10);
		tick += 10;


	}

}