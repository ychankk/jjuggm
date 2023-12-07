/*
구현기능
> 아이템 랜덤배치
> 플레이어 0번이 아이템 좌표와 겹칠경우 아이템 획득
(교환했을 때 들고있던 아이템을 내려놔야하는데 못함, 무시했을 때 아이템 두고 가야하는데 못함)
> 0번 플레이어가 강탈/회유/무시 선택할 수 있음
> 랜덤플레이어가 아이템을 들고있을 때, 50%로 교환
> 랜덤플레이어끼리 만났을 때 강탈/회유/무시 수행
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
float dist_ptp(int, int); // 플레이어간 거리 계산
void do_rob(int, int); // 강탈 함수
void do_conciliate(int, int); // 회유 함수
void make_item(); // 아이템 배치
void get_item(int, int); // 아이템 획득, 변경 함수

ITEM_INFO item_info[ITEM_MAX];
int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];// 각 플레이어 위치, 이동 주기
int ix[ITEM_MAX], iy[ITEM_MAX];//각 아이템의 위치
char item_display[ITEM_MAX]; // 서로 다른 아이템을 'I'로 출력되게 하려고
char temp_message[100];

void night_init(void) {
	map_init(15, 40); // 15x30 크기 amp생성
	int x, y;
	for (int i = 0; i < n_player; i++) { // 플레이어의 위치를 랜덤으로 정해주는 부분
		// 같은 자리가 나오면 다시 생성
		do {
			x = randint(1, N_ROW - 2);
			y = randint(1, N_COL - 2);
		} while (!placable(x, y)); // do-while문을 통해 넣을곳이 없다면 멈추도록.
		px[i] = x;
		py[i] = y;
		period[i] = randint(100, 500); // 이동 주기 또한 랜덤으로 설정

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
	// 각 방향으로 움직일 때 x, y값 delta
	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_UP: dir = DIR_UP; break; // dir에 0이 들어감
	case K_DOWN: dir = DIR_DOWN; break; // " 1이 "
	case K_LEFT: dir = DIR_LEFT; break;
	case K_RIGHT: dir = DIR_RIGHT; break;
	default: return;
	}

	// 움직여서 놓일 자리
	int nx, ny;
	nx = px[0] + dx[dir];
	ny = py[0] + dy[dir];
	if (!placable(nx, ny)) {
		return;
	}

	night_move_tail(0, nx, ny);
}

void night_move_random(int player, int dir) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리

	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);

	} while (!placable(nx, ny));

	night_move_tail(p, nx, ny);
}

void night_move_tail(int plr, int nx, int ny) { // 
	int p = plr;  // 이름이 길어서...

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
	float distance = 0;
	dx = px[i] - px[j];
	dy = py[i] - py[j];
	xx = dx * dx;
	yy = dy * dy;

	distance = (float)sqrt(xx + yy);
	return distance;

}

void do_rob(int p1, int p2) {
	dialog(" 강탈시도! ");
	if ((player[p1].str + player[p1].item.str_buf) * 0.01 > (player[p2].str + player[p2].item.str_buf) * 0.01) {
		dialog(" 성 공! ");
		ITEM x = player[p1].item;
		player[p1].item = player[p2].item;
		player[p2].item = x; // p1,p2의 아이템 교환
		if (player[p1].stamina > 40) {
			player[p1].stamina -= 40;
			if (player[p2].stamina > 60) {
				player[p2].stamina -= 60;
			}
			else { player[p2].stamina = 0; }
		}
		else {
			player[p1].stamina = 0;
			if (player[p2].stamina > 40) {
				player[p2].stamina -= 40;
			}
			else { player[p2].stamina = 0; }
		}


	}
	else if ((player[p1].str + player[p1].item.str_buf) * 0.01 <= (player[p2].str + player[p2].item.str_buf) * 0.01) {
		dialog(" 실 패! ");
		if (player[p1].stamina < 60) {
			player[p1].stamina = 0;
			if (player[p2].stamina > 40) {
				player[p2].stamina -= 40;
			}
			else { player[p2].stamina = 0; }
		}
		else {
			player[p1].stamina -= 60;
			if (player[p2].stamina > 40) {
				player[p2].stamina -= 40;
			}
			else { player[p2].stamina = 0; }
		}
	}
}

void do_conciliate(int p1, int p2) {
	dialog(" 회유시도! ");
	if ((player[p1].intel + player[p1].item.intel_buf) * 0.01 > (player[p2].intel + player[p2].item.intel_buf) * 0.01) {
		dialog(" 성 공! ");
		ITEM x = player[p1].item;
		player[p1].item = player[p2].item;
		player[p2].item = x; // p1,p2의 아이템 교환
		if (player[p1].stamina > 20) {
			player[p1].stamina -= 20;
			if (player[p2].stamina > 40) {
				player[p2].stamina -= 40;
			}
			else { player[p2].stamina = 0; }
		}
		else {
			player[p1].stamina = 0;
			if (player[p2].stamina > 20) {
				player[p2].stamina -= 20;
			}
			else { player[p2].stamina = 0; }
		}

	}
	else if ((player[p1].intel + player[p1].item.intel_buf) * 0.01 <= (player[p2].intel + player[p2].item.intel_buf) * 0.01) {
		dialog(" 실 패! ");

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

		// player 1 부터는 랜덤으로 움직임(8방향)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				night_move_random(i, -1);
			}
		}

		// 0번이 아이템과 좌표가 겹칠경우
		for (int i = 0; i < ITEM_MAX; i++) {
			if (px[0] == ix[i] && py[0] == iy[i]) {
				if (player[0].hasitem == false) {
					get_item(0, i);
					dialog(" get item! ");
				}
				else if (player[0].hasitem == true) {
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
		//0번이 플레이어와 만났을 때
		for (int i = 1; i < n_player; i++) {
			if (player[0].hasitem == true && player[i].hasitem == true) {
				if (dist_ptp(0, i) < sqrt(2)) {
					dialog(" (1)강탈  (2)회유  (3)무시  ");
					if (player[0].stamina > 0) {
						switch (key) {
						case K_ROB:

							do_rob(0, i);

							break;

						case  K_CON:

							do_conciliate(0, i);

							break;
						case K_IGNOR:
							dialog(" 무시 ");
							dialog(" press move key ");
							break;

						}
					}
				}
			}
		}

		// 나머지 플레이어가 아이템과 좌표가 겹칠경우
		for (int j = 1; j < n_player; j++) {
			for (int i = 0; i < ITEM_MAX; i++) {
				if (px[j] == ix[i] && py[j] == iy[i]) {
					if (player[j].hasitem == false) {
						get_item(j, i);
					}
					else if (player[j].hasitem == true) {
						int key = randint(0, 1);
						switch (key) {
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

		// 나머지 플레이어가 다른 플레이어와 만났을 때
		bool once = false;
		for (int i = 1; i < n_player; i++) {
			for (int j = i + 1; j < n_player; j++) {

				if (player[i].hasitem == true && player[j].hasitem == true) {
					if (dist_ptp(i, j) < sqrt(2)) {
						if (player[i].stamina > 0) {
							int key1 = randint(0, 2);
							switch (key1) {
							case 0:
								do_rob(i, j);
								break;
							case  1:
								do_conciliate(i, j);
								break;
							case 2:
								break;

							}
							once = true;
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
