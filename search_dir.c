
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 1000

int FOUND = 0;
int depth = 0;
char directory[1000];

void find(char* path, int depth, char* filename)
{
	DIR *dir=opendir(path);
	struct dirent *current;
	struct stat buf;

	while((current = readdir(dir)) != NULL)
	{
		stat(current-> d_name, &buf);

		if(S_ISREG(buf.st_mode))
		{
			if(strcmp(filename, current-> d_name) == 0)
			{	
				FOUND = 1;
				printf("FILE IS FOUND, PATH: %s/%s\n", directory, filename);
			
			}
		}
	}
	rewinddir(dir);	
	while((current = readdir(dir)) != NULL)
		{
			char addres[LENGTH];
			strcpy(addres, path);
			strcat(addres, "/");
			strcat(addres, current -> d_name);
			stat(addres, &buf);
			strcpy(directory, addres);

		    if(strcmp(current -> d_name, "..") == 0)
            {
                continue;
            }
            if(strcmp(current -> d_name, ".") == 0)
            {
                continue;
            }

			if (S_ISDIR(buf.st_mode)) 
			{
				if (depth > 0)
				{
					find(addres, depth - 1, filename);
				}
			}
		}

	(void)closedir(dir);
}
	
main(int argc, char *argv[])
{
	find(argv[1], atoi(argv[2]), argv[3]);
	if (!FOUND) printf("FILE WASN'T FOUND\n");
	return 0;
}