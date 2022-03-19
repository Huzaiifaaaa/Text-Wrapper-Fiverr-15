#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/file.h>
#include<sys/types.h>
#include <malloc.h>
#include<unistd.h>
#include <dirent.h>
#include<process.h>
#include<wchar.h>
#include <direct.h>
//including respective header files

//defining macros
#define MAXSIZE 1000

//function headings
int ReadFile(char FileName[]);
void RemoveSpace();
int ProcessData(int width);
void WriteFile();

//variable declarations
int size=0;
int processcounter=0;
char FileName[20];
char FileData[MAXSIZE]={0};
char Directory[MAXSIZE]={0};
char SpaceRemoved[MAXSIZE]={0};
char ProcessedData[MAXSIZE]={0};

//main function, execution starts here, gets command line arguments
int main(int argc, char** argv)
{
    int result;
    int width= strtol(argv[1], NULL, 10);//getting first argument, which is width

    if(width<0)//if width is negative, exiting the program
    {
        printf("Invalid Width!\n");
        return 0;
    }

    if(argc==3 && strchr(argv[2], '.')!=NULL)//if arguments passed are & we are reading a file
    {
        strcpy(FileName, argv[2]);//getting second argument, which can be a file or a directory
        result=ReadFile(FileName);//Reading the file
        if(result==1)//if file does not exists
        {
            printf("Unable To Open %s\n",FileName);//printing message
            return 0;//returning
        }
        else//if file exists
        {
            printf("\nOpening File %s!\n", FileName);//printing message
            printf("Reading File %s!\n",FileName);//printing message
        }
        RemoveSpace();//calling function to remove extra spaces
        size=strlen(SpaceRemoved);//getting size after removing spaces
        result=ProcessData(width);//processing string
        WriteFile();//writing filr
    }
    else if(argc==3 && strchr(argv[2], '.')==NULL)//if arguments are 3 & directory path is passed
    {
        int status=0;

        strcpy(Directory, argv[2]);//getting directory path
        status=chdir(Directory);//changing directory
        if(status<0)//if invalid directory path is entered
        {
            printf("\nUnable To Switch Directory!\n\n");//printing message
            return 0;//exiting
        }
        else//if path is valid
        {
            DIR *directory;//pointer to directory
            struct dirent *direct;//pointer to directory
            directory = opendir(".");//opening directory

            while ((direct = readdir(directory)) != NULL)//looping till every file is iterated
            {
                if( !strcmp(direct->d_name, ".") || !strcmp(direct->d_name, ".."))
                {

                }
                else
                {
                    for(int i=0;i<MAXSIZE;i++)//reinitializing the variables
                    {
                        FileData[i]=0;
                        SpaceRemoved[i]=0;
                        ProcessedData[i]=0;
                    }

                    strcpy(FileName, direct->d_name);//copying file name
                    result=ReadFile(FileName);//reading the file
                    if(result==1)
                    {
                        printf("Unable To Open %s!\n",FileName);//if file id not readable
                        continue;//continue
                    }
                    else//if file is readable
                    {
                        printf("\nOpening File %s!\n",FileName);//printing message
                        printf("Reading File %s!\n",FileName);//printing message
                        RemoveSpace();//removing extra spaces from filr
                        size=strlen(SpaceRemoved);//getting length after removing spaces
                        result=ProcessData(width);//processing the file
                        WriteFile();//writing to filr
                        sleep(2);//adding a bit delay
                        printf("\n\n");//new line
                    }
                 }//end of else
            }//end of while
            closedir(directory);//closing directory
        }//end of else
    }
    else
    {
        printf("Welcome To Text Wrapper!\n\n");//printing message
        printf("Enter Text: ");//printing message
        fgets(FileData,MAXSIZE, stdin);//getting string
        RemoveSpace();//calling function to remove spacing
        size=strlen(SpaceRemoved);//getting string size
        result=ProcessData(width);//processing data

         printf("Normalized Text: \n");
        for(int i=0;i<processcounter;i++)
        {
            printf("%c",ProcessedData[i]);
        }
        printf("\n");
    }

    return 0;//returning
}//end of main function


//Function WriteFile()
//Responsible for writing processed text to a file
//Takes nothing as argument
//Returns nothing
void WriteFile()
{
    char output[MAXSIZE]="wrap";//prefix file name
    strcat(output,FileName);//concateinating the file names

    FILE *writefile;//Filepointer
    writefile = fopen(output,"w");//opening file

    for(int i=0;i<processcounter-1;i++)//writing to file character by character
    {
        fprintf(writefile,"%c",ProcessedData[i]);
    }

    printf("Text of File %s Wrapped!\n", FileName);//printing message
    printf("File Saved As %s!\n",output);//printing message

    fclose(writefile);//closing file
}//end of function


//Function ProcessData()
//Responsible for normalization of text
//Takes width as argument
//Returns integer
int ProcessData(int width)
{
    processcounter=0;//initializing variables
    int count=0;

    for(int i=0;i<size-1;i++)//looping size-1 times to access the data character by character
    {
        if(count==width)//if count=width, adding a new line caharacter
        {
            ProcessedData[processcounter]='\n';//adding new line character
            count=0;//setting count to zero
            processcounter++;//incrementing
        }

        if(SpaceRemoved[i]=='\n')//if character is newline
        {
            ProcessedData[processcounter]=' ';//replacing it with space
            processcounter++;//incrementing
            continue;
        }
        else if(ProcessedData[processcounter-1]=='\n' && SpaceRemoved[i]==' ')//if previous character is ne line character & next character is space,skipping
        {
            continue;
        }
        else if(count!=width)//if count is not equal to width, simply assigning
        {
            ProcessedData[processcounter]=SpaceRemoved[i];//assigning
            count++;//incrementing
            processcounter++;//incrementing
        }
    }//end of for

    return 0;//returning zero
}//end of function


//Function RemoveSpace()
//Responsible for removing extra spaces from text
//Takes nothing as argument
//Returns nothing
void RemoveSpace()
{
    int j;
    int i = 0;//declaring & initializing varibales

    while(FileData[i] == ' ')//looping till the character is a space
    {
        i++;
    }

    for(j = 0;FileData[i] != '\0'; i++)//looping till null character reached
    {
      if(FileData[i]==' ' && FileData[i-1]==' ')
      {
          continue;//continue
      }
      SpaceRemoved[j] = FileData[i];//assigning
      j++;//incrementing
    }

    SpaceRemoved[j] = '\0';//adding end of line character
}//end of function


//Function ReadFile()
//Responsible for reading a file to array
//Takes filenam as argument
//Returns 0 on successful execution
int ReadFile(char FileName[])
{
    int fd= open(FileName, O_RDONLY, 0);//opening file
    if(fd<0)//if file does not exist, return 1
    {
        return 1;
    }

    int fid=read(fd, &FileData, MAXSIZE-1);//reading file to array
    size=fid;//assigning to size
    return 0;//returning zero
}//end of function
