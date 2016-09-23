/*NAME: AADIL AHMED ADAM

  CLASS: UAH CS590 SUMMER 2016
  DATE: 26 JUNE 2016
  DESCRIPTION: C program to implement the copy command in unix by copying mutiple files into  agiven directory
******************************************************************************************************************
*/

//header files
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#define BUFFERSIZE 1024

//main function
int main(int argc, char *argv[])
{
int fd1,fd2,fd3;
int i ,count=argc-1;
struct stat check;
if(argc>3)
{
	printf("Your arguments value is %d \n",argc);
	char *dest=argv[argc-1];
	lstat (argv[argc-1], &check);   // returns the file attributes
	dest=malloc(strlen(argv[argc-1])+1);      // allocates memory to store the destination directory name string
	strcat(dest,argv[count]);
	if(!(S_ISDIR(check.st_mode))){           // checks the directory exists or not
		printf("Directory doesnot exist\n \n");
	}else{
		for(i=count-1;i>=1;i--){      // iterate for number of input files given
		 printf("%s is copied\n",argv[i]);
		if((fd1 = open(argv[i], O_RDONLY))<0){       //returns a file descriptor for input files
			printf("\n Input file %d doesnot exist \n ",i);
			continue;
		}else{
	        	access(dest,W_OK);     // check access for the directory
		  	if(EACCES == errno){
				printf("Access denied\n");                         
				exit(0);
			}
			free(dest);    // free the allocated destination filename memory on every iteration
			dest=malloc(strlen(argv[i])+strlen(argv[argc-1])+2);
			strcat(dest,argv[count]);     // append the directory name with the input file name
			strcat(dest, "/");           // append the "/" after the directory name on every iteration.
			strcat(dest, argv[i]);        // append the file name to the dest array after the "/"
			if((fd2=open(dest,O_WRONLY))>0)
			{
				char y; 
				printf("Destination file is not empty, Do you wana override it: ");
				scanf("%s",&y);	
				if(y=='y')   // copy the contents of the file 
				{
					char buf[8192];
					size_t content=read(fd1, &buf[0], sizeof(buf));
					write(fd2, &buf[0],content)==content;
				}else{
					printf("User doesnot want to override the file\n");
					return 0;
				}			
			}else{							
				fd2=open(dest,O_CREAT|O_WRONLY,0777);
				char buf[8192];
				size_t content = read(fd1, &buf[0], sizeof(buf));
				write(fd2, &buf[0],content)==content;	
			}
		}
		}
	printf("All the existing input files have been successfully copied into the given directory!\n");
	}
}else{
printf("Insufficient number of arguments\n");
return 0;
}
}
// end of the program
