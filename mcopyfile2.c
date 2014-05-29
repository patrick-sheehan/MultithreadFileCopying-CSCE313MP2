/*  mcopyfile2.c
	 	Patrick Sheehan
	 	CSCE 313 MP2
	 	26 Feb 2014
	 	Resources: stackoverflow, tutorialspoint.com
*/

#include <stdio.h>
#include <pthread.h>

#define CHARS_PER_THREAD 1000

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void multiThreadCopy(char*, char*);
void sectionCopy(void**);

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Expected input and output files as parameters\n");
		return 0;
	}
	multiThreadCopy(argv[1], argv[2]);
//	printf("\n\n[operation complete]\n");
	return 0;
}
void multiThreadCopy(char* src, char* dest)
{
	FILE * srcFile;
	FILE * destFile;
	size_t pos = 0;	// track position in the file

	srcFile = fopen(src, "r");
	destFile = fopen(dest, "w");
	// get size of file in bytes/chars
	fseek (srcFile, 0, SEEK_END);
	size_t size = ftell(srcFile);
	fseek(srcFile, 0, SEEK_SET); // reset seek
//	printf("size of source file: \t%zu bytes\n", size);
	void* args[2] = {srcFile, destFile};

	for (pos = 0; pos < size; pos += CHARS_PER_THREAD)
	{
		pthread_t thread;
		pthread_mutex_lock(&lock);
		pthread_create(&thread, NULL, (void*)sectionCopy, args);
	}

	pthread_mutex_lock(&lock);
	fclose(srcFile);
	fclose(destFile);
};

void sectionCopy(void* files[])
{	// copy a buffer of bytes from one file to another
	FILE * srcFile 	= files[0];
	FILE * destFile = files[1];
	char currSection[CHARS_PER_THREAD];
	int charsRead = fread(currSection, sizeof(char), CHARS_PER_THREAD, srcFile);
	fwrite(currSection, sizeof(char), charsRead, destFile);
	pthread_mutex_unlock(&lock);
}
