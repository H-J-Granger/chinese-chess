#include "chessboard.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

void two_palyers() {
	using std::puts;
	using std::scanf;
	
	map.print();
	puts("�̷����ߣ��췽����");
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
				puts("�췽ѡ��Ҫ�ƶ������ӣ�");
				scanf("%d %d", &fromx, &fromy);
				fromx--; fromy--;
			} while (fromx >= HEIGHT || fromx < 0 || fromy >= WIDTH || fromy < 0 || map.data_color[fromx][fromy] != red);
			map.print(map.able_positions(fromx, fromy));
			while (true) {
				puts("�췽ѡ��Ҫ�ƶ�����λ�ã�");
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
				puts("�̷�ѡ��Ҫ�ƶ������ӣ�");
				scanf("%d %d", &fromx, &fromy);
				fromx--; fromy--;
			} while (fromx >= HEIGHT || fromx < 0 || fromy >= WIDTH || fromy < 0 || map.data_color[fromx][fromy] != green);
			map.print(map.able_positions(fromx, fromy));
			while (true) {
				puts("�̷�ѡ��Ҫ�ƶ�����λ�ã�");
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
	puts(map.status() == 1 ? "�췽��ʤ" : "�̷���ʤ");
}

int main(int argc, char** argv) {
	two_palyers();
	return 0;
}
