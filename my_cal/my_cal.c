#include "my_driver.S"

void my_cal() {
	char c;
	int ten = 0;
	char binary[512] = {};
	int i = 0;

	while ((c = _my_getchar()) != '\n') {
		ten = ten * 10 + (c - '0');
	}

	while (ten > 0) {
		binary[i++] = ten % 2;
		tem /= 2;
	}
	i--;
	while (i >= 0) {
		_my_putchar(i - '0');
		i--;
	}
}

