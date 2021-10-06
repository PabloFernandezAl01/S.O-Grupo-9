#include <stdio.h>
#include <stdlib.h>
#include <err.h>

char* loadstr(FILE * file);

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	int c,ret;

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	// /* Read file byte by byte */
	// while ((c = getc(file)) != EOF) {
	// 	/* Print byte to stdout */
	// 	ret=putc((unsigned char) c, stdout);

	// 	if (ret==EOF){
	// 		fclose(file);
	// 		err(3,"putc() failed!!");
	// 	}
	// }

	// fclose(file);
	loadstr(file);

	return 0;
}

char*
loadstr(FILE * file)
{
	// Complete the function
	
	int cont = 0;
	if (file != NULL){
		while(getc(file) != EOF) cont++;

		fseek(file, 0, SEEK_END);
		cont = ftell(file);
	}
	char* ptr = malloc(cont);

	rewind(file);

	if(fread(ptr, cont, 1, file) >= 1) {
		fwrite(ptr, cont, 1, stdout);
		return ptr;
	}

	return NULL;
}
