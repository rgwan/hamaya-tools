#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *readFiletoBuffer(size_t *size, unsigned char *fileName)
{
	FILE *fp;
	unsigned char *buffer;
	fp = fopen(fileName,"rb");
	if(fp == NULL) exit(0);
	
	fseek(fp,0,SEEK_END);
	*size = ftell(fp);
	rewind(fp);

	buffer = malloc(*size);
	fread(buffer,1,*size,fp);

	fclose(fp);
	return buffer;
}

int writeBuffertoFile(unsigned char *buffer, size_t size, unsigned char *fileName)
{
	FILE *fp;
	fp = fopen(fileName,"wb+");
	if(fp == NULL) exit(0);
	
	fwrite(buffer,1,size,fp);

	fclose(fp);
}

int main(int argc,char *argv[])
{

	unsigned char *buffer;
	unsigned char seg_title[16]="XXXX.dat";
	size_t size, seg_size, fptr = 0;
	
	if(argc<2)
	{
		printf("Usage:%s <C02-File> <Out-Directory>\n",argv[0]);
		printf("This tool can help you unpack the c02 file\n");
		exit(0);
	}
	
	buffer = readFiletoBuffer(&size,argv[1]);
	printf("File %s, Length %d\n",argv[1],size);
	
	while(fptr < size)
	{
		memcpy(seg_title, &buffer[fptr], 4);
		seg_size = buffer[fptr + 4] * 0x1000000 + buffer[fptr + 5] * 0x10000 + \
			buffer[fptr + 6] * 0x100 + buffer[fptr + 7];
		printf("Found Segment %s. size %d\n", seg_title, seg_size);
		writeBuffertoFile(&buffer[fptr], seg_size + 8, seg_title);
		fptr += seg_size + 8;

	}
}