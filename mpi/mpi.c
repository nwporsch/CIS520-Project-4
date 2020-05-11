#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>
#include <mpi.h>

/*constants*/
#define NUM_ENTRIES 100
#define LINE_LENGTH 1000

int NUM_THREADS;

/*all entries in file*/
char entries[NUM_ENTRIES][LINE_LENGTH];
int max_substring[NUM_ENTRIES];
int local_max_substring[NUM_ENTRIES];

int read_file();
void get_substring_num(int id);
void print_results();

int main(int argc, char* argv[]){
	//Starting the program so we need to see the current time
	struct timeval start, readInFile, finish;
	double timeInterval;
	
	MPI_Status Status;
	int rc;
	int numtasks, rank;	

	rc = MPI_Init(&argc, &argv);	
	
	if(rc != MPI_SUCCESS)
	{
		printf ("Error starting MPI program. Terminating.\n");
        	MPI_Abort(MPI_COMM_WORLD, rc);
	}

	gettimeofday(&start, NULL);
	
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	NUM_THREADS = numtasks;
	printf("size = %d rank = %d\n", numtasks, rank);
	fflush(stdout);
	if(rank ==0){
		if(read_file() != -1){
			gettimeofday(&readInFile, NULL);		
			int i;
			MPI_Bcast(max_substring, NUM_ENTRIES*LINE_LENGTH, MPI_INT, 0, MPI_COMM_WORLD);
			for(i = 0; i < NUM_THREADS; i++){
	
				get_substring_num(i);
			}

			MPI_Reduce(local_max_substring, max_substring, NUM_ENTRIES*LINE_LENGTH, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
			print_results();
			gettimeofday(&finish, NULL);

			//Print out the timings of for the program
			timeInterval = ((readInFile.tv_sec - start.tv_sec) * 1000.0) + ((readInFile.tv_usec - start.tv_usec) / 1000.0);
			printf("\nTiming completed for program using base configuration with %d threads\n", NUM_THREADS);
			printf("Reading in File: %lf nanoseconds\n", timeInterval); 
			timeInterval = ((finish.tv_sec - readInFile.tv_sec) * 1000.0) + ((finish.tv_usec - readInFile.tv_usec) / 1000.0);
			printf("Comparisons of wiki pages: %lf nanoseconds\n", timeInterval);
			timeInterval = ((finish.tv_sec - start.tv_sec) * 1000.0) + ((finish.tv_usec - start.tv_usec) / 1000.0);
			printf("Overall time: %lf nanoseconds\n", timeInterval); 
		}
	}	

	MPI_Finalize();
	return 0;
}


int read_file(){
	
	FILE *fp;
	char str1[LINE_LENGTH] = "";
	//fp = fopen("/homes/dan/625/wiki_dump.txt", "r");
	fp = fopen("/homes/nwporsch/CIS520-Project-4/smallwiki.txt","r");
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
	
	while((read = getline(&line, &len, fp)) != -1 || lineNumber < NUM_ENTRIES){
		strncpy(entries[lineNumber], line, LINE_LENGTH-1);
		entries[lineNumber][LINE_LENGTH] = 0;
		lineNumber++;

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
