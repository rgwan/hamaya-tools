#include <stdio.h>
#include <stdlib.h>

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

	unsigned char *bufferh,*bufferl,*buffero;
	unsigned char s;
	size_t sizeh,sizel,sizeo,i;
	
	if(argc<3)
	{
		printf("Usage:%s <In-File-H> <In-File-L> <Out-File>\n",argv[0]);
		printf("This tool can help you to merge hi-lo to one binary file\n");
		exit(0);
	}
	
	bufferh = readFiletoBuffer(&sizeh,argv[1]);
	bufferl = readFiletoBuffer(&sizel,argv[2]);
	if (sizeh != sizel)
	{
		printf("File length isn't equal! aborting!\n");
		exit(0);
	}
	printf("Input File size: H %d bytes, L %d bytes\n",sizeh,sizel);
	//endianConvert(bufferh,sizeh);
	//endianConvert(bufferl,sizel);
	sizeo=sizeh + sizel;
	buffero = malloc(sizeo);
	
	for(i=0;i<=sizel;i+=2)
	{
		buffero[2*i+3] = bufferl[i];
		buffero[2*i+2] = bufferl[i+1];
		buffero[2*i+1] = bufferh[i];
		buffero[2*i+0] = bufferh[i+1];
	}
	writeBuffertoFile(buffero,sizeo,argv[3]);
	printf("Completed write:%s\n",argv[3]);	
	free(bufferh);
	free(bufferl);
	free(buffero);
}