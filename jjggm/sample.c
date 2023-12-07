// 간단한 예시

#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h> 

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void sample_init(void); // 초기설정
void move_manual(key_t key); // 
void move_random(int i, int dir);
void move_tail(int i, int nx, int ny);

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기

void sample_init(void) {
	map_init(15, 40); // 15x40 크기 amp생성
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

	tick = 0;
}

void move_manual(key_t key) {
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

	move_tail(0, nx, ny);
}

// 0 <= dir < 4가 아니면 랜덤
void move_random(int player, int dir) {
	int p = player;  // 이름이 길어서...
	int nx, ny;  // 움직여서 다음에 놓일 자리

	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)	

	do {
		nx = px[p] + randint(-1, 1);
		ny = py[p] + randint(-1, 1);
	} while (!placable(nx, ny));

	move_tail(p, nx, ny);
}

// back_buf[][]에 기록
void move_tail(int plr, int nx, int ny) { // 
	int p = plr;  // 이름이 길어서...

	if (px[p] == nx && py[p] == ny) return;

	else {
		back_buf[nx][ny] = back_buf[px[p]][py[p]];
		back_buf[px[p]][py[p]] = ' ';
		px[p] = nx;
		py[p] = ny;
	}


}

void sample(void) {
	sample_init();
	system("cls");
	int temp_time = 10;

	// 이렇게 sample()함수 안에 넣어줘야하네.
	// 만약 다른곳에서도 쓰고싶으면 mugunghwa()함수에 안으면 되듯 넣어주면 됨.
	display();

	while (1) {
		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			move_manual(key);
		}

		// player 1 부터는 랜덤으로 움직임(8방향)
		for (int i = 1; i < n_player; i++) {
			if (tick % period[i] == 0) {
				move_random(i, -1);
			}
		}
		display();
		Sleep(100);
		tick += 10;
		temp_time -= 1;
		if (temp_time == 0) break;

	}



	dialog("game start");
}

