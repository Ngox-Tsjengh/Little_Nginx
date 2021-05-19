#include <stdio.h>
void panic_spin(char* filename,			\
				int line,				\
				const char* func,		\
				const char* condition)	\
{
	printf("\n\n\n!!!!! error !!!!!\n");
	printf("filename: %s\n", filename);
	printf("line:%d\n",line);
	printf("function: %s\n",(char*)func);
	printf("condition: %s\n",(char*)condition);
	while(1);
}
