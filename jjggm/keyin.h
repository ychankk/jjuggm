#ifndef _KEYIN_H_
#define _KEYIN_H_

// �Է� �޴� Ű ����
// Ű�� �߰�/������ ��� N_KEY��,
// keyin.c:is_valid():key_list[]�� �Բ� �����ؾ� ��
#define N_KEY		15  // K_UNDEFINED �� �� Ű ����

// ����: #define ��� enum �������ε� ���� ����
#define K_ARROW		224
#define K_UP		72
#define K_DOWN		80
#define K_LEFT		75
#define K_RIGHT		77
#define K_QUIT		'q'
#define K_SPACE		' '
#define K_UNDEFINED	'\0'

#define K_Y 'y'
#define K_N 'n'
#define K_ROB '1'
#define K_CON '2'
#define K_IGNOR '3'

#define K_left_str 'z'
#define K_right_str '/'
#define K_left_lay 'x'
#define K_right_lay '.'



typedef int key_t;

key_t get_key(void);

#endif
