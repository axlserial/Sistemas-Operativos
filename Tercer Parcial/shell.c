#include <stdio.h>
#define MAX_LONG 255

int main() {
	char buffer[MAX_LONG];

	printf("$> ");
	fgets(buffer, MAX_LONG, stdin);
	printf("$> %s", buffer);

	return 0;
}