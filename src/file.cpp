#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "file.h"

char *file_read(char *filename) {
	FILE *fp = fopen(filename, "r");

	if(!fp) {
		printf("ERROR: Could not read '%s'\n", filename);
		exit(1);
	}
	char buffer[512] = {0};
	int bytes_read, total_bytes = 0;
	char *file_content = NULL;

	while(!feof(fp) && !ferror(fp)) {
		bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fp);
		file_content = (char *) realloc(file_content, total_bytes + bytes_read + 1);
		memcpy(file_content + total_bytes, buffer, bytes_read);
		total_bytes += bytes_read;
	}
	if(ferror(fp)) {
		printf("Error reading file!\n");
		exit(1);
	}

	file_content[total_bytes] = '\0';
	printf("'%s' size: %d bytes\n", filename, total_bytes);
	fclose(fp);

	return file_content;
}
