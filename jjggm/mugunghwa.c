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


int btn = 0; // btn�� ���� � ���������� ���� �޶���. (btn�� 2�̶�� ���� �ڸ� ���ƺ��°�)

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];  // �� �÷��̾� ��ġ, �̵� �ֱ�
int pass[PLAYER_MAX], n_pass=0;

void mugunghwa_init(void) {
	map_init(4 + n_player, 40);
	int x, y;
	for (int i = 0; i < n_player; i++) {
		x = i + 2;
		y = 40 - 2;
		px[i] = x;
		py[i] = y;
		period[i] = randint(50, 150); // �̵� �ֱ� ���� �������� ����
		back_buf[px[i]][py[i]] = '0' + i;
	}
	for (int i = 0; i < n_player; i++) { // ���� �����
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
		if (rand_temp <= 6) { // ��������
			nx = px[p];
			ny = py[p] - 1;
		}
		else if (rand_temp == 7) { // ����
			nx = px[p] - 1;
			ny = py[p];
		}
		else if (rand_temp == 8) { // �Ʒ���
			nx = px[p] + 1;
			ny = py[p];
		}
		else {
			nx = px[p];
			ny = py[p];
			break;
		}	// ���ڸ�
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

bool check_front(int plr) { // �տ� �ٸ� �÷��̾ ������ 1, ������ 0�� ��ȯ
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

	char* line[10] = { "��","��","ȭ","��","��","��","��","��","��","��" };
	int temp_line = 0;
	system("cls");
	dialog("game start");
	display();

	int randsleep;
	int my_tick = 0; // ���� ������ �ð��� �����ֱ� ���� ����.

	while (1) {
		n_pass = 0;
		for (int i = 0; i < n_player; i++) {
			if (pass[i] == 1) n_pass++;
		}
		if (n_alive == 1 || (n_alive==n_pass)) { // ����忡 �Ѹ� ���������� ���� ���� -> ending���� �Ѿ
			break;
		}

		// player 0�� ������ ������(4����)
		key_t key = get_key();
		if (key == K_QUIT) {
			break;
		}
		else if (key != K_UNDEFINED) {
			mugunghwa_move_manual(key);
		}

		// player 1 ���ʹ� �������� ������(3����)
		for (int i = 1; i < n_player; i++) {
			if (player[i].is_alive == 0 || pass[i] == 1) {

			}
			else if ((tick % period[i]) == 0 && (btn != 2 || (btn == 2 && (randint(0, 9) < 1)))) {
				mugunghwa_move_random(i);
			}

		}

		if (btn == 0) {
			if (temp_line <= 4) { // ����ȭ���� ��� ����
				randsleep = randint(50, 100);
			}
			else if (temp_line <= 9) { // �Ǿ����ϴ� ��� ����
				randsleep = randint(30, 50);

			}
			btn = 1; // randsleep���� �޾����� ����� ����
		}
		else if (btn == 1 && my_tick % randsleep == 0) {
			printxy_str(line[temp_line], N_ROW, temp_line * 3); // �ѱ��� ���
			temp_line++; //���������� ��ġ ���ڷ� �̵�
			btn = 0; // ��� ������ �ٽ� randsleep���� �ޱ� ���� 

			if (temp_line == 10) { // ���� 10���ڸ� �� ����ߴٸ�
				for (int i = 0; i < n_player; i++) { // ���� @@@�� �ٲٱ�
					back_buf[2 + i][1] = '@';
				}

				temp_line = 0; // �ٽ� '��'�� ����ϱ� ���� ù ���� ��ġ�� �̵�
				randsleep = 3000; // 3�� ���� ���� 3000���� ����
				btn = 2; // 

				my_tick = 0;
			}

		}
		else if (btn == 2 && my_tick % randsleep == 0) {
			btn = 0; // �ٽ� randsleep�� ������ ���� �ϹǷ�
			my_tick = 0; //������ �ð� �ʱ�ȭ
			for (int i = 0; i < 30; i++) { // ��µǾ��� �� �ٽ� ' ' �� �ʱ�ȭ
				printxy(' ', N_ROW, i);
			}
			for (int i = 0; i < n_player; i++) { // ���� �ٽ� ###���� �ٲ���
				back_buf[2 + i][1] = '#';
			}
		}
		my_tick += 10;


		display();
		Sleep(10);
		tick += 10;

	}
}

