#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_UP		0
#define DIR_DOWN	1
#define DIR_LEFT	2
#define DIR_RIGHT	3

void mugunghwa_init(void);
void mugunghwa(void);
void mugunghwa_move_manual(key_t key);
void mugunghwa_move_random(int i);
bool check_front(int plr);
char temp_message[50];


int btn = 0; // btn은 영희가 어떤 상태인지에 따라 달라짐. (btn이 2이라면 영희가 뒤를 돌아보는것)

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // 각 플레이어 위치, 이동 주기
int pass[PLAYER_MAX], n_pass=0;

void mugunghwa_init(void) {
	map_init(4 + n_player, 40);
	int x, y;
	for (int i = 0; i < n_player; i++) {
		x = i + 2;
		y = 40 - 2;
		px[i] = x;
		py[i] = y;
		period[i] = randint(50, 150); // 이동 주기 또한 랜덤으로 설정
		back_buf[px[i]][py[i]] = '0' + i;
	}
	for (int i = 0; i < n_player; i++) { // 영희 만들기
		x = 2 + i;
		y = 1;
		back_buf[x][y] = '#';

	}
	tick = 0;

}

void mugunghwa_move_manual(key_t key) {
	if (player[0].is_alive == 1) {
		if (back_buf[px[0]][py[0] - 1] == '#') {
			pass[0] = 1;
			back_buf[px[0]][py[0]] = ' ';
			return;
		}

		if (btn == 2 && check_front(0) == 1) {
			sprintf_s(temp_message, sizeof(temp_message), "%d player dead!", 0);
			dialog(temp_message);
			player[0].is_alive = 0;
			n_alive -= 1;
			back_buf[px[0]][py[0]] = ' ';
		}
		else move_manual(key);
	}
}


void mugunghwa_move_random(int plr) {
	int p = plr;
	int nx;
	int ny;

	int rand_temp;

	if (back_buf[px[p]][py[p] - 1] == '#') {
		pass[p] = 1;
		back_buf[px[p]][py[p]] = ' ';
		return;
	}
	do {
		rand_temp = randint(0, 9);
		if (rand_temp <= 6) { // 왼쪽으로
			nx = px[p];
			ny = py[p] - 1;
		}
		else if (rand_temp == 7) { // 위로
			nx = px[p] - 1;
			ny = py[p];
		}
		else if (rand_temp == 8) { // 아래로
			nx = px[p] + 1;
			ny = py[p];
		}
		else {
			nx = px[p];
			ny = py[p];
			break;
		}	// 제자리
	} while (!placable(nx, ny)); //  || (px[p] == nx && py[p] == ny)

	if (btn == 2 && (px[p] != nx || py[p] != ny) && check_front(p) == 1) {
		sprintf_s(temp_message, sizeof(temp_message), "%d player dead!", p);
		dialog(temp_message);
		player[p].is_alive = 0;
		n_alive -= 1;
		back_buf[px[p]][py[p]] = ' ';
		return;
	}
	else move_tail(p, nx, ny);


}

bool check_front(int plr) { // 앞에 다른 플레이어가 없으면 1, 있으면 0을 반환
	int p = plr;
	bool result = 1;
	for (int i = 1; i < py[p]; i++) {
		if (!((back_buf[px[p]][i]) == '@' || (back_buf[px[p]][i]) == ' ')) {
			result = 0;
		}
	}
	return result;

}

void mugunghwa(void) {
	mugunghwa_init();

	char* line[10] = { "무","궁","화","꽃","이","피","었","습","니","다" };
	int temp_line = 0;
	system("cls");
	dialog("game start");
	display();

	int randsleep;
	int my_tick = 0; // 영희 기준의 시간을 정해주기 위해 설정.

	while (1) {
		n_pass = 0;
		for (int i = 0; i < n_player; i++) {
			if (pass[i] == 1) n_pass++;
		}
		if (n_alive == 1 || (n_alive==n_pass)) { // 경기장에 한명만 남아있으면 게임 종료 -> ending으로 넘어감
			break;
		}

		// player 0만 손으로 움직임(4방향)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			mugunghwa_move_manual(key);
		}

		// player 1 부터는 랜덤으로 움직임(3방향)
		for (int i = 1; i < n_player; i++) {
			if (player[i].is_alive == 0 || pass[i] == 1) {

			}
			else if ((tick % period[i]) == 0 && (btn != 2 || (btn == 2 && (randint(0, 9) < 1)))) {
				mugunghwa_move_random(i);
			}

		}

		if (btn == 0) {
			if (temp_line <= 4) { // 무궁화꽃이 출력 간격
				randsleep = randint(50, 100);
			}
			else if (temp_line <= 9) { // 피었습니다 출력 간격
				randsleep = randint(30, 50);

			}
			btn = 1; // randsleep값을 받았으니 출력할 차례
		}
		else if (btn == 1 && my_tick % randsleep == 0) {
			printxy_str(line[temp_line], N_ROW, temp_line * 3); // 한글자 출력
			temp_line++; //다음글자의 위치 숫자로 이동
			btn = 0; // 출력 했으니 다시 randsleep값을 받기 위해 

			if (temp_line == 10) { // 만약 10글자를 다 출력했다면
				for (int i = 0; i < n_player; i++) { // 영희 @@@로 바꾸기
					back_buf[2 + i][1] = '@';
				}

				temp_line = 0; // 다시 '무'를 출력하기 위해 첫 글자 위치로 이동
				randsleep = 3000; // 3초 쉬기 위해 3000으로 설정
				btn = 2; // 

				my_tick = 0;
			}

		}
		else if (btn == 2 && my_tick % randsleep == 0) {
			btn = 0; // 다시 randsleep을 받으러 가야 하므로
			my_tick = 0; //영희의 시간 초기화
			for (int i = 0; i < 30; i++) { // 출력되었던 곳 다시 ' ' 로 초기화
				printxy(' ', N_ROW, i);
			}
			for (int i = 0; i < n_player; i++) { // 영희를 다시 ###으로 바꿔줌
				back_buf[2 + i][1] = '#';
			}
		}
		my_tick += 10;


		display();
		Sleep(10);
		tick += 10;

	}
}

