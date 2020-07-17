#ifndef CONTROL_H_
#define CONTROL_H_

#include <cstdlib>


#ifdef _WIN32
constexpr int RED = 0x0C, GREEN = 0x0A, RED_OVER_WHITE = 0x7C, YELLOW_OVER_WHITE = 0x7F, GREEN_OVER_WHITE = 0x7C;
#include <Windows.h>

class FontColor {
public:
	FontColor() {
		m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	
	void SetColor(int nColor) {
		SetConsoleTextAttribute(m_hStdOut, nColor);
	}
	void UnSetColor() {
		SetConsoleTextAttribute(m_hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	
	~FontColor() {}
private:
	HANDLE m_hStdOut;
};

void clear() {
	std::system("cls");
}

void sleep(int n) {
	Sleep(n);
}

#else
#include <unistd.h>

constexpr char GREEN[] = "0;32", RED[] = "0;31", YELLOW_OVER_WHITE[] = "47;33", GREEN_OVER_WHITE[] = "47;32", RED_OVER_WHITE[] = "47;31";

class FontColor {
public:
	FontColor() {
		m_bUnset = true;
	}
	
	void SetColor(const char* nColor) {
		printf("\033[%sm", nColor);
		m_bUnset = false;
	}
	
	void UnSetColor() {
		if(!m_bUnset) {
			printf("\033[0m");
			m_bUnset = true;
		}
	}
	
	~FontColor() {
		UnSetColor();
	}
	
private:
	bool m_bUnset;
};

void clear() {
	std::system("clear");
}

void sleep(int n) {
	usleep(n);
}

#endif
FontColor console;

#endif // #ifndef COLOR_H_
