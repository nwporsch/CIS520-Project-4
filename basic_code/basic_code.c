#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*constants*/
#define NUM_ENTRIES 100
#define NUM_THREADS 4
#define LINE_LENGTH 1000

/*all entries in file*/
char entries[NUM_ENTRIES][LINE_LENGTH];
int max_substring[NUM_ENTRIES];

int read_file();
void get_substring_num(int id);
void print_results();

int main(){
	if(read_file() != -1){
		int i;
		for(i=0;i<NUM_THREADS; i++){
			get_substring_num(i);
		}
		print_results();
	}
	return 0;
}


int read_file(){
	
	FILE *fp;
	char str1[LINE_LENGTH];
	fp = fopen("/homes/nwporsch/CIS520-Project-4/smallwiki.txt", "r");
	
	if(fp == NULL) {
		perror("Failed: ");
		return -1;
	}
	
	/* Add each line of the file into entries */
	int i = 0;
	while(fgets(str1, LINE_LENGTH, fp) != NULL && i < NUM_ENTRIES){
		strcpy(entries[i], str1);
		i++;
	}

	fclose(fp);
	return 0;
}
void get_substring_num(int id){
	int startPos = id * (NUM_ENTRIES / NUM_THREADS);
	int endPos = startPos + (NUM_ENTRIES / NUM_THREADS);
	
	char str1[LINE_LENGTH];
	char str2[LINE_LENGTH];
	
	int str1_total = 0;
	int str2_total = 0;
	
	int i, j;
	int final_total;

	for( i = startPos; i < endPos -1; i++){
		strcpy(str1, entries[i]);
		strcpy(str2, entries[i+1]);		
		for(j = 0; j < LINE_LENGTH; j++){
			str1_total = str1_total +  (int)str1[j];
			str2_total = str2_total + (int)str2[j];
		}
		
		final_total = str1_total-str2_total;
		
		max_substring[i] = final_total;
	}
}

void print_results(){
	int i = 0;
	int total = 0;
	for ( i = 0; i < NUM_ENTRIES; i++ ) {
		total += max_substring[i];
		printf("%d - %d: %d\n", i, i+1, max_substring[i]);
	}
	printf("\nTotal characters:  %d\n", total);
	
	
}
