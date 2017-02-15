
//Access the C Standard Library
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//Don't know why, but the header name is different on a linux system??
#ifdef __linux__
    #include <memory.h>
#else
    #include <mem.h>
#endif

//Preprocessor constant definitions
#define NUMBER_OF_DATAPOINTS 1440
#define INPUT_BUFFER 64
#define RAW_DATA_FILE "../20130922WindData.csv"
#define SORTED_OUTPUT_FILE "../sortedData.dat"

//Function Prototypes
void loadFileData(float arrayToStoreData[], size_t arraySize);
void bubbleSort(float unsortedArray[], size_t arraySize);
void insertionSort(float unsortedArray[], size_t arraySize);
void writeSortedDataToFile(float dataToWriteToFile[], size_t arraySize);
void printArrayContent(float arrayToPrint[], size_t arraySize);


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
    bubbleSort(windSpeedsForBubble, NUMBER_OF_DATAPOINTS);    //Sort date using Bubble Sort algorithm
    insertionSort(windSpeedsForInsert, NUMBER_OF_DATAPOINTS); //Sort date using Insert Sort algorithm


    ////Requirement #4, Save the sorted data in a new file to be used in Question 2.
    //Write the Sorted data to file, using either sorted Array. File name specified as SORTED_OUTPUT_FILE.
    writeSortedDataToFile(windSpeedsForInsert, NUMBER_OF_DATAPOINTS);

    ////Testing
    //printArrayContent(windSpeedsForInsert);

    return 0;   //Assuming successful program completion

}//End of main()

/**
 * Function loadFileData
 * This function is tasked with reading in the content of a datafile containing wind speeds in a comma separated format.
 * The data is in the format... 9/22/2013,00:01,3.9758
 *
 * @param windSpeeds - A memory reference to the data array to be populated.
 * @param arraySize - The size of the array to control bounds
 */
void loadFileData(float arrayToStoreData[], size_t arraySize) {

    FILE *inputPtr;     //A pointer to an input file

    //Try open the file...
    if ((inputPtr = fopen(RAW_DATA_FILE, "r")) == NULL) { //Opening Failed?
        puts("FATAL ERROR! - The file could not be opened!");
        exit(1);    //Quit the program

    } else {    //Opening Success!
        size_t index = 0;                   //A counter to iterate through the array.
        char buffer[INPUT_BUFFER] = "";     //A buffer to accept a line read in from the input file.

        fscanf(inputPtr, "%s", buffer);     //Read in the first line of the file
        while (!feof(inputPtr) && index < arraySize) {  //Once there is data and we have'nt exceeded our array bounds...

            //Strings are in the following format... "9/22/2013,00:01,3.9758"
            strtok(buffer, " ,");   //Dump first token, ie. '9/22/2013'
            strtok(NULL, " ,");    //Dump next token, ie. '00:01'
            arrayToStoreData[index++] = strtof((strtok(NULL, " ,")), NULL);   //Store the 3rd token, as a float, into the array.

            ////TESTING
            //printf("Data %d is: %f\n", index - 1, windSpeeds[index-1]);

            fscanf(inputPtr, "%s", buffer);    //Read in the next line in the file

        }   //Back to the while condition

        //If the inputPtr contains the EOF character, the while loop finishes and we add the last piece of data...
        strtok(buffer, " ,");   //Dump first token, ie. '9/22/2013'
        strtok(NULL, " ,");    //Dump next token, ie. '00:01'
        arrayToStoreData[index] = strtof((strtok(NULL, " ,")), NULL);     //Store the 3rd token, as a float, into the array.

        ////TESTING
        //printf("Data %d is: %f\n", index, windSpeeds[index]);

        fclose(inputPtr); //When finished, Close the file.
    }//End if/else

}//End function loadFileData

/**
 * Function bubbleSort
 * This function takes in a reference to an Unsorted array and sorts its data in ascending order.
 * Largely based on the example from (Deitel, 2013) Fig 6.15.
 *
 * @param unsortedArray - The unsorted array to sort
 * @param arraySize - The Size of the Array to process
 */
void bubbleSort(float unsortedArray[], size_t arraySize) {

    clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    unsigned int swapCount = 0;     //Counter to track the number of data item swaps required to complete this task.
    unsigned int comparisonCount = 0;   //Counter to track the number of data item comparisons required to complete this task.


    /* initialize a */
    size_t pass; /* passes counter */
    size_t i; /* comparisons counter */
    float hold; /* temporary location used to swap array elements */

    /* bubble sort */
    /* loop to control number of passes */
    for (pass = 1; pass < arraySize; pass++) {

        /* loop to control number of comparisons per pass */
        for (i = 0; i < arraySize - 1; i++) {
            comparisonCount++;  //For every comparison, increment the comparisonCount counter

            /* compare adjacent elements and swap them if first
            element is greater than second element */
            if (unsortedArray[i] > unsortedArray[i + 1]) {
                swapCount++;    //For every swap, increment the swapCount counter
                hold = unsortedArray[i];    //Place a data item into temp storage
                unsortedArray[i] = unsortedArray[i + 1];    //Put the item from position 'i+1', into position 'i'
                unsortedArray[i + 1] = hold;    //Put the item we have in temp storage, originally
                                                //from position 'i', back into position 'i+1'.
                                                //Swap complete :)
            } /* end if */
        } /* end inner for */
    } /* end outer for */

    clock_t finishTime = clock();   //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    printf("Bubble Sort completed after \t%f seconds, with %7u data swaps and  %8u data comparisons.\n",
           (double) (finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

}//End of function bubbleSort

/**
 * Function insertSort
 * This function takes in a reference to an Unsorted array and sorts its data in ascending order.
 *
 * @param unsortedArray - The unsorted array to sort
 * @param arraySize - The Size of the Array to process
 */
void insertionSort(float unsortedArray[], size_t arraySize) {
    clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    unsigned int copyCount = 0;     //Counter to track the number of data item copies required to complete this task.
    unsigned int insertionCount = 0;//Counter to track the number of data item insertions required to complete this task.

    size_t in, out;                 //Array iterators for outer and inner loops

    //Leaving the first item in place, Mark the next item and work my way through the list...
    for (out = 1; out < arraySize; out++) {
        copyCount++;                        //For every copy, increment the copyCount counter
        float temp = unsortedArray[out];    //Make a copy of the marked item; allowing a space to shift items into.
        in = out;                           //Set the target for my inner loop

        //Check the location is not ZERO and the data item before the current
        //position has a higher value than the value I'm trying to place. (ie. UnOrdered)
        //I want to determine, into what position do I need to insert my temp value...
        while (in > 0 && unsortedArray[in - 1] >= temp) {   //If TRUE, then I need to find the correct place for my value.
            copyCount++;                    //For every copy, increment the copyCount counter
            unsortedArray[in] = unsortedArray[in - 1];  //Shift the previous data value, to my current position.
            --in;                           //Move my iterator to target the previous location
        }   //Back to the While condition to check my new position's value.
        insertionCount++;                   //For every insertion, increment the insertionCount counter.
        unsortedArray[in] = temp;           //Once the above loop has found the correct position, Insert my value.

    } //Move on the the next start position in the Array.

    clock_t finishTime = clock();           //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    printf("Insertion Sort completed after \t%f seconds, with %7u data copies and %8u data insertions.\n",
           (double) (finishTime - startTime) / CLOCKS_PER_SEC, copyCount, insertionCount);

}//End function insertionSort


/**
 * Function writeSortedDataToFile
 * This function takes in a reference to an Array and it's size and writes this data to a file.
 * The filename is controlled by the pre-processor constant SORTED_OUTPUT_FILE.
 *
 * @param dataToWriteToFile - A reference to the source data Array
 * @param arraySize - The Size of the Array to process
 */
void writeSortedDataToFile(float dataToWriteToFile[], size_t arraySize) {
    FILE *outputFile;//A pointer to an Output file

    //Try open the file...
    if ((outputFile = fopen(SORTED_OUTPUT_FILE, "w")) == NULL) {//Opening Failed?
        puts("Failed to Open file for Writing!");
        //Perhaps add some better Error Handling in the future?
    } else {//Opening Success!
        for (size_t index = 0; index < arraySize; index++) {         //Iterate through the Array
            //For each item, send the data and a newline character to the Output Pointer...
            fprintf(outputFile, "%f\n", dataToWriteToFile[index]);
        }//Next iteration
    }//End if/else

    fclose(outputFile); //Close the file.

}//End function writeSortedDataToFile

/**
 * Function printArrayContent
 * Test function to confirm content of array.
 *
 * @param arrayToPrint - Array to test
 * @param arraySize - The Size of the Array to process
 */
void printArrayContent(float arrayToPrint[], size_t arraySize) {

    float *arrayPtr;            //Trying to figure out Pointer notation
    arrayPtr = arrayToPrint;    //Array names ARE Pointers???
    for (size_t index = 0; index < arraySize; index++) {    //Iterate through the array
        //Print the value referenced by the pointer, then increment the pointer.
        printf("%d: %f\n", index + 1, *(arrayPtr++));   //There is probably a better way to do this...but it was just for a test.
    }//Next iteration

}//End function printArrayContent