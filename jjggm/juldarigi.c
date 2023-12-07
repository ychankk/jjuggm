#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"
#include <stdio.h>

#define DIR_left		0
#define DIR_right		1
#define DIR_left_lay	2
#define DIR_right_lay	3

void juldarigi_init();
void print_str();
void juldarigi_display();
void juldarigi_move(key_t key);
void juldarigi_move_left();
void juldarigi_move_right();
void check_fell();

int px[PLAYER_MAX], py[PLAYER_MAX];
int is_alive_juldarigi[PLAYER_MAX];
int n_alive_juldarigi;
bool check_lay[PLAYER_MAX] = { 0 };

float left_str = 0;
float right_str = 0;
float total_str = 0;
float just_left_str = 0;
float just_right_str = 0;
float plus_left_str = 0;
float plus_right_str = 0;

// check fell 함수에서 사용됨
int cnt_left = 0;
int cnt_right = 1;

bool whether_left_lay = 0;
bool whether_right_lay = 0;

char temp_message[50];

void juldarigi_init() {
	int x, y;
	int cnt = 0;
	map_init(3, 29);
	back_buf[0][14] = back_buf[2][14] = ' ';
	back_buf[1][13] = back_buf[1][14] = back_buf[1][15] = '-';
	for (int i = 0; i < n_player; i++) {
		x = 1;
		if (cnt % 2 == 0) y = 12 - cnt / 2;
		else if (cnt % 2 == 1) y = 16 + cnt / 2;
		px[i] = x;
		py[i] = y;
		back_buf[x][y] = '0' + i;
		cnt++;
	}
	tick = 0;

	update_player_status();
	cnt = 0;
	for (int i = 0; i < n_player; i++) {
		is_alive_juldarigi[i] = 1;
		PLAYER_STATUS* ps = &player_status[i];
		if (cnt % 2 == 0) just_left_str += ps->real_str;
		else if (cnt % 2 == 1) just_right_str += ps->real_str;
		cnt++;
	}
	n_alive_juldarigi = n_player;
}

void print_str() {
	total_str = right_str - left_str;
	char temp_str[100];
	sprintf_s(temp_str, sizeof(temp_str), "str:  %.1f      ", total_str);
	printxy_str(temp_str, 4, 0);

}

void juldarigi_display() {
	draw();
	print_str();

	// print_status (juldarigi용)
	gotoxy(N_ROW + 4, 0);
	printf("no. of players left: %d  (juldarigi left : %d) \n", n_alive, n_alive_juldarigi);
	printf("                   intl       str     stm    real_intel    real_str\n");
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];
		PLAYER_STATUS* ps = &player_status[i];
		if (!p->is_alive) p->hasitem = 0;// 아이템이 없는 플레이어(탈락했었던 플레이어) 아이템 삭제
		update_player_status();
		printf("player %2d: %5s", i, p->is_alive ? "alive" : "dead");
		printf("  %2d(+%d)    %2d(+%d)    %3d%%       %4.1f        %4.1f\n", p->intel, (p->hasitem ? p->item.intel_buf : 0), p->str, (p->hasitem ? p->item.str_buf : 0), p->stamina, ps->real_intel, ps->real_str);

	}
}

void juldarigi_move(key_t key) {
	int dir;  // 움직일 방향(0~3)
	switch (key) {
	case K_left_str: dir = DIR_left; break; // dir에 0이 들어감
	case K_right_str: dir = DIR_right; break; // " 1이 "
	case K_left_lay: dir = DIR_left_lay; break;
	case K_right_lay: dir = DIR_right_lay; break;
	default: return;
	}

	switch (dir) {
	case 0: plus_left_str += 1; break;
	case 1: plus_right_str += 1; break;
	case 2:
		if (whether_left_lay != 1) {
			sprintf_s(temp_message, sizeof(temp_message), "left team - lay!!");
			printxy_str(temp_message, 3, 0);
			Sleep(1500);
			sprintf_s(temp_message, sizeof(temp_message), "                  ");
			printxy_str(temp_message, 3, 0);

			for (int i = 0; i < n_player; i += 2) {
				PLAYER* p = &player[i];
				if (is_alive_juldarigi[i] == 1) {
					// 스테미나 깍일 플레이어 check_lay로 표기 (여기서 바로 스테미나를 깎아버리면 30스테미너가 깎인 상태에서 힘 2배가 됨)
					check_lay[i] = 1;
				}
			}
			whether_left_lay = 1; just_left_str *= 2;
		}
		break;
	case 3:
		if (whether_right_lay != 1) {
			sprintf_s(temp_message, sizeof(temp_message), "right team - lay!!");
			printxy_str(temp_message, 3, 0);
			Sleep(1500);
			sprintf_s(temp_message, sizeof(temp_message), "                  ");
			printxy_str(temp_message, 3, 0);

			for (int i = 1; i < n_player; i += 2) {
				PLAYER* p = &player[i];
				if (is_alive_juldarigi[i] == 1) {
					check_lay[i] = 1;
				}
			}
			whether_right_lay = 1; just_right_str *= 2;
		}
		break;
	}
}

void juldarigi_move_left() {
	int x, y;

	for (int i = 2; i < 27; i++) {
		x = 1;
		y = i;
		back_buf[x][y - 1] = back_buf[x][y];
	}
}

void juldarigi_move_right() {
	int x, y;
	for (int i = 26; i > 1; i--) {
		x = 1;
		y = i;
		back_buf[x][y + 1] = back_buf[x][y];
	}
}

//void check_fell() {
//	int x, y;
//	if (back_buf[1][14] != '-' && back_buf[1][15] == '-') { //  왼쪽팀이 떨어진 경우
//		PLAYER* p = &player[cnt_left];
//		PLAYER_STATUS* ps = &player_status[cnt_left];
//		is_alive_juldarigi[cnt_left] = 0;
//		n_alive_juldarigi--;
//		// 살았던 사람이 탈락하면 아이템 몰수, 힘과 지능 기본값 절반으로 
//		// 죽었던 사람이 탈락하면 게임오버
//		// 탈락하면 just_~~_str에서 차감
//		just_left_str -= ps->real_str;
//		if(p->is_alive) {
//			p->hasitem = 0;
//			p->str /= 2;
//			p->intel /= 2;
//		} // 죽었던 사람은 원래 is_alive가 0이므로 더 건드리지 않아도 된다.
//		sprintf_s(temp_message, sizeof(temp_message), "%d player dead!", cnt_left);
//		printxy_str(temp_message, 3, 0);
//		Sleep(1000);
//		sprintf_s(temp_message, sizeof(temp_message), "               ");
//		printxy_str(temp_message, 3, 0);
//		
//		for (int i = 14; i > 2; i--) {
//			x = 1;
//			y = i;
//			back_buf[x][y] = back_buf[x][y-1];
//		}
//		cnt_left += 2;
//
//	}
//	else if (back_buf[1][14] != '-' && back_buf[1][13] == '-') {
//		PLAYER* p = &player[cnt_right];
//		PLAYER_STATUS* ps = &player_status[cnt_right];
//		is_alive_juldarigi[cnt_right] = 0;
//		n_alive_juldarigi--;
//
//		just_right_str -= ps->real_str;
//
//		if (p->is_alive) {
//			p->hasitem = 0;
//			p->str /= 2;
//			p->intel /= 2;
//		} // 죽었던 사람은 원래 is_alive가 0이므로 더 건드리지 않아도 된다.
//		sprintf_s(temp_message, sizeof(temp_message), "%d player dead!", cnt_right);
//		printxy_str(temp_message, 3, 0);
//		Sleep(1000);
//		sprintf_s(temp_message, sizeof(temp_message), "               ");
//		printxy_str(temp_message, 3, 0);
//		for (int i = 14; i < 26; i++) {
//			x = 1;
//			y = i;
//			back_buf[x][y] = back_buf[x][y + 1];
//		}
//		cnt_right += 2;
//	}
//	
//}

void check_fell() {
	int x, y;
	if (back_buf[1][14] != '-' && total_str > 0) { //  왼쪽팀이 떨어진 경우
		PLAYER* p = &player[cnt_left];
		PLAYER_STATUS* ps = &player_status[cnt_left];
		is_alive_juldarigi[cnt_left] = 0;
		n_alive_juldarigi--;
		p->is_alive = false;
		// 살았던 사람이 탈락하면 아이템 몰수, 힘과 지능 기본값 절반으로 
		// 죽었던 사람이 탈락하면 게임오버
		// 탈락하면 just_~~_str에서 차감
		just_left_str -= ps->real_str;
		if (p->is_alive) {
			p->hasitem = 0;
			p->str /= 2;
			p->intel /= 2;
		} // 죽었던 사람은 원래 is_alive가 0이므로 더 건드리지 않아도 된다.
		sprintf_s(temp_message, sizeof(temp_message), "%d player dead!", cnt_left);
		printxy_str(temp_message, 3, 0);
		Sleep(1000);
		sprintf_s(temp_message, sizeof(temp_message), "               ");
		printxy_str(temp_message, 3, 0);

		for (int i = 14; i > 2; i--) {
			x = 1;
			y = i;
			back_buf[x][y] = back_buf[x][y - 1];
		}
		cnt_left += 2;

	}
	else if (back_buf[1][14] != '-' && total_str < 0) {
		PLAYER* p = &player[cnt_right];
		PLAYER_STATUS* ps = &player_status[cnt_right];
		is_alive_juldarigi[cnt_right] = 0;
		n_alive_juldarigi--;
		p->is_alive = false;

		just_right_str -= ps->real_str;

		if (p->is_alive) {
			p->hasitem = 0;
			p->str /= 2;
			p->intel /= 2;
		} // 죽었던 사람은 원래 is_alive가 0이므로 더 건드리지 않아도 된다.
		if (cnt_left >= n_player) cnt_left -= 2;
		if (cnt_right >= n_player) cnt_right -= 2;

		sprintf_s(temp_message, sizeof(temp_message), "%d player dead!", cnt_right);
		printxy_str(temp_message, 3, 0);
		Sleep(1000);
		sprintf_s(temp_message, sizeof(temp_message), "               ");
		printxy_str(temp_message, 3, 0);
		for (int i = 14; i < 26; i++) {
			x = 1;
			y = i;
			back_buf[x][y] = back_buf[x][y + 1];
		}
		cnt_right += 2;
	}

}

void juldarigi() {
	juldarigi_init();

	system("cls");
	juldarigi_display();


	while (1) {

		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			juldarigi_move(key);
		}

		left_str = just_left_str + plus_left_str;
		right_str = just_right_str + plus_right_str;

		if (tick % 2000 == 0) {
			if (((left_str < right_str) && (back_buf[1][14] != '-' && back_buf[1][15] == '-')) || ((left_str > right_str) && (back_buf[1][14] != '-' && back_buf[1][13] == '-'))) {
				check_fell();
			}
			else {
				if (left_str > right_str) {
					juldarigi_move_left();
					check_fell();
					if (whether_left_lay == 1) {
						juldarigi_move_left();
						check_fell();
					}
				}
				else if (left_str < right_str) {
					juldarigi_move_right();
					check_fell();
					if (whether_right_lay == 1) {
						juldarigi_move_right();
						check_fell();
					}
				}
			}

			plus_left_str = plus_right_str = 0;

			if (whether_left_lay == 1) {
				whether_left_lay = 0;
				just_left_str /= 2; // 2배 된 것 다시 돌려놓기
				for (int i = 0; i < n_player; i += 2) { // 힘 2배 됐던 후에 스테미나 차감할 수 있도록.
					PLAYER* p = &player[i];
					if (check_lay[i] == 1) {
						p->stamina -= 30;
						check_lay[i] = 0;
					}
				}

			}
			if (whether_right_lay == 1) {
				whether_right_lay = 0;
				just_right_str /= 2;
				for (int i = 1; i < n_player; i += 2) { // 힘 2배 됐던 후에 스테미나 차감할 수 있도록.
					PLAYER* p = &player[i];
					if (check_lay[i] == 1) {
						p->stamina -= 30;
						check_lay[i] = 0;
					}
				}
			}

			juldarigi_display();
			if (cnt_left >= n_player || cnt_right > n_player) {
				if (cnt_right >= n_player) { // cnt_right 가 n_player값보다 크거나 같아지는는 순간 left팀이 우승함
					sprintf_s(temp_message, sizeof(temp_message), "!!!! left team game win !!!!");
					for (int i = 0; i < n_player; i += 2) {
						if (is_alive_juldarigi[i] == 1) {
							PLAYER* p = &player[i];
							p->is_alive = 1;
						}

					}
				}
				else if (cnt_left >= n_player) { // cnt_left 가 n_player값보다 크거나 같아지는 순간 right팀이 우승함
					sprintf_s(temp_message, sizeof(temp_message), "!!!! right team game win !!!!");
					for (int i = 1; i < n_player; i += 2) {
						if (is_alive_juldarigi[i] == 1) {
							PLAYER* p = &player[i];
							p->is_alive = 1;
						}

					}
				}

				printxy_str(temp_message, 3, 0);
				Sleep(3000);

				break;
			}
		}



		juldarigi_display();
		Sleep(10);
		tick += 10;
	}



}