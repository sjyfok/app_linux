#include <stdio.h>
#include <time.h>

int main(void)
{
	time_t timCur = 0;
	time_t timEnd = 0;

	timCur = time((time_t*)0);
	printf("timCur = %d\n", (int)timCur);
	while (1) {
		timEnd = time((time_t*)0);
		if (timEnd != timCur)
		{
			printf("timCur = %d\n", (int)timCur);
			timCur = timEnd;
		}
	}

	return 0;
}
