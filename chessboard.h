#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include <cstdio>
#include "control.h"
#include <vector>
#include <cstdlib>
#include <cstring>

enum chess_type {
	null,
	ju,
	ma,
	xiang,
	shi,
	jiang,
	zu,
	pao
};

enum chess_color{
	white,
	red,
	green
};

constexpr int WIDTH = 9, HEIGHT = 10;

struct Map {
	struct Move {
		int fromx, fromy;
		int tox, toy;
		chess_type eaten_type;
		chess_color eaten_color;
	};
	
	struct Stack {
		std::vector<Map::Move> data;
		void push(Map::Move x) {
			data.push_back(x);
		}
		void pop() {
			if (data.empty()) {
				throw "size error!";
			}
			data.pop_back();
		}
		Map::Move top() const {
			if (data.empty()) {
				throw "size error!";
			}
			return data.back();
		}
		Move(int fx=0,int fy=0,int tx=0,int ty=0,chess_type et=null,chess_color ec=white){
			fromx=fx;fromy=fy;
			tox=tx;toy=ty;
			eaten_type=et;eaten_color=ec;
		}
	} stack;
	
	chess_type data_type[HEIGHT][WIDTH] = {
		{ju, ma, xiang, shi, jiang, shi, xiang, ma, ju},
		{},
		{null, pao, null, null, null, null, null, pao, null},
		{zu, null, zu, null, zu, null, zu, null, zu},
		{},
		{},
		{zu, null, zu, null, zu, null, zu, null, zu},
		{null, pao, null, null, null, null, null, pao, null},
		{},
		{ju, ma, xiang, shi, jiang, shi, xiang, ma, ju}
	};
	chess_color data_color[HEIGHT][WIDTH];
	
	Map() {
		for (int i = 0; i < HEIGHT; ++i) {
			for (int j = 0; j < WIDTH; ++j) {
				if (data_type[i][j] != null) {
					data_color[i][j] = i < HEIGHT / 2 ? red : green;
				}
			}
		}
	}
	
	bool tag[HEIGHT][WIDTH];
	
	std::vector<std::pair<int, int>> able_positions(int x, int y);
	void print(std::vector<std::pair<int, int>> V = std::vector<std::pair<int, int>>());
	
	void move(int fromx, int fromy, int tox, int toy) {
		if (fromx < 0 || fromx >= HEIGHT || fromy < 0 || fromy >= WIDTH ||
			tox < 0 || tox >= HEIGHT || toy < 0 || toy >= WIDTH) {
			throw "out of range!";
		}
		auto able = able_positions(fromx, fromy);
		for (auto now : able) {
			//std::printf("(%d, %d) ", now.first, now.second);
			if (now.first == tox && now.second == toy) {
				stack.push(Map::Move(fromx, fromy, tox, toy, data_type[tox][toy], data_color[tox][toy]));
				data_type[tox][toy] = data_type[fromx][fromy];
				data_color[tox][toy] = data_color[fromx][fromy];
				data_type[fromx][fromy] = null;
				data_color[fromx][fromy] = white;
				return;
			}
		}
		throw "can't move!";
		return;
	}
	
	void move(Move mov) {
		move(mov.fromx, mov.fromy, mov.tox, mov.toy);
	}
	
	void undo() {
		Map::Move mov = stack.top();
		data_type[mov.fromx][mov.fromy] = data_type[mov.tox][mov.toy];
		data_color[mov.fromx][mov.fromy] = data_color[mov.tox][mov.toy];
		data_type[mov.tox][mov.toy] = mov.eaten_type;
		data_color[mov.tox][mov.toy] = mov.eaten_color;
		stack.pop();
	}
	
	int status() {
		bool tag[2] = {0, 0};
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if (data_type[i][j] == jiang) {
					tag[data_color[i][j] != green] = 1;
				}
			}
		}
		if (!tag[0]) {
			return 1;
		}
		if (!tag[1]) {
			return -1;
		}
		return 0;
	}
} map;


std::vector<std::pair<int, int>> Map::able_positions(int x, int y) {
	std::vector<std::pair<int, int>> res;
	if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
		return res;
	}
	switch (data_type[x][y]) {
		case null: {
			break;
		}
		case ju: {
			for (int i = x + 1; i < HEIGHT; ++i) {
				if (data_color[i][y] == data_color[x][y]) {
					break;
				} else {
					res.push_back({i, y});
					if (data_color[i][y] != white) {
						break;
					}
				}
			}
			for (int i = x - 1; i >= 0; --i) {
				if (data_color[i][y] == data_color[x][y]) {
					break;
				} else {
					res.push_back({i, y});
					if (data_color[i][y] != white) {
						break;
					}
				}
			}
			for (int i = y + 1; i < WIDTH; ++i) {
				if (data_color[x][i] == data_color[x][y]) {
					break;
				} else {
					res.push_back({x, i});
					if (data_color[x][i] != white) {
						break;
					}
				}
			}
			for (int i = y - 1; i >= 0; --i) {
				if (data_color[x][i] == data_color[x][y]) {
					break;
				} else {
					res.push_back({x, i});
					if (data_color[x][i] != white) {
						break;
					}
				}
			}
			break;
		}
		case ma: {
			if (y + 2 < WIDTH && data_color[x][y + 1] == white) {
				if (x + 1 < HEIGHT && data_color[x + 1][y + 2] != data_color[x][y]) {
					res.push_back({x + 1, y + 2});
				}
				if (x - 1 >= 0 && data_color[x - 1][y + 2] != data_color[x][y]) {
					res.push_back({x - 1, y + 2});
				}
			}
			if (y - 2 >= 0 && data_color[x][y - 1] == white) {
				if (x + 1 < HEIGHT && data_color[x + 1][y - 2] != data_color[x][y]) {
					res.push_back({x + 1, y - 2});
				}
				if (x - 1 >= 0 && data_color[x - 1][y - 2] != data_color[x][y]) {
					res.push_back({x - 1, y - 2});
				}
			}
			if (x + 2 < HEIGHT && data_color[x + 1][y] == white) {
				if (y + 1 < WIDTH && data_color[x + 2][y + 1] != data_color[x][y]) {
					res.push_back({x + 2, y + 1});
				}
				if (y - 1 >= 0 && data_color[x + 2][y - 1] != data_color[x][y]) {
					res.push_back({x + 2, y - 1});
				}
			}
			if (x - 2 >= 0 && data_color[x - 1][y] == white) {
				if (y + 1 < WIDTH && data_color[x - 2][y + 1] != data_color[x][y]) {
					res.push_back({x - 2, y + 1});
				}
				if (y - 1 >= 0 && data_color[x - 2][y - 1] != data_color[x][y]) {
					res.push_back({x - 2, y - 1});
				}
			}
			break;
		}
		case pao: {
			bool jumped = false;
			for (int i = x + 1; i < HEIGHT; ++i) {
				if (!jumped) {
					if (data_type[i][y] != null) {
						jumped = true;
					} else {
						res.push_back({i, y});
					}
				} else {
					if (data_type[i][y] == null) {
						continue;
					}
					if (data_color[i][y] != data_color[x][y]) {
						res.push_back({i, y});
					}
					break;
				}
			}
			jumped = false;
			for (int i = x - 1; i >= 0; --i) {
				if (!jumped) {
					if (data_type[i][y] != null) {
						jumped = true;
					} else {
						res.push_back({i, y});
					}
				} else {
					if (data_type[i][y] == null) {
						continue;
					}
					if (data_color[i][y] != data_color[x][y]) {
						res.push_back({i, y});
					}
					break;
				}
			}
			jumped = false;
			for (int i = y + 1; i < WIDTH; ++i) {
				if (!jumped) {
					if (data_type[x][i] != null) {
						jumped = true;
					} else {
						res.push_back({x, i});
					}
				} else {
					if (data_type[x][i] == null) {
						continue;
					}
					if (data_color[x][i] != data_color[x][y]) {
						res.push_back({x, i});
					}
					break;
				}
			}
			jumped = false;
			for (int i = y - 1; i >= 0; --i) {
				if (!jumped) {
					if (data_type[x][i] != null) {
						jumped = true;
					} else {
						res.push_back({x, i});
					}
				} else {
					if (data_type[x][i] == null) {
						continue;
					}
					if (data_color[x][i] != data_color[x][y]) {
						res.push_back({x, i});
					}
					break;
				}
			}
			break;
		}
		case xiang: {
			if (x + 2 < HEIGHT && y + 2 < WIDTH && data_type[x + 1][y + 1] == null && data_type[x + 2][y + 2] != data_type[x][y] && x != 4) {
				res.push_back({x + 2, y + 2});
			}
			if (x + 2 < HEIGHT && y - 2 >= 0 && data_type[x + 1][y - 1] == null && data_type[x + 2][y - 2] != data_type[x][y] && x != 4) {
				res.push_back({x + 2, y - 2});
			}
			if (x - 2 >= 0 && y + 2 < WIDTH && data_type[x - 1][y + 1] == null && data_type[x - 2][y + 2] != data_type[x][y] && x != 5) {
				res.push_back({x - 2, y + 2});
			}
			if (x - 2 >= 0 && y - 2 >= 0 && data_type[x - 1][y - 1] == null && data_type[x - 2][y - 2] != data_type[x][y] && x != 5) {
				res.push_back({x - 2, y - 2});
			}
			break;
		}
		case zu: {
			if (data_color[x][y] == red) {
				if (x + 1 < HEIGHT && data_color[x][y] != data_color[x + 1][y]) {
					res.push_back({x + 1, y});
				}
				if (x > 4) {
					if (y - 1 >= 0 && data_color[x][y] != data_color[x][y - 1]) {
						res.push_back({x, y - 1});
					}
					if (y + 1 < WIDTH && data_color[x][y] != data_color[x][y + 1]) {
						res.push_back({x, y + 1});
					}
				}
			} else {
				if (x - 1 >= 0 && data_color[x][y] != data_color[x - 1][y]) {
					res.push_back({x - 1, y});
				}
				if (x < 5) {
					if (y - 1 >= 0 && data_color[x][y] != data_color[x][y - 1]) {
						res.push_back({x, y - 1});
					}
					if (y + 1 < WIDTH && data_color[x][y] != data_color[x][y + 1]) {
						res.push_back({x, y + 1});
					}
				}
			}
			break;
		}
		case shi: {
			auto check = data_color[x][y] == red ?
				[](int x, int y) {
					return 0 <= x && x <= 2 && 3 <= y && y <= 5;
				}
				: [](int x, int y) {
					return 7 <= x && x <= 9 && 3 <= y && y <= 5;
				}
			;
			for (int dx = -1; dx != 3; dx += 2) {
				for (int dy = -1; dy != 3; dy += 2) {
					if (check(x + dx, y + dy) && data_color[x + dx][y + dy] != data_color[x][y]) {
						res.push_back({x + dx, y + dy});
					}
				}
			}
			break;
		}
		case jiang: {
			auto check = data_color[x][y] == red ?
				[](int x, int y) {
					return 0 <= x && x <= 2 && 3 <= y && y <= 5;
				}
				: [](int x, int y) {
					return 7 <= x && x <= 9 && 3 <= y && y <= 5;
				}
			;
			int delta[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
			for (int i = 0; i < 4; ++i) {
				int dx = delta[i][0], dy = delta[i][1];
				if (check(x + dx, y + dy) && data_color[x + dx][y + dy] != data_color[x][y]) {
					res.push_back({x + dx, y + dy});
				}
			}
			break;
		}
	}
	return res;
}

void Map::print(std::vector<std::pair<int, int>> V) {
	using std::puts;
	using std::memset;
	using std::printf;
	
	clear();
	
	console.UnSetColor();
	puts("   1  2  3  4  5  6  7  8  9");
	puts("  ----------------------------");
	memset(tag, 0, sizeof(tag));
	
	for (auto i : V) {
		tag[i.first][i.second] = true;
	}
	for (int i = 0; i < HEIGHT; ++i) {
		console.UnSetColor();
		
		if (i != HEIGHT - 1) {
			printf(" %d|", i + 1);
		} else {
			printf("%d|", i + 1);
		}
		
		for (int j = 0; j < WIDTH; ++j) {
			if(!tag[i][j]) {
				switch (data_color[i][j]) {
					case white: {
						console.UnSetColor();
						break;
					}
					case green: {
						console.SetColor(GREEN);
						break;
					}
					case red: {
						console.SetColor(RED);
						break;
					}
				}
			} else {
				switch (data_color[i][j]) {
					case white: {
						console.SetColor(YELLOW_OVER_WHITE);
						break;
					}
					case green: {
						console.SetColor(GREEN_OVER_WHITE);
						break;
					}
					case red: {
						console.SetColor(RED_OVER_WHITE);
						break;
					}
				}
			}
			
			switch (data_type[i][j]) {
				case ju: {
					printf("车");
					break;
				}
				case ma: {
					printf("马");
					break;
				}
				case pao: {
					printf("炮");
					break;
				}
				case shi: {
					printf("士");
					break;
				}
				case jiang: {
					printf("将");
					break;
				}
				case zu: {
					printf("卒");
					break;
				}
				case null: {
					printf("  ");
					break;
				}
				case xiang: {
					printf("象");
					break;
				}
			}
			console.UnSetColor();
			printf("|");
		}
		puts("");
		if(i+1==HEIGHT/2) puts("  ============================");
		else puts("  ----------------------------");
	}
	console.UnSetColor();
}

#endif // #ifndef CHESSBOARD_H
