#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>



void tokenize(FILE *f, )
{
	char happy[20];
	FILE *myfile = fopen("testplan.txt", "r");

	fscanf(myfile, "%[^a-z]", &happy);
	fscanf(myfile, "%[a-z]", &happy);
	printf("%s\n", happy);
	fscanf(myfile, "%[^a-z]", &happy);
	fscanf(myfile, "%[a-z]", &happy);
	printf("%s\n", happy);
	//fscanf(myfile, "%[a-z]", &happy);
	//printf("%s\n", happy);
	return 0;

}
