#include <stdio.h>
int main(int argc, const char *argv[])
{
	union Data{
	int a;
	char b;
	}data;
	data.a=0x1234;
	printf("%c",data.b);
	if(data.b==0x12)
	printf("1");
	else
	printf("0");
	return 0;
}
