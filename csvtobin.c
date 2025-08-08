#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	FILE *fp;
    FILE *fpcsv;
	unsigned char *buffer;
	unsigned char s;
	size_t size,i;
	
	if(argc<2)
	{
		printf("Usage:%s <In-File> <Out-File>\n",argv[0]);
		printf("This tool can help you convert DSView's csv to bin file\n");
		exit(0);
	}
    char line[1024];
    fpcsv = fopen(argv[1], "r+");
    fp = fopen(argv[2], "wb+");
    do
    {
        fgets(line, sizeof(line), fpcsv);
        if (feof(fpcsv))
            break;

        int ssize = strlen(line);
        if (ssize < 4)
            break;

        uint32_t dat;
        sscanf(&line[ssize - 4], "%02x", &dat);
        uint8_t bdat;
        bdat = dat & 0xff;
        fwrite(&bdat, 1, 1, fp);
        // printf("%02x\n", dat);s
    } while (!feof(fpcsv));
    fclose(fpcsv);
    fclose(fp);
}
