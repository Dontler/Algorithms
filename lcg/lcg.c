#include <stdio.h>
#include <time.h>

unsigned int seed = 1;

unsigned int lcg() {
	int a = 16807;
	unsigned int m = 0x7fffffff;
	//Values a and m are compiler dependent
	seed = seed * a % m;
	return seed;
}

int main() {
	seed = time(0);
	printf("%d\n", lcg() % 100); // Generate a random number in the range: [0, 99]
	return 0;
}
