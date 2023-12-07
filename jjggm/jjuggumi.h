#pragma once
// 공용 설정
#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX	10 // 플레이어 최대인원 선언
#define ITEM_MAX 10

typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
} ITEM;

typedef struct {
	int id;
	char name[100];

	// 능력치: 지능, 힘, 스테미나
	int intel, str, stamina;

	bool is_alive;
	bool hasitem;
	ITEM item;
} PLAYER;

typedef struct {
	float real_intel;
	float real_str;
} PLAYER_STATUS;


int n_player, n_alive, n_item; // 플레이어수, 살아있는 플레이어수, 아이템 수
int tick;  // 시계

PLAYER player[PLAYER_MAX];
ITEM item[ITEM_MAX];
PLAYER_STATUS player_status[PLAYER_MAX];

// 미니게임
void sample(void);
void mugunghwa(void);
void nightgame(void);
void juldarigi(void);
//void jebi(void);

int randint(int low, int high);

#endif
