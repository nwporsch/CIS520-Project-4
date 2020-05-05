#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/*constants*/
#define NUM_ENTRIES 100
#define NUM_THREADS 1
#define LINE_LENGTH 1000


/*all entries in file*/
char entries[NUM_ENTRIES][LINE_LENGTH];
int max_substring[NUM_ENTRIES];

int read_file();
void get_substring_num(int id);
void print_results();

int main(){
	//Starting the program so we need to see the current time
	struct timeval start, readInFile, finish;
	double timeInterval;

	gettimeofday(&start, NULL);

	if(read_file() != -1){
		gettimeofday(&readInFile, NULL);		
		int i;

		for(i = 0; i < NUM_THREADS; i++){
	
			get_substring_num(i);
		}
		
		print_results();
		gettimeofday(&finish, NULL);

		//Print out the timings of for the program
		timeInterval = ((readInFile.tv_sec - start.tv_sec) * 1000.0) + ((readInFile.tv_usec - start.tv_usec) / 1000.0);
		printf("\nTiming completed for program using OpenMP with %d threads\n", NUM_THREADS);
		printf("Reading in File: %lf nanoseconds\n", timeInterval); 
		timeInterval = ((finish.tv_sec - readInFile.tv_sec) * 1000.0) + ((finish.tv_usec - readInFile.tv_usec) / 1000.0);
		printf("Comparisons of wiki pages: %lf nanoseconds\n", timeInterval);
		timeInterval = ((finish.tv_sec - start.tv_sec) * 1000.0) + ((finish.tv_usec - start.tv_usec) / 1000.0);
		printf("Overall time: %lf nanoseconds\n", timeInterval); 
	}

	return 0;
}


int read_file(){
	
	FILE *fp;
	char str1[LINE_LENGTH] = "";
	fp = fopen("/homes/dan/625/wiki_dump.txt", "r");
	
	if(fp == NULL) {
		perror("Failed: ");
		return -1;
	}
	
	/* Add each line of the file into entries */
	int lineNumber = 0;
	char ch = ' ';
	char previousch = ' ';
	int currentLengthOfString = 0;
	
	while(ch != EOF || lineNumber <= NUM_ENTRIES){
		ch = fgetc(fp);
		if(ch == 'n' && previousch == '\\'){
			ch = '\n';
		}		


		if(currentLengthOfString >= LINE_LENGTH || ch == '\n'){
	
			strcpy(entries[lineNumber],str1);
			strcpy(str1, "");
			currentLengthOfString = 0;
			lineNumber++;

			if(lineNumber == 101){
				break;
			}

			if(ch != '\n'){
				while(ch != '\n' || ch != EOF){
					previousch = ch;
					ch = fgetc(fp);
					if(previousch == '\\' && ch == 'n'){
						ch = '\n';
					}
					
				}
			}
		}
		else{
			if(ch != '\\'){
				strncat(str1, &ch,1);
				currentLengthOfString++;
			}
			previousch = ch;
		}
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


	
	for( i = startPos; i < endPos - 1; i++){
		strcpy(str1, entries[i]);
		strcpy(str2, entries[i+1]);		
		for(j = 0; j < LINE_LENGTH; j++){
			if(j < strlen(str1)){
				str1_total = str1_total +  (int)str1[j];
			}
			if(j< strlen(str2)){
				str2_total = str2_total + (int)str2[j];
			}
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
	
	
}
