//#include "my_driver.S"

char _my_getchar();
void _my_put_char(char);


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
		ten /= 2;
	}
	i--;
	while (i >= 0) {
		_my_putchar(i - '0');
		i--;
	}
}

