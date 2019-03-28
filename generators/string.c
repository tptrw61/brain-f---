#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	int q, r, i;
	int c;
	printf("string (\n");
	printf("[-]>[-]\n");
	while ((c = getchar()) != EOF) {
		if (c == 0)
			continue;
		//notation
		if (c == 32)
			printf("   SPC ");
		else if (c == 10)
			printf("   LF  ");
		else if (c >= 127 || c < 32)
			printf("  %4d  ", c);
		else
			printf("%c %4d ", c, c);
		//end of notation
		if (c < 0x10) {
			for (i = 0; i < c; i++)
				putchar('+');
			printf(">[-]");
		} else if (c < 0x20) {
			q = c / 4;
			r = c % 4;
			for (i = 0; i < r; i++)
				putchar('+');
			printf(">[-]");
			for (i = 0; i < q; i++)
				putchar('+');
			printf("[<++++>-]");
		} else if (c < 0x40) {
			q = c / 8;
			r = c % 8;
			char x = '+';
			if (r > 4) {
				r = 8 - r;
				q++;
				x = '-';
			}
			for (i = 0; i < r; i++)
				putchar(x);
			printf(">[-]");
			for (i = 0; i < q; i++)
				putchar('+');
			printf("[<++++++++>-]");
		} else {
			q = c / 16;
			r = c % 16;
			char x = '+';
			if (r > 8) {
				r = 16 - r;
				q++;
				x = '-';
			}
			for (i = 0; i < r; i++)
				putchar(x);
			printf(">[-]");
			for (i = 0; i < q; i++)
				putchar('+');
			printf("[<++++++++++++++++>-]");
		}
		putchar(10); //newline
	}
	printf("<[<]\n");
	printf(")\n");
	return 0;
}
