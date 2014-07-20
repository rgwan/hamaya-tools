#include <stdio.h>
#include <stdlib.h>
unsigned char PSR_Magic[]="PSR-SLX";
unsigned char SONG_Magic[]="SONG";
unsigned char Style_Magic[]="STYLE";
unsigned char Song_1[]="Overview";
unsigned char Data_Magic[]="EvtD";

#define TRUE 1
#define FALSE -1
size_t size;
unsigned char *buffer;

int readFiletoBuffer(char *fileName)
{
	FILE *fp;
	fp = fopen(fileName,"rb");
	if(fp == NULL) return FALSE;
	
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	rewind(fp);

	buffer = malloc(size);
	fread(buffer,1,size,fp);

	fclose(fp);
	return TRUE;
}

int writeBuffertoFile(char *fileName, unsigned char *write_buffer, size_t length)
{
	FILE * fp;
	fp = fopen(fileName,"wb+");
	if(fp == NULL) return FALSE;
	
	fwrite(write_buffer,1,length,fp);

	fclose(fp);
	return TRUE;
}

size_t findParts(unsigned char *whole, unsigned char *part, size_t from, size_t size_whole, size_t size_part)
{
	size_t i, j;
	size_t end = size_whole - size_part;
	size_t partlen = size_part;
	for(i = from;i <= end;i ++)
	{
		for(j = 0;j < partlen;j ++)
			if(part[j] != whole[i + j])
				goto cont;
		return i;
		cont:;
	}
	return FALSE;
}

int main(int argc,char *argv[])
{
	size_t slx;
	size_t slx_o;
	size_t slx_p;
	size_t song_p, song_1, song_size, song_offset;
	size_t style_p;
	size_t data_p;
	int i;
	
	if(argc<2)
	{
		printf("Usage:%s <In-File> \n",argv[0]);
		printf("This tool can help you exclude files\n");
		exit(0);
	}
	if(readFiletoBuffer(argv[1]) == FALSE)
	{
		printf("Can not open file!\n");
		exit(0);
	}
	printf("File size:%d\n",size);
	
	// Try to find SLX-Parts
	slx_o = 0;
	while(1)
	{
		slx = findParts(buffer, PSR_Magic, slx_o,size ,sizeof(PSR_Magic) - 1);
		if (slx == FALSE) break;
		printf ("Found PSR-SLX parts in offset %d ",slx);
		if(buffer[slx + 11] == 0x4F)
			printf ("Type Boot_Block");
		else if(buffer[slx + 11] == 0x20)
		{
			printf ("Type Data_Block");
			slx_p = slx;
		}
		putchar('\n');
		
		slx_o = slx + 1;
	}
	putchar('\n');
	//Try to find Song-Parts
	song_p = findParts(buffer, SONG_Magic, slx_p,size ,sizeof(SONG_Magic) - 1);
	if(song_p == FALSE)
	{
		printf("Unable to locate SONG segment!\n");
		exit(0);
	}
	style_p = findParts(buffer, Style_Magic, song_p,size ,sizeof(Style_Magic) - 1);
	if(style_p == FALSE)
	{
		printf("Unable to locate Style segment!\n");
		exit(0);
	}
	data_p = findParts(buffer, Data_Magic, style_p,size ,sizeof(Data_Magic) - 1);
	if(data_p == FALSE)
	{
		printf("Unable to locate Data segment!\n");
		exit(0);
	}
	printf("Find Song in offset %d, Style in offset %d, Data in offset %d\n",song_p,style_p,data_p);
	song_1 = findParts(buffer, Song_1, style_p,size ,sizeof(Song_1) - 1);
	if(song_1 == FALSE)
	{
		printf("Unable to locate Song1 segment!\n");
		exit(0);
	}	
	for(i=0;i<10;i++)
	{
		song_size = buffer[song_1 + i * 88 + 75] *0x10000 + buffer[song_1 + i * 88 + 76] *0x100 + buffer[song_1 + i * 88 + 77];
		song_offset = data_p + buffer[data_p + 9 + 4 * i] *0x10000 + buffer[data_p + 10 + 4 * i] *0x100 + buffer[data_p + 11 + 4 * i];
		printf("%s , size %d bytes, offset = %d\n",&buffer[song_1 + i * 88], song_size, song_offset);
		writeBuffertoFile(&buffer[song_1 + i * 88],&buffer[song_offset],song_size);
	}
	free(buffer);

}