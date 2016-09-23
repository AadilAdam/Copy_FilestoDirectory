/*NAME: AADIL AHMED ADAM
  CLASS: UAH CS530 SUMMER 2016 SECTION 1                                               DATE: 26 JUNE 2016
  DESCRIPTION: C program to demonstrate the transfer of contents of a file to another file/directory using unix I/O routines.  
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

int copyContents(int fd1,int fd2);     // function to copy conetnts os a particular file to another file

//function description
int copyContents(int fd1,int fd2){
char buf[8192];
size_t content = read(fd1, &buf[0], sizeof(buf));
write(fd2, &buf[0], content) == content;
return 0;
};

//main function
int main(int argc, char *argv[])
{
	int fd1,fd2,fd3;         // variables to store function descriptors
	if(argc==3)              //loop if arguments =3
	{
	struct stat check;
	int i;
	//if(argv[1]!=argv[2]){
	int result = access (argv[2], F_OK);	// F_OK tests existence also (R_OK,W_OK,X_OK)for readable, writeable, executable
	if(result<0)
	{	
		printf("Destination file doesnot exist.\n create the file to continue. \n");
	}
	lstat(argv[2], &check);	    //returns the file attributes of the destiantion file
	printf("%s is given destination!!\n",argv[2]);   		printf("%s is given source!!\n",argv[1]); 
	if(( fd1 = open(argv[1], O_RDONLY,0777))<0){           //initializes access to the file and returns a file descriptor 
		perror("\n \n Input file doesnot exist. \n ");
	}
	else{
		printf("Copying file contents to  another file\n");
		if((fd2 = open(argv[2],O_WRONLY))>0){         //initailaizes access to the destination file and returns a file descriptor
			char y; 
			printf("Destination file is not empty.\n Do you want to override it?:  ");
			scanf("%s",&y);	
			if(y=='y')
			{
				copyContents(fd1,fd2);
			}else{
				printf("User doesnot want to override the file\n");
				return 0;
			}
		}else{
			if (S_ISDIR(check.st_mode))     // system call to check if the file is a directory or not
			{		
				access(argv[2],W_OK);    // check access permission to the directory
				if(EACCES == errno){
					printf("Access permission denied");
					exit(0);
				}
				strcat(argv[2], "/");    // append the input file under the destination directory
				strcat(argv[2],argv[1]);
				if((fd2=open(argv[2],O_WRONLY,0777))>0)
				{
					char y; 
					printf("Destination file is not empty! Do you want to override it: ");
					scanf("%s",&y);	
					if(y=='y')
					{
						copyContents(fd1,fd2);
					}else{
						printf("User doesnot want to override the file\n");
		                                return 0;
					}
							
				}else{	  //if the destination file doesnot exist then create the file and transfer the contents.
					fd3 =open(argv[2],O_CREAT|O_WRONLY,0777);
					copyContents(fd1,fd3);
				}
			}else{       // if the directory doesnot exist then create the directory and transfer the contents.
				char *dest = argv[2];
				mkdir(dest,0777);
				fd2=open(dest,O_CREAT|O_WRONLY,0777);
				copyContents(fd1,fd2);
			}
		}
	}
	}else{
		printf("Invalid number of arguments\n");
		return 0;
	}
	fcloseall();
}

// end of program.
