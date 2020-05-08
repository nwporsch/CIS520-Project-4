#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>

#define LINE_LENGTH 1000

/*constants*/
int NUM_ENTRIES = 1000;
int NUM_THREADS = 1;


/*all entries in file*/
char **entries;
int *max_substring;

int read_file();
void get_substring_num(int id);
void print_results();

int main(int argc, char* argv[]){

	//Starting the program so we need to see the current time
	struct timeval start, readInFile, finish;
	double timeInterval;
	int i = 0;

	
	//See if arguments were passed
	if(argc == 3){
	
		NUM_ENTRIES = strtol(argv[1], NULL, 10);	
		NUM_THREADS = strtol(argv[2], NULL, 10);
		max_substring = (int*)malloc(NUM_ENTRIES * sizeof(int));
		entries = malloc((NUM_ENTRIES)* sizeof(char*));
		while(i < NUM_ENTRIES){
			entries[i] = malloc(LINE_LENGTH * sizeof(char));
			i++;
		}
	}
	else{
		perror("Invalid amount of arguments");
		return -1;
	}
	gettimeofday(&start, NULL);

	if(read_file() != -1){
		gettimeofday(&readInFile, NULL);		
	
		omp_set_num_threads(NUM_THREADS);

		#pragma omp parallel
		{
			get_substring_num(omp_get_thread_num());
		}

		print_results();
		gettimeofday(&finish, NULL);

		//Print out the timings of for the program
		timeInterval = ((readInFile.tv_sec - start.tv_sec) * 1000.0) + ((readInFile.tv_usec - start.tv_usec) / 1000.0);
		printf("\nTiming completed for program using OpenMP with %d threads and %d CPUs\n", NUM_THREADS, getenv("SLURM_CPUS_ON_NODE"));
		printf("Reading in File: %lf nanoseconds\n", timeInterval); 
		timeInterval = ((finish.tv_sec - readInFile.tv_sec) * 1000.0) + ((finish.tv_usec - readInFile.tv_usec) / 1000.0);
		printf("Comparisons of wiki pages: %lf nanoseconds\n", timeInterval);
		timeInterval = ((finish.tv_sec - start.tv_sec) * 1000.0) + ((finish.tv_usec - start.tv_usec) / 1000.0);
		printf("Overall time: %lf nanoseconds\n", timeInterval); 
	}

	free(max_substring);
	i = 0;
	while(i < NUM_ENTRIES){
		free(entries[i]);
		i++;
	}
	free(entries);
	return 0;
}


int read_file(){
	
	FILE *fp;
	char str1[LINE_LENGTH] = "";
	fp = fopen("/homes/dan/625/wiki_dump.txt", "r");
	//fp = fopen("/homes/nwporsch/CIS520-Project-4/smallwiki.txt","r");
	if(fp == NULL) {
		perror("Failed: ");
		return -1;
	}

	/* Add each line of the file into entries */
	int lineNumber = 0;
	char ch = ' ';
	int currentLengthOfString = 0;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	read = getline(&line,&len,fp);
	while(lineNumber < NUM_ENTRIES && read != -1){
		
		strncpy(entries[lineNumber], line, LINE_LENGTH-2);
		
		lineNumber++;
		
		read = getline(&line,&len,fp);
		
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


	#pragma omp private(startPos,endPos,str1,str2,str1_total,str2_total,i,j,final_total)
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
