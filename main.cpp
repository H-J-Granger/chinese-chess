/*
 * @Author: BilyHurington
 * @Date: 2020-07-19 12:45:28
 * @LastEditors: BilyHurington
 * @LastEditTime: 2020-07-19 21:54:49
 */ 
#include "chessboard.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "chessrobot.h"

void two_palyers() {
	using std::puts;
	using std::scanf;
	
	map.print();
	puts("绿方先走，红方后走");
	sleep(1000);
	bool now = 0;
	
	while (!map.status()) {
		clear();
		now ^= 1;
		int fromx, fromy, tox, toy;
		if (!now) {
			do {
				clear();
				map.print();
				puts("红方选择要移动的棋子：");
				scanf("%d %d", &fromx, &fromy);
				fromx--; fromy--;
			} while (fromx >= HEIGHT || fromx < 0 || fromy >= WIDTH || fromy < 0 || map.data_color[fromx][fromy] != red);
			map.print(map.able_positions(fromx, fromy));
			while (true) {
				puts("红方选择要移动到的位置：");
				scanf("%d %d", &tox, &toy);
				tox--; toy--;
				try {
					map.move(fromx, fromy, tox, toy);
				} catch (const char *s) {
					continue;
				}
				break;
			}
			map.print();
		} else {
			do {
				clear();
				map.print();
				puts("绿方选择要移动的棋子：");
				scanf("%d %d", &fromx, &fromy);
				fromx--; fromy--;
			} while (fromx >= HEIGHT || fromx < 0 || fromy >= WIDTH || fromy < 0 || map.data_color[fromx][fromy] != green);
			map.print(map.able_positions(fromx, fromy));
			while (true) {
				puts("绿方选择要移动到的位置：");
				scanf("%d %d", &tox, &toy);
				tox--; toy--;
				try {
					map.move(fromx, fromy, tox, toy);
				} catch (const char *s) {
					continue;
				}
				break;
			}
			map.print();
		}
	}
	clear();
	map.print();
	puts(map.status() == 1 ? "红方获胜" : "绿方获胜");
}

void one_palyer() {
	using std::puts;
	using std::scanf;
	
	map.print();
	puts("绿方先走，红方后走");
	sleep(1000);
	bool now = 0;
	
	while (!map.status()) {
		now ^= 1;
		int fromx, fromy, tox, toy;
		if (!now) {
			map.move(robot_choose());
			map.print();
		} else {
			do {
				map.print();
				puts("绿方选择要移动的棋子：");
				scanf("%d %d", &fromx, &fromy);
				fromx--; fromy--;
			} while (fromx >= HEIGHT || fromx < 0 || fromy >= WIDTH || fromy < 0 || map.data_color[fromx][fromy] != green);
			map.print(map.able_positions(fromx, fromy));
			while (true) {
				puts("绿方选择要移动到的位置：");
				scanf("%d %d", &tox, &toy);
				tox--; toy--;
				try {
					map.move(fromx, fromy, tox, toy);
				} catch (const char *s) {
					continue;
				}
				break;
			}
			map.print();
		}
	}
	map.print();
	puts(map.status() == 1 ? "红方获胜" : "绿方获胜");
}

int main(int argc, char** argv) {
	one_palyer();
	return 0;
}
