#include <stdio.h>
#include <string.h>

void kmp(char *target, char *query, int *result, int *failf) {
	int l = strlen(target);
	for (int i = 0, j = 0; i<l; i++) {
		if (target[i] != query[j]) {
			if (j == 0)result[i] = 0;
			else {
				i--;
				j = failf[j - 1];
			}
		}
		else result[i] = ++j;
	}
}