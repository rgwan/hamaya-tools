#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


int isValidChunkBuf(uint8_t *buf)
{
    uint8_t *validHeaders[] = 
    {
        "MTrk",
        "XFIH",
        "XFKM",
        0
    };
    int i = 0;
    int ret = 0;
    while(validHeaders[i] != 0)
    {
        if(memcmp(buf, validHeaders[i], 4) == 0)
            ret = 1;
        i++;
    }
    return ret;
}

void dumpMIDI(uint8_t *buf, size_t offset, char *rec_name)
{
    uint32_t chunkSize;
    uint32_t *midiChunkSize;
    
    size_t midiSize = 0;
    uint8_t *hdrBuf = buf;
    uint8_t foundname = 0;

    char fileTitle[256] = "UNTITLED";
    char fileName[256];

    do
    {
        midiChunkSize = (uint32_t *)(hdrBuf + 4);
        chunkSize = htonl(*midiChunkSize);
        //printf("chunkSize %d\n", chunkSize);

        if(foundname == 0 && memcmp(hdrBuf, "MTrk", 4) == 0)
        {
            foundname = 1;
            uint8_t *pFileTit;
            if(hdrBuf[16] & 0x80 == 0 || hdrBuf[12] == 0)
            {
                pFileTit = hdrBuf + 16;
            }
            else
                pFileTit = hdrBuf + 12;

            strcpy(fileTitle, pFileTit);
        }
        midiSize += chunkSize + 8; // for Header
        hdrBuf += chunkSize + 8;

    } while(isValidChunkBuf(hdrBuf));


    if(foundname == 0)
    {
        return ;
    }
    if (rec_name)
        strcpy(fileTitle, rec_name);

    int i;
    for(i = 0; i < strlen(fileTitle); i++)
    {
        if(fileTitle[i] & 0x80)
        {
            strcpy(fileTitle, "UNTITLED");
            break;
        }
        if(fileTitle[i] == '\r' || fileTitle[i] == '\n')
        {
            fileTitle[i] = '0';
            break;
        }
    }

    printf("fileTitle = '%s', size = %d\n", fileTitle, midiSize);
    snprintf(fileName, sizeof(fileName), "%s_%08x.mid", fileTitle, offset);

    printf("Save to file '%s'\n", fileName);
    FILE *fpo = fopen(fileName, "wb+");
    fwrite(buf, 1, midiSize, fpo);
    fclose(fpo);
}

static int songno = 0;
//int fat = 0x1b40f6a - 88;

char *find_songname_psre(uint8_t *buffer, int songno)
{
    int name_ofs = 0x54bbc;
    char *ret = buffer + name_ofs;
    
    if(songno < 3)
        return NULL;
    songno -= 3;
    printf("No = %d ",  songno);

    while(songno > 0)
    {
        while(*++ret != 0);
        while(*++ret == 0);
        songno --;
    }
    printf("Song name =  %s\n", ret);
    return ret;
}

int main(int argc, char *argv[])
{
    uint8_t MIDI_Header[] =
        {'M', 'T', 'h', 'd', 0, 0, 0, 6};

    uint8_t *buffer;
    FILE *fpi;

    if(argc < 2)
    {
        printf("Usage: %s <input file>\n", argv[0]);
        return -1;
    }
    printf("Opening file '%s'\n", argv[1]);
    fpi = fopen(argv[1], "rb+");
    fseek(fpi, 0, SEEK_END);
    size_t filesize = ftell(fpi);
    buffer = malloc(filesize);
    fseek(fpi, 0, SEEK_SET);
    fread(buffer, 1, filesize, fpi);
    printf("Read %d bytes\n", filesize);
    fclose(fpi);

    uint8_t *curptr = buffer;
    size_t remains = filesize;

    size_t curpos = 0;

    while(curptr = memchr(curptr, 'M', remains))
    {   //查找'MThd'
        curpos = curptr - buffer;
        if(memcmp(curptr, MIDI_Header, sizeof(MIDI_Header)) == 0)
        {
            printf("Found 'MThd' in %08x\n", curpos);
            //char *rec_name = &buffer[fat + 88 * songno];
            char *rec_name = find_songname_psre(buffer, songno);
            if (rec_name != NULL)
                dumpMIDI(curptr, curpos, rec_name);
            songno ++;
        }
        curptr ++;

        remains = filesize - curpos;
    }

    free(buffer);
}