#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>
#define ENTER 10
#define BACKSPACE 8
#define ESC 27
#define SPACE 32
#define LEFT 75 // 방향키
#define RIGHT 77
#define UP 72
#define DOWN 80
int mine[60][60] = { 0, }, flag, game_over, hide;
int seen[60][60], dx[4] = { 1, 0, -1, 0 }, dy[4] = { 0, 1, 0, -1 };
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
} //좌표 함수
void SwitchingConsoleCursor(bool flag) {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 10;
	cursorInfo.bVisible = flag;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
} //커서 보이기/숨기기 함수
void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
} //글씨 색상 함수
void ConsoleSize(int w, int h) {
	char command[60];
	sprintf(command, "mode con: cols=%d lines=%d", 2 * w + 4, h + 8);
	system(command);
} //콘솔창 크기 조절 함수
void main_s() { //메인 화면
	puts("┌────────────────────────────────────┐");
	puts("│                                    │");
	puts("│   ==============================   │");
	puts("│   ■※■※①■■■▶■①※※※※   │");
	puts("│   ▶■②①①■■■■■①②③④※   │");
	puts("│   ①  M I N E S W E E P E R   ①   │");
	puts("│   ※②①□   지 뢰 찾 기  ■■■   │");
	puts("│   ■※③①①■■※■■■①②②①   │");
	puts("│   ■※■※①■■■■■■①▶※①   │");
	puts("│   ==============================   │");
	puts("│                                    │");
	puts("│   ┌───────────┐    ┌───────────┐   │");
	puts("│   │ 1   쉬움  │    │ 2   보통  │   │");
	puts("│   └───────────┘    └───────────┘   │");
	puts("│                                    │");
	puts("│   ┌───────────┐    ┌───────────┐   │");
	puts("│   │ 3  어려움 │    │ 4  커스텀 │   │");
	puts("│   └───────────┘    └───────────┘   │");
	puts("│                                    │");
	printf("└────────────────────────────────────┘");
} //Main_Screen
void select_s() { //선택 화면
	puts("┌─────────────────────────┐");
	puts("│         ┌───────┐       │");
	puts("│   가로  │ 0     │  칸 < │");
	puts("│         └───────┘       │");
	puts("│         ┌───────┐       │");
	puts("│   세로  │ 0     │  칸   │");
	puts("│         └───────┘       │");
	puts("│         ┌───────┐       │");
	puts("│   지뢰  │ 0     │  개   │");
	puts("│         └───────┘       │");
	puts("│                         │");
	puts("│   space 키로 설정 전환  │");
	puts("│   가로 : 9 이상 60 이하 │");
	puts("│   세로 : 9 이상 60 이하 │");
	puts("│   지뢰 : 1개 이상       │");
	printf("└─────────────────────────┘");
} //Select_Screen
void map_s(int w, int h) {
	printf("┌"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┐\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("├"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┤\n");
	for (int i = 0; i < h; i++) {
		printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	}
	printf("├"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┤\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("│"); for (int i = 0; i <= w * 2; i++) { printf(" "); } printf("│\n");
	printf("└"); for (int i = 0; i <= w * 2; i++) { printf("─"); } printf("┘");
	gotoxy(2, 1); printf("▶ 깃발 : ");
	gotoxy(2, 2); printf("방향키로 커서 이동");
	gotoxy(2, h + 5); printf("1 : 클릭 모드");
	gotoxy(2, h + 6); printf("2 : 깃발 모드");
} //Map_Screen
void print(int k) { //출력
	switch (k) {
	    case 0:
			printf("□");
			break;
		case 1:
			printf("①");
			break;
		case 2:
			printf("②");
			break;
		case 3:
			printf("③");
			break;
		case 4:
			printf("④");
			break;
		case 5:
			printf("⑤");
			break;
		case 6:
			printf("⑥");
			break;
		case 7:
			printf("⑦");
			break;
		case 8:
			printf("⑧");
			break;
		case 9: {
			textcolor(14); //YELLOW
			printf("※");
			textcolor(7); //WHITEGRAY (기본)
			break;
		}
	}
}
void make_map(int w, int h, int cnt) {
	srand(time(NULL));
	while (cnt) { //지뢰 랜덤 배치
		int x = rand() % w;
		int y = rand() % h;
		if (mine[y][x] != 9) {
			mine[y][x] = 9;
			for (int i = y - 1; i <= y + 1; i++) {
				for (int j = x - 1; j <= x + 1; j++) {
					if (i < 0 || i >= h || j < 0 || j >= w) continue;
					if ((i == y && j == x) || (mine[i][j] == 9)) continue;
					mine[i][j]++;
				}
			}
			cnt--;
		}
	}
}
void erase(int x, int y) {
	gotoxy((1 + x) * 2, 4 + y);
	if (seen[y][x] == 0) printf("■");
	else if (seen[y][x] == -1) printf("▶");
	else print(mine[y][x]);
}
void dfs(int w, int h, int x, int y) { //안 열린 칸 클릭했을 때
	gotoxy((1 + x) * 2, 4 + y);
	if (seen[y][x] == -1) flag++;
	seen[y][x] = 1;
	print(mine[y][x]);
	hide--;
	if (mine[y][x] == 9) game_over = 1;
	if (mine[y][x] > 0) return;
	for (int i = 0; i < 4; i++) {
		int px = x + dx[i];
		int py = y + dy[i];
		if (px < 0 || px >= w || py < 0 || py >= h) continue;
		if (seen[py][px] == 1) continue;
		dfs(w, h, px, py);
	}
	return;
}
void minesweeper(int w, int h, int cnt) { //게임 작동
	system("cls");
	ConsoleSize(w, h);
	map_s(w, h);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			seen[i][j] = mine[i][j] = 0;
			gotoxy((1 + j) * 2, 4 + i);
			printf("■");
		}
	}
	make_map(w, h, cnt);
	flag = cnt;
	hide = w * h;
	gotoxy(12, 1);
	printf("%d", flag);
	int x = 0, y = 0, mode = 0, key;
	while (1) {
		gotoxy((1 + x) * 2, 4 + y);
		textcolor(10); //GREEN
		if (!mode) printf("▣");
		else printf("▶");
		textcolor(7); // WHITEGRAY
		key = getch(); //커서
		if (key == ESC) break;
		else if (key == 49) mode = 0;
		else if (key == 50) mode = 1;
		else if (key == 224) {
			key = getch();
			if (key == UP && y > 0) { //상하좌우 이동
				erase(x, y);
				y--;
			}
			else if (key == DOWN && y < h - 1) {
				erase(x, y);
				y++;
			}
			else if (key == LEFT && x > 0) {
				erase(x, y);
				x--;
			}
			else if (key == RIGHT && x < w - 1) {
				erase(x, y);
				x++;
			}
		}
		else if (key == SPACE) {
			if (mode == 0 && seen[y][x] == 0) {
				game_over = 0;
				dfs(w, h, x, y);
				if (game_over) {
					for (int i = 0; i < h; i++) {
						for (int j = 0; j < w; j++) {
							gotoxy((1 + j) * 2, 4 + i);
							if (mine[i][j] == 9) print(mine[i][j]);
						}
					}
					gotoxy(2, h + 5); textcolor(12); //RED
					printf("《 GAME OVER 》"); textcolor(7); // WHITEGRAY
					gotoxy(2, h + 6); printf("ESC 키로 메인화면");
					while (1) {
						key = getch();
						if (key == ESC) break;
					}
					break;
				}
				else if (hide == cnt) {
					gotoxy(2, h + 5); textcolor(9); //BLUE
					printf("《 GAME CLEAR 》"); textcolor(7); //WHITEGRAY
					gotoxy(2, h + 6); printf("ESC 키로 메인화면");
					while (1) {
						key = getch();
						if (key == ESC) break;
					}
					break;
				}
				else {
					gotoxy(12, 1);
					printf("%-4d", flag);
				}
			}
			else if (mode == 1 && seen[y][x] < 1) {
				gotoxy((1 + x) * 2, 4 + y);
				if (seen[y][x] == 0 && flag > 0) {
					flag--;
					seen[y][x] = -1;
					printf("▶");
					gotoxy(12, 1);
					printf("%-4d", flag);
				}
				else if (seen[y][x] == -1) {
					flag++;
					seen[y][x] = 0;
					printf("■");
					gotoxy(12, 1);
					printf("%-4d", flag);
				}
			}
		}
	}
} //게임 화면
int main() {
	SwitchingConsoleCursor(false); //커서 숨김
	int mode;
	while (1) {
		system("cls");
		system("mode con cols=39 lines=20"); //콘솔창 크기 조절
		main_s();
		fflush(stdin);
		mode = getch();
		if (mode == 49) {
			minesweeper(9, 9, 10); //초급
		}
		else if (mode == 50) {
			minesweeper(16, 16, 40); //중급
		}
		else if (mode == 51) {
			minesweeper(30, 16, 99); //고급
		}
		else if (mode == 52) { //커스텀
			int start = 0, w = 0, h = 0, cnt = 0, key = 1, sel = 1;
			system("cls");
			system("mode con cols=28 lines=16");
			select_s();
			while (1) {
				key = getch();
				if (key == ESC) break;
				else if (key == SPACE) { //설정 전환
					if (sel == 1 && w >= 9 && w <= 60) {
						sel++;
						gotoxy(24, 2);
						printf(" ");
						gotoxy(24, 5);
						printf("<");
					}
					else if (sel == 2 && h >= 9 && h <= 60) {
						sel++;
						gotoxy(24, 5);
						printf(" ");
						gotoxy(24, 8);
						printf("<");
						gotoxy(4, 11);
						printf("space 키로 게임 시작");
					}
					else if (sel == 3 && cnt > 0 && cnt < w * h) {
						start = 1;
						break;
					}
				}
				else if (key >= 48 && key <= 57) { //숫자 입력하기
					if (sel == 1) {
						if(w < 10) w = w * 10 + key - 48;
						gotoxy(12, 2);
						printf("%d", w);
					}
					if (sel == 2) {
						if (h < 10) h = h * 10 + key - 48;
						gotoxy(12, 5);
						printf("%d", h);
					}
					if (sel == 3) {
						if(cnt < 1000) cnt = cnt * 10 + key - 48;
						gotoxy(12, 8);
						printf("%d", cnt);
					}
				}
				else if (key == BACKSPACE) { //숫자 지우기
					if (sel == 1) {
						if (w < 10) {
							gotoxy(12, 2);
							printf("0");
						}
						else {
							gotoxy(13, 2);
							printf(" ");
						}
						w /= 10;
					}
					if (sel == 2) {
						if (h < 10) {
							gotoxy(12, 5);
							printf("0");
						}
						else {
							gotoxy(13, 5);
							printf(" ");
						}
						h /= 10;
					}
					if (sel == 3) {
						if (cnt < 10) {
							gotoxy(12, 8);
							printf("0");
						}
						else if(cnt < 100) {
							gotoxy(13, 8);
							printf(" ");
						}
						else if (cnt < 1000) {
							gotoxy(14, 8);
							printf(" ");
						}
						else {
							gotoxy(15, 8);
							printf(" ");
						}
						cnt /= 10;
					}
				}
			}
			if (start) minesweeper(w, h, cnt);
		}
	}
	return 0;
}