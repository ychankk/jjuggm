// ���� �Լ�

// 2023-2 ������α׷��� ����: �޲ٹ� ����

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
void intro(void);
void update_player_status(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init() {
	srand((unsigned int)time(NULL));
	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 �����ϸ� �����Լ����� ó���ϰ� ����
	}
	// �÷��̾� ������ load & �ʱ⼳��
	fscanf_s(fp, "%d", &n_player); // "jjuggumi.dat" ���� 6 ��
	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		// ���� �� ��� ����(����ü ������, ��������������)
		PLAYER* p = &player[i];
		// ���Ͽ��� �� ���� �ִ� �б�
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->stamina = 100; // 100%
		// ���� ����
		p->is_alive = true;
		p->hasitem = false;
	}
	// ������ ������ load
	fscanf_s(fp, "%d", &n_item); // 10�� ����
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}
	fclose(fp);
	return 0;
}

void update_player_status() {
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];
		if (p->stamina > 100) p->stamina = 100; // ���ٵǴ� ���׹̳��� ���⼭ ��������
		else if (p->stamina < 0) p->stamina = 0;
		player_status[i].real_intel = ((p->intel)+(p->hasitem ? p->item.intel_buf : 0)) * ((float)p->stamina / 100);
		player_status[i].real_str = ((p->str) + (p->hasitem ? p->item.str_buf : 0)) * ((float)p->stamina / 100);
	}
}

void intro() {
	system("cls");
	// ASCII ��Ʈ ���
	printf(
		"     ___    ___  _   _  _____  _____  _   _ ___  ___ _____\n"
		"   | _  |  |_  || | | ||  __ | | __ || | | ||  |/  ||_   _|\n"
		"      | |    | || | | || |  |/| |  |/| | | || .  . |  | |\n"
		"      | |    | || | | || | __ | | __ | | | || ||/| |  | |\n"
		" /|__ / //|__/ /| |_| || |_| || |_| || |_| || |  | | _| |_\n"
		"|_____ /|_____ /|____ /|____ /|____ /|____ /|_|  |_|/ ___ /\n"
	);
	Sleep(5000); //5�� ����
	system("cls"); // �ܼ�ȭ�� �����

}

void ending() {
	system("cls"); // �ܼ�ȭ�� �����
	printf(" _____    ___  ___  ___ _____   _____  _   _  _____ ______\n");
	printf("| __  |  / _ ||  |/  ||  ___| |  _  || | | ||  ___ |  ___ |\n");
	printf("| | |/  / /_||| .  . || |__   | | | || | | || |__  | |_/ /\n");
	printf("| | __ | ___||| | /| ||  __|  | | | || | | ||  __| |  _  |\n");
	printf("| |_  || |  ||| |  | || |___  | |/ / | |/ / | |___ | | | |_\n");
	printf("|____/ |_|  |||_|  |_||_____| | __/  |___/  |____ /|_| |__/\n");
	printf("\n");

	/* // ����ڰ� 1���϶� (�⸻���� �� ���� �ڵ�)
	int winner = 0;for (int i = 0; i < n_player; i++) {
		if (player[i] == 1) {
			winner = i;
		}
	}
	*/


	Sleep(2000);
	printf("����ڴ� �ٷιٷ�~~~~~~\n");
	Sleep(1000);
	printf("...........\n");
	Sleep(1000);
	printf("...........\n");
	Sleep(1000);
	printf("...........\n");
	Sleep(1000);
	printf("�����մϴ�!!\n");
	Sleep(1000);
	printf("456���� ���ΰ���!!\n");
	Sleep(1000);
	// printf("%d!!!!!!\n", winner);
	for (int i = 0; i < n_player; i++) if (player[i].is_alive == 1) printf("%d ", i);
	printf("!!!!!!\n");
}

int main(void) {

	jjuggumi_init();
	intro();
	sample();
	mugunghwa();
	nightgame();
	juldarigi();
	jebi();
	ending();



	return 0;
}


