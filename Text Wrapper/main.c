#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/file.h>
#include<sys/types.h>

#define MAXSIZE 1000

int ReadFile(char FileName[]);
void RemoveSpace();
int ProcessData(int width);
void WriteFile();

int size=0;
int processcounter=0;
char FileName[20];
char FileData[MAXSIZE]={0};
char SpaceRemoved[MAXSIZE]={0};
char ProcessedData[MAXSIZE]={0};

int main(int argc, char** argv)
{
    int result;
    int width= strtol(argv[1], NULL, 10);

    if(width<0)
    {
        printf("Invalid Width!!\n");
        return 0;
    }

    if(argc<=2)
    {
        printf("Welcome To Text Wrapper!!\n\n");
        printf("Enter Text: ");
        fgets(FileData,MAXSIZE, stdin);
        RemoveSpace();
        size=strlen(SpaceRemoved);
        result=ProcessData(width);
    }
    else if(argc==3 && strchr( FileName, '.'))
    {
        strcpy(FileName, argv[2]);
        result=ReadFile(FileName);
        if(result==1)
        {
            printf("Unable To Open The File!!\n");
            return 0;
        }
        else
        {
            printf("\nOpening File...\n");
            printf("Reading File...\n\n");
        }
        RemoveSpace();
        size=strlen(SpaceRemoved);
        result=ProcessData(width);
        WriteFile();
        printf("Text Wrapped..\n");
        printf("File Saved..\n");
    }
    else if(argc==3)
    {
        printf("Text Wrapped..\n");
        printf("File Saved..\n");
    }

    return 0;
}

void WriteFile()
{
    FILE *writefile;
    char output[MAXSIZE]="wrap";
    strcat(output,FileName);
    writefile = fopen(output,"w");

    for(int i=0;i<processcounter;i++)
    {
        fprintf(writefile,"%c",ProcessedData[i]);
    }

    fclose(writefile);
}

int ProcessData(int width)
{
    processcounter=0;
    int count=0;

    for(int i=0;i<size-1;i++)
    {
        if(count==width)
        {
            ProcessedData[processcounter]='\n';
            count=0;
            processcounter++;
        }

        if(ProcessedData[processcounter-1]=='\n' && SpaceRemoved[i]==' ')
        {
            continue;
        }
        else if(count!=width)
        {
            ProcessedData[processcounter]=SpaceRemoved[i];
            count++;
            processcounter++;
        }
    }

    return 0;
}

void RemoveSpace()
{
    int i = 0, j;

    while(FileData[i] == ' ')
    {
        i++;
    }

    for(j = 0;FileData[i] != '\0'; i++){
      if(FileData[i]==' ' && FileData[i-1]==' '){
          continue;
      }
      SpaceRemoved[j] = FileData[i];
      j++;
    }
    SpaceRemoved[j] = '\0';
}

int ReadFile(char FileName[])
{
    int fd= open(FileName, O_RDONLY, 0);
    if(fd<0)
    {
        return 1;
    }

    int fid=read(fd, &FileData, MAXSIZE-1);
    size=fid;
    return 0;
}
