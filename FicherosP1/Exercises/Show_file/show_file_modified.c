#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int c,ret;

	if (argc!=3) {
		fprintf(stderr,"Usage: %s <file_name> <block_size>\n", argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	int size;
    sscanf(argv[2], "%d", &size);

    int* ptr = malloc(size);
    
    int nmemb = 1;

    while(fread(ptr, size, nmemb, file) >= nmemb){
        fwrite(ptr, size, nmemb, stdout);
    }

	fclose(file);
	return 0;
}
