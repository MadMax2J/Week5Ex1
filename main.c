#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mem.h>

#define NUMBER_OF_DATAPOINTS 1440
#define INPUT_BUFFER 64
#define RAW_DATA_FILE "../20130922WindData.csv"
#define SORTED_OUTPUT_FILE "../sortedData.dat"

void printArrayContent(float arrayToPrint[]);
void insertionSort(float unsortedArray[]);
void bubbleSort(float unsortedArray[]);
void writeSortedDataToFile(float dataToWriteToFile[], size_t i);
void loadFileData(float *windSpeeds, size_t arraySize);

int main() {
    //Need two arrays of unsorted data, in order to compare the performance of my two sorting algorithms.
    float windSpeedsForBubble[NUMBER_OF_DATAPOINTS];    //An array of floats for my Bubble Sort
    float windSpeedsForInsert[NUMBER_OF_DATAPOINTS];    //An array of floats for my Insert Sort

    ////Requirement #2, load the last column (wind speeds) of the comma delimited file into a one-dimensional array.
    //Populate my arrays with the same data from the Raw Data file. File name specified as RAW_DATA_FILE.
    loadFileData(windSpeedsForBubble, NUMBER_OF_DATAPOINTS);
    loadFileData(windSpeedsForInsert, NUMBER_OF_DATAPOINTS);

    ////Testing - Unsorted data
    //printArrayContent(windSpeedsForBubble);


    ////Requirement #3, Run both instrumented algorithms with the wind speed data.
    bubbleSort(windSpeedsForBubble);    //Sort date using Bubble Sort algorithm
    insertionSort(windSpeedsForInsert); //Sort date using Insert Sort algorithm


    ////Requirement #4, Save the sorted data in a new file to be used in Question 2.
    //Write the Sorted data to file, using either sorted Array. File name specified as SORTED_OUTPUT_FILE.
    writeSortedDataToFile(windSpeedsForInsert, NUMBER_OF_DATAPOINTS);

    ////Testing
    //printArrayContent(windSpeedsForInsert);

    return 0;   //Assuming successful program completion

}//End of main()

void loadFileData(float *windSpeeds, size_t arraySize){

    FILE *inputPtr;     //A pointer to an input file

    //Try open the file...
    if((inputPtr = fopen(RAW_DATA_FILE, "r")) == NULL){ //Opening Failed?
        puts("The file could not be opened!");

    } else {    //Opening Success!
        size_t index = 0;                   //A counter to iterate through the array.
        char buffer[INPUT_BUFFER] = "";     //A buffer to accept a line read in from the input file.

        fscanf( inputPtr, "%s", buffer);    //Read in the first line of the file
        while( !feof( inputPtr ) && index < arraySize){  //Once there is data and we have'nt exceeded our array

            //Strings are in the following format... "9/22/2013,00:01,3.9758"
            strtok (buffer," ,");   //Dump first token, ie. '9/22/2013'
            strtok (NULL, " ,");    //Dump next token, ie. '00:01'
            windSpeeds[index++] = strtof((strtok (NULL, " ,")), NULL);

            ////TESTING
            //printf("Data %d is: %f\n", index - 1, windSpeeds[index-1]);

            fscanf( inputPtr, "%s", buffer);    //Read in the next line in the file

        }   //Back to the while condition

        //If the inputPtr contains the EOF character, the while loop finishes and we add the last piece of data...
        strtok (buffer," ,");   //Dump first token, ie. '9/22/2013'
        strtok (NULL, " ,");    //Dump next token, ie. '00:01'
        windSpeeds[index] = strtof((strtok (NULL, " ,")), NULL);
        ////TESTING
        //printf("Data %d is: %f\n", index, windSpeeds[index]);


        fclose( inputPtr ); //When finished, Close the file.
    }

}


void writeSortedDataToFile(float dataToWriteToFile[], size_t size) {
    FILE *outputFile;

    if((outputFile = fopen(SORTED_OUTPUT_FILE, "w")) == NULL){
        puts("Failed to Open file for Writing!");
    }else{
        for(size_t index = 0; index < size; index++){
            fprintf(outputFile, "%f\n", dataToWriteToFile[index]);
        }
    }

    fclose(outputFile);

}

void insertionSort(float unsortedArray[]) {
    clock_t startTime = clock();
    unsigned int copyCount = 0;
    unsigned int insertionCount = 0;


    size_t in, out;

    for(out = 1; out < NUMBER_OF_DATAPOINTS; out++){

        float temp = unsortedArray[out]; //remove market item //Why not 0??
        in = out;

        while(in > 0 && unsortedArray[in-1] >= temp){
            copyCount++;
            unsortedArray[in] = unsortedArray[in-1];
            --in;
        }
        insertionCount++;
        unsortedArray[in] = temp;


    }
    clock_t finishTime = clock();

    printf("Insertion Sort completed after \t%f seconds, with %7u data copies and %8u data insertions.\n",
           (double)(finishTime - startTime) / CLOCKS_PER_SEC, copyCount, insertionCount);
    //printf("Time taken for Insertion Sort was: %f!\n\n\n", (double)(finishTime - startTime) / CLOCKS_PER_SEC);

}



void bubbleSort(float unsortedArray[]) {

    clock_t startTime = clock();
    unsigned int swapCount = 0;
    unsigned int comparisonCount = 0;


    /* initialize a */
    size_t pass; /* passes counter */
    size_t i; /* comparisons counter */
    float hold; /* temporary location used to swap array elements */

    /* bubble sort */
    /* loop to control number of passes */
    for ( pass = 1; pass < NUMBER_OF_DATAPOINTS; pass++ ) {

        /* loop to control number of comparisons per pass */
        for ( i = 0; i < NUMBER_OF_DATAPOINTS - 1; i++ ) {

            /* compare adjacent elements and swap them if first
            element is greater than second element */
            comparisonCount++;
            if ( unsortedArray[ i ] > unsortedArray[ i + 1 ] ) {
                swapCount++;
                hold = unsortedArray[ i ];
                unsortedArray[ i ] = unsortedArray[ i + 1 ];
                unsortedArray[ i + 1 ] = hold;
            } /* end if */
        } /* end inner for */
    } /* end outer for */

    clock_t finishTime = clock();

    printf("Bubble Sort completed after \t%f seconds, with %7u data swaps and  %8u data comparisons.\n",
           (double)(finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

    //printf("Time taken for Bubble Sort was: %f!\n\n\n", (double)(finishTime - startTime) / CLOCKS_PER_SEC);

}


void printArrayContent(float windSpeeds[]) {

    float *arrayPtr;
    arrayPtr = windSpeeds;
    for (size_t index = 0; index < NUMBER_OF_DATAPOINTS; index++) {
        printf("%d: %f\n", index + 1, *(arrayPtr++));
    }

}