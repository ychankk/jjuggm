#pragma once
// ���� ����
#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX	10 // �÷��̾� �ִ��ο� ����
#define ITEM_MAX 10

typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
} ITEM;

typedef struct {
	int id;
	char name[100];

	// �ɷ�ġ: ����, ��, ���׹̳�
	int intel, str, stamina;

	bool is_alive;
	bool hasitem;
	ITEM item;
} PLAYER;

typedef struct {
	float real_intel;
	float real_str;
} PLAYER_STATUS;


int n_player, n_alive, n_item; // �÷��̾��, ����ִ� �÷��̾��, ������ ��
int tick;  // �ð�

PLAYER player[PLAYER_MAX];
ITEM item[ITEM_MAX];
PLAYER_STATUS player_status[PLAYER_MAX];

// �̴ϰ���
void sample(void);
void mugunghwa(void);
void nightgame(void);
void juldarigi(void);
//void jebi(void);

int randint(int low, int high);

#endif
