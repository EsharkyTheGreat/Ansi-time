#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

void reset();
void update(char *matrix, int *len);

// GLOBAL VARIABLES
int ROW, COL;
float TIME;

void goodbye()
{
	puts("\x1b[0m\x1b[?25h");
	system("clear");
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		puts("Usage: matrix.out <time>");
		return 1;
	}
	signal(SIGINT, goodbye);
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	ROW = w.ws_row;
	COL = w.ws_col;
	TIME = atoi(argv[1]) * 1000;

	const int MAT = (COL + 1) * ROW;
	char matrix[MAT];
	system("clear");
	int len[COL];
	for (int i = 0; i < COL; ++i)
		len[i] = 0;
	for (int i = 0; i < MAT; ++i)
		matrix[i] = ' ';
	for (int i = 0; i < ROW; ++i)
		matrix[COL + (COL + 1) * i] = '\n';
	puts("\x1b[38;2;100;230;110m\x1b[?25l");
	while (1)
	{
		reset();
		printf("%s", matrix);
		update(matrix, len);
		usleep(TIME);
	}

	return 0;
}

void reset()
{
	printf("\x1b[%dF", ROW);
}

void update(char *matrix, int *len)
{
	const char SPACE = ' ';
	// Move down
	for (int i = ROW - 2; i >= 0; --i)
	{
		for (int j = 0; j < COL; ++j)
		{
			matrix[(i + 1) * (COL + 1) + j] = matrix[i * (COL + 1) + j];
		}
	}
	for (int i = 0; i < COL; ++i)
	{
		if (len[i]-- > 0)
		{
			matrix[i] = (char)65 + rand() % 48;
		}
		else
		{
			if (rand() % 40 > 38)
			{
				len[i] = ROW / 4 + rand() % (ROW / 2);
			}
			matrix[i] = SPACE;
		}
	}
}
