nclude <stdio.h>
#include <stdlib.h>
#include <string.h>

/*constants*/
#define NUM_ENTRIES 1000000
#define NUM_THREADS 8
#define LINE_LENTH 1000

/*all entries in file*/
char entries[NUM_ENTRIES][LINE_LENGTH];

void read_file();
void get_substring_num(int id);

void main(){
	read_file();
	
	for(i=0;i<NUM_THREADS; i++){
		get_substring_num(i);
	}
}

void read_file(){
	
	FILE *fp;
	char str1[LINE_LENGTH];
	fp = fopen("/homes/dan/625/wiki_dump.txt", "r");
	
	if(fp == NULL) {
		perror("Failed: ");
		return;
	}
	
	/* Add each line of the file into entries */
	int i = 0;
	while(fgets(str1, LINE_LENGTH, fp) != NULL && i < NUM_ENTRIES){
		strcpy(entries[i], str1);
		i++;
	}

	fclose(fp);
	
}
