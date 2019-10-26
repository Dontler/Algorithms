#include <stdio.h>
#include <time.h>

unsigned int seed = 1;

unsigned int lcg() {
	int a = 16807;
	unsigned int m = 0x7fffffff;

	seed = seed * a % m;
	return seed;
}

int main() {
	seed = time(0);
	while(1) {
		printf("%d", lcg() % 37);
		getchar();
	}
	return 0;
}
