#include <stdio.h>

int main()
{
	int arr[10][20];
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			arr[i][j] = i +j;
		}
	}
	printf("%d",**arr);
}
