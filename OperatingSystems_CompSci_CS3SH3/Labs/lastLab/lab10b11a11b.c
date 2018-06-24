/*Author: Dr. Neerja Mhaskar
* Course: CS 3SH3 - Operating Systems
* Solution to practice labs 10b 11a 11b
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

int main ( int argc, char *argv[])
{
    struct stat fileAttributes;

    if (argc != 2) {
		fprintf(stderr,"Usage: ./a.out <input file>\n");
		return -1;
    }

    if (stat(argv[(argc - 1)], &fileAttributes) < 0)
        printf("File Error Message = %s\n", strerror(errno));
    else {
	printf("\n\n");
	printf("File attributes for the File: '%s'\n", argv[1]);
	printf("-------------------------------------------------\n");
	printf("Inode: %d\n", fileAttributes.st_ino);
	printf("Size (in bytes): %d\n",fileAttributes.st_size);
	printf("Blocks: %d\n", fileAttributes.st_blocks);
	printf("File Permissions: ");
	printf( (S_ISDIR(fileAttributes.st_mode)) ? "d" : "-");
	printf( (fileAttributes.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileAttributes.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileAttributes.st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileAttributes.st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileAttributes.st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileAttributes.st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileAttributes.st_mode & S_IROTH) ? "r" : "-");
	printf( (fileAttributes.st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileAttributes.st_mode & S_IXOTH) ? "x" : "-");
	printf("\n");
	printf("Uid: %d\n",fileAttributes.st_uid);
	printf("Time of last access: %s", ctime(&fileAttributes.st_atime));
    	printf("Time of last data modification: %s", ctime(&fileAttributes.st_mtime));
    	printf("Last Status Change time: %s", ctime(&fileAttributes.st_ctime));		
   }

    return 0;
}
