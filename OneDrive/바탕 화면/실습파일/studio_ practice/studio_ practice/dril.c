#include <stdio.h>
int main(void)
{
	int num = 0, total = 0;



	while (num != 1) {
		printf("°ª:");
		scanf("%d", &num);
		total = total + num;
	}

	printf("%d", total);
	return 0;
}


