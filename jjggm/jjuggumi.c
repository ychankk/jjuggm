// 메인 함수

// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);
void intro(void);
void update_player_status(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init() {
	srand((unsigned int)time(NULL));
	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1; // -1 리턴하면 메인함수에서 처리하고 종료
	}
	// 플레이어 데이터 load & 초기설정
	fscanf_s(fp, "%d", &n_player); // "jjuggumi.dat" 에서 6 읽
	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		// 아직 안 배운 문법(구조체 포인터, 간접참조연산자)
		PLAYER* p = &player[i];
		// 파일에서 각 스탯 최댓값 읽기
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->stamina = 100; // 100%
		// 현재 상태
		p->is_alive = true;
		p->hasitem = false;
	}
	// 아이템 데이터 load
	fscanf_s(fp, "%d", &n_item); // 10을 읽음
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
		if (p->stamina > 100) p->stamina = 100; // 오바되는 스테미나도 여기서 정리해줌
		else if (p->stamina < 0) p->stamina = 0;
		player_status[i].real_intel = ((p->intel)+(p->hasitem ? p->item.intel_buf : 0)) * ((float)p->stamina / 100);
		player_status[i].real_str = ((p->str) + (p->hasitem ? p->item.str_buf : 0)) * ((float)p->stamina / 100);
	}
}

void intro() {
	system("cls");
	// ASCII 아트 출력
	printf(
		"     ___    ___  _   _  _____  _____  _   _ ___  ___ _____\n"
		"   | _  |  |_  || | | ||  __ | | __ || | | ||  |/  ||_   _|\n"
		"      | |    | || | | || |  |/| |  |/| | | || .  . |  | |\n"
		"      | |    | || | | || | __ | | __ | | | || ||/| |  | |\n"
		" /|__ / //|__/ /| |_| || |_| || |_| || |_| || |  | | _| |_\n"
		"|_____ /|_____ /|____ /|____ /|____ /|____ /|_|  |_|/ ___ /\n"
	);
	Sleep(5000); //5초 지연
	system("cls"); // 콘솔화면 지우기

}

void ending() {
	system("cls"); // 콘솔화면 지우기
	printf(" _____    ___  ___  ___ _____   _____  _   _  _____ ______\n");
	printf("| __  |  / _ ||  |/  ||  ___| |  _  || | | ||  ___ |  ___ |\n");
	printf("| | |/  / /_||| .  . || |__   | | | || | | || |__  | |_/ /\n");
	printf("| | __ | ___||| | /| ||  __|  | | | || | | ||  __| |  _  |\n");
	printf("| |_  || |  ||| |  | || |___  | |/ / | |/ / | |___ | | | |_\n");
	printf("|____/ |_|  |||_|  |_||_____| | __/  |___/  |____ /|_| |__/\n");
	printf("\n");

	/* // 우승자가 1명일때 (기말과제 때 쓰일 코드)
	int winner = 0;for (int i = 0; i < n_player; i++) {
		if (player[i] == 1) {
			winner = i;
		}
	}
	*/


	Sleep(2000);
	printf("우승자는 바로바로~~~~~~\n");
	Sleep(1000);
	printf("...........\n");
	Sleep(1000);
	printf("...........\n");
	Sleep(1000);
	printf("...........\n");
	Sleep(1000);
	printf("축하합니다!!\n");
	Sleep(1000);
	printf("456억의 주인공은!!\n");
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


