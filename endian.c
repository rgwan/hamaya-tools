#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	FILE *fp;
	unsigned char *buffer;
	unsigned char s;
	size_t size,i;
	
	if(argc<2)
	{
		printf("Usage:%s <In-File> <Out-File>\n",argv[0]);
		printf("This tool can help you to invert endian on WORD\n");
		exit(0);
	}
	
	fp = fopen(argv[1],"rb");
	if(fp == NULL) exit(0);
	
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	rewind(fp);

	printf("File size:%d\n",size);	
	buffer = malloc(size);
	fread(buffer,1,size,fp);

	fclose(fp);
	
	for(i=0;i<=size;i+=2)
	{
		s=buffer[i];
		buffer[i]=buffer[i+1];
		buffer[i+1]=s;
	}
	
	fp = fopen(argv[2],"wb+");
	if(fp == NULL) exit(0);
	
	fwrite(buffer,1,size,fp);

	fclose(fp);
	printf("Completed write:%s\n",argv[2]);	
	free(buffer);
}