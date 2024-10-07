/*-------------------------------------------
Program 3: Build a Word Ladder - ding dong done
Course: CS 211, Spring 2024, UIC
System: Advanced zyLab
Author: Owen Gutierrez

Description: This program allows the user to build a word ladder, in which each word is one character 
different than the one above and below it. User starts by inputting a specified word length, then inputs a 
specified max ladder height, then a start and final word. These can also be inputted using command-line 
arguments which are "-n", "-m", "-d", "-s", and "-f" respectively. Starting from the start word user is then 
asked to input valid words until you reach the final word AND have number of words equal to ladder height, 
otherwise program will terminate with an incomplete ladder. 
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//-------------------------------------------------------------------
// Task I: write the strNumMods() function, which returns the
//         count of character differences between two strings, including
//         extra characters in longer string if different lengths
//-------------------------------------------------------------------
int strNumMods(char* str1, char* str2) {
    int numDiff = 0;
 
    for(size_t i = 0; i < strlen(str1); ++i) {
        if(i == strlen(str2)) { // reached the end of str2 before str1
            numDiff += (strlen(str1) - strlen(str2)); // add difference in length to numDiff
            return numDiff;
        }
        else if(str1[i] != str2[i]) { // adds up all different characters per index
            ++numDiff;
        }
    }
    // reaching here means str2 has a length is greater than or equal to str1
    numDiff += (strlen(str2) - strlen(str1)); // add difference in length to numDiff
    return numDiff;
}


//-------------------------------------------------------------------
// Task II: write the strDiffInd() function, which returns the
//          index of the character where the two strings first differ;
//          returns strlen(str1) = strlen(str2) if no differences
//-------------------------------------------------------------------
int strDiffInd(char* str1, char* str2) {
    for(size_t i = 0; i < strlen(str1); ++i) {
        if(str1[i] != str2[i]) { // returns index of first difference of str1 and str2
            return i;
        }
    }
    return strlen(str1); // str2 is or begins as str1
}


//-------------------------------------------------------------------
// Task III: write addWord() function, which adds a new word to
//           a dynamic heap-allocated array of C-strings; heap-space must  
//           be allocated for the new word inside this function, then copy  
//           the chars from [newWord] to the newly allocated heap-space;
//           if adding the new word pushes numWords above [maxWords], then 
//           reallocate the [words] array with double the capacity
//           parameter: 
//              char*** words - array of C-strings passed-by-pointer
//              [numWords] - current # of words in [words] array
//              [maxWords] - current capacity of *words array
//              [newWord] - C-string word to be added to [words]
//-------------------------------------------------------------------
void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    if(((*numWords)+1) > (*maxWords)) { // if numWords would exceed capacity of maxWords
        // reallocates words array with new size
        (*maxWords) *= 2;
        char** updatedWords = (char**) malloc((*maxWords) * sizeof(char*)); 
        for(int h = 0; h < (*numWords); ++h) {
            updatedWords[h] = malloc((strlen((*words)[h]) + 1) * sizeof(char));
            strcpy(updatedWords[h], (*words)[h]);
            free((*words)[h]);
        }

        free(*words);
        (*words) = updatedWords;
    }
    ++(*numWords);   // increments numWords to account for new word

    // mallocs exact size of newWord (with null char) to the last element of (*words) array
    (*words)[(*numWords)-1] = (char*) malloc((strlen(newWord) + 1) * sizeof(char)); 
    strcpy((*words)[*numWords-1], newWord); // sets last element to newWord
}



//-------------------------------------------------------------------
// Task V: write the findWord() function, which returns the
//         index in the [words] C-string array where a match to the 
//         C-string [aWord] is found, using a linear search; 
//         return -1 if no match to [aWord] is found in [words] array; 
//-------------------------------------------------------------------
int findWord(char** words, int numWords, char* aWord) {
    int indFound = -1;
    // loops through each word and checks if it's the same as aWord
    for(int i = 0; i < numWords; ++i) { 
        if(strcmp(words[i], aWord) == 0) {
            indFound = i; // stores index of the word in words array matching aWord
        }
    }
    return indFound; 
}


//-------------------------------------------------------------------
// Task VI: write the validWordCheck() function, which checks 
//          if [aWord] is valid for the next word in the word ladder;
//
//          return FALSE for the following cases:
//          - [aWord] is invalid because it is not in the words array,  
//                  i.e. either not in dictionary OR wrong word length;
//          - [aWord] is invalid because it is NOT a single-char 
//                  difference from the previous word in the ladder; 
//
//          return TRUE for the following cases:
//             - [aWord] is "DONE" because the user wants to stop;
//             - [aWord] is valid and will be added to the word ladder
//
//          [words] is an array of [numWords] C-strings that contains all 
//              words in the dictionary of the correct word length
//          [ladder] is an array of [ladderHeight] C-strings that 
//              represents an incomplete word ladder
//          [aWord] is a C-string that must be checked for validity here
//-------------------------------------------------------------------
bool validWordCheck(char** words, int numWords, char** ladder, int ladderHeight, char* aWord) {
    if(strcmp(aWord, "DONE") == 0) { // DONE check
        printf("Stopping with an incomplete word ladder...\n");
        return true;
    }
    else if(findWord(words, numWords, aWord) < 0) { // word in dictionary check
        printf("Entered word NOT in dictionary. Try again...\n");
        return false;
    }  
    else if(strNumMods(ladder[ladderHeight-1], aWord) != 1) { // one word difference check
        printf("Entered word is NOT a one-character change from the previous word. Try again...\n");
        return false;
    }
    printf("Entered word is valid and will be added to the word ladder.\n"); // passes all checks
    return true;
}




//-------------------------------------------------------------------
// Task VII: write the ladderComplete() Boolean function, 
//           which should simply check for a completed word ladder, i.e.
//           return true if the final word in [ladderWords] is a match to 
//           [finalWord]; otherwise, return false; the number of words in
//           the word ladder is [n]
//-------------------------------------------------------------------
bool ladderComplete(char** ladderWords, int n, char* finalWord) {
    return (strcmp(ladderWords[n-1], finalWord) == 0);
}


//-------------------------------------------------------------------
// Task IX: write the displayIncompleteLadder() function, 
//          which should display the [n] C-strings in the [ladderWords] 
//          array with the first word at the bottom of the ladder, and 
//          each successive C-string one rung higher on the ladder. To 
//          signify the ladder as incomplete, display three lines of  
//          "..." at the top of the ladder. The ladder must be displayed 
//          with an indentation of two whitespaces on every line;
//-------------------------------------------------------------------
void displayIncompleteLadder(char** ladderWords, int n) {
    for(int h = 0; h < 3; ++h) { // prints 3 sets of 3 dots symbolizing an incomplete ladder
        printf("  ...\n");
    }
    if(n >= 1) {
        for(int i = (n-1); i >= 0; --i) { // reverse prints ladderWords
            printf("  %s\n", ladderWords[i]);
        }
    }
}


//-------------------------------------------------------------------
// Task X: write the displayCompleteLadder() function, 
//         which should display the C-strings in the [ladderWords] array 
//         with the first word at the bottom of the ladder, and each 
//         successive C-string one rung higher on the ladder. In between
//         each ladder rung, display the symbol '^' to signify the 
//         character that changes between the two rungs of the ladder. 
//         The ladder should be displayed with an indentation of two 
//         whitespaces to the left of every word;
//-------------------------------------------------------------------
void displayCompleteLadder(char** ladderWords, int n) {
    if(n >= 2) {
        int h = (n-2); // bottom word
        for(int i = (n-1); i > 0; --i) { // top word
            printf("  %s\n", ladderWords[i]);
            for(int j = 0; j < strlen(ladderWords[i]); ++j) {
                if(j == strDiffInd(ladderWords[h], ladderWords[i])) {
                    printf("  ^"); // prints out arrow where at the one character difference
                }
                else {
                    printf(" "); // prints out spaces otherwise
                }
            }
            printf("\n");
            --h;
        }
    }
    printf("  %s\n", ladderWords[0]); // prints out the bottom-most word
}


int main(int argc, char* argv[]) {

    printf("\nProcessing command-line arguments...\n");

    int wordLen = 0;
    int maxLadder = 1;
    char dict[100] = "notAfile";     
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid"; 

    //-------------------------------------------------------------------
    // Task XII: handle command-line arguments
    // command-line arguments:
    //  [-n wordLen] = sets word length for word ladder;
    //                 if [wordLen] is not a valid input 
    //                 (cannot be less than 2 or greater than 20),
    //                 or missing from command-line arguments, 
    //                 then let user set it using interactive user input
    // [-m maxLadder] = sets maximum word ladder height;
    //                  [maxLadder] must be at least 2;
    //                  if [maxLadder] is invalid or missing from 
    //                  command-line arguments, then let user set it 
    //                  using interactive user input
    // [-d dictFile] = sets dictionary file;
    //                 if [dictFile] is invalid (file not found) or 
    //                 missing from command-line arguments, then let
    //                 user set it using interactive user input
    // [-s startWord] = sets the starting word;
    //                  if [startWord] is invalid 
    //                  (not in dictionary or incorrect length) or 
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-f finalWord] = sets the final word;
    //                  if [finalWord] is invalid  
    //                  (not in dictionary or incorrect length) or 
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    //-------------------------------------------------------------------
    for(size_t i = 0; (i+1) < argc; ++i) {
        if((strcmp(argv[i], "-n") == 0) && (argv[i+1] != NULL)) {  // command-line arg for word length
            if((atoi(argv[i+1]) >= 2) && (atoi(argv[i+1]) <= 20)) {
                wordLen = atoi(argv[i+1]);
            }
        }
        else if((strcmp(argv[i], "-m") == 0) && (argv[i+1] != NULL)) { // command-line arg for max ladder
            if(atoi(argv[i+1]) >= 2) {
                maxLadder = atoi(argv[i+1]);
            }
        }
        else if((strcmp(argv[i], "-d") == 0) && (argv[i+1] != NULL)) {  // command-line arg for dictionary
            strcpy(dict, argv[i+1]);
        }
        else if((strcmp(argv[i], "-s") == 0) && (argv[i+1] != NULL)) {  // command-line arg for start word
            strcpy(startWord, argv[i+1]);
        }
        else if((strcmp(argv[i], "-f") == 0) && (argv[i+1] != NULL)) {  // command-line arg for final word
            strcpy(finalWord, argv[i+1]);
        }
    }

    printf("Interactively (re)setting invalid parameters:\n");

    // set word length using interactive user-input if necessary
    while (wordLen < 2 || wordLen > 20) {
        printf("Invalid word length = %d\n", wordLen);
        printf("Enter a valid word length (must be 2-20): ");
        scanf("%d",&wordLen);
        printf("\n");
    }

    // set max ladder height using interactive user-input if necessary
    if (maxLadder < 2) {
        printf("Invalid max ladder height = %d\n", maxLadder);
        printf("Enter a valid max ladder height (must be >1): ");
        scanf("%d",&maxLadder);
        printf("\n");
    }

    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    FILE* fileTry = fopen(dict,"r");
    while (fileTry == NULL) {
        printf("Dictionary file %s not found...\n", dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        fileTry = fopen(dict,"r");
    }
    fclose(fileTry);
    
    // build the [words] array, a heap-allocated array of C-strings
    // where the words are a read-in from the dictionary file
    // and only those words of the desired length [wordLen] 

    int numWords = 0; // initially, no words in the array
    int maxWords = 4; // initially, capacity = 4

    char** words = (char**)malloc(maxWords*sizeof(char*));
    //-------------------------------------------------------------------
    // Task IV: file-read the word list from the dictionary file 
    //          [dict], storing only words of length set by 
    //          [wordLen] in [words], which is a dynamically 
    //          growing heap-allocated array of C-strings:
    //               - the word list should be a dynamic array of 
    //                 pointers, initially with capacity (i.e. 
    //                 maximum size) 4; the capacity doubles 
    //                 whenever more space is needed
    //               - each element of the word list array should 
    //                 point to a heap-allocated C-string that 
    //                 can store a word containing exactly 
    //                 [wordLen] lower-case letters; don't forget
    //                 that there must be a single char at the 
    //                 end of the C-string for the null character
    //-------------------------------------------------------------------
    FILE* dictionary = fopen(dict, "r");
    // stores all words in dictionary with same length as wordLen in words array
    while(!feof(dictionary)) {
        char newWord[30];
        fscanf(dictionary, "%s\n", newWord);
        if(strlen(newWord) == wordLen) {
            addWord(&words, &numWords, &maxWords, newWord);
        }
    }
    fclose(dictionary);

    printf("The dictionary contains %d words of length %d.\n",numWords,wordLen);
    printf("Max size of the dynamic words array is %d.\n",maxWords);
    printf("\n");

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("Dictionary %s contains insufficient %d-letter words...\n",dict,wordLen);
        printf("Terminating program...\n");
        return -1;
    }

    // resetting start word using interactive user-input if necessary
    while (findWord(words,numWords,startWord) == -1) {
        printf("Start word %s is not valid...\n", startWord);
        printf("Enter a valid start word: ");
        scanf("%s", startWord);
        printf("\n");
    }

    // resetting final word using interactive user-input if necessary
    while (findWord(words,numWords,finalWord) == -1) {
        printf("Final word %s is not valid...\n", finalWord);
        printf("Enter a valid final word: ");
        scanf("%s", finalWord);
        printf("\n");
    }


    printf("\nWelcome to the CS 211 Word Ladder Builder!\n");
    printf("\n"); 

    printf("Your goal is to make a word ladder between two ");
    printf("%d-letter words: \n  %s -> %s\n\n",wordLen, startWord,finalWord);  
    
    // Allocating the word ladder for the maximum allowed height
    char** ladder = (char**)malloc(maxLadder*sizeof(char*));

    int ladderHeight = 0; // initially, the ladder is empty
    
    // add the start word to the ladder, i.e. the "bottom rung"
    addWord(&ladder,&ladderHeight,&maxLadder,startWord);
    
    char aWord[30] = "XYZ";
    printf("\n");

    // Let the user build a word ladder interactively & iteratively.
    // First, check that ladder is not too long AND not complete.
    //-------------------------------------------------------------------
    // Task VIII: modify the while loop condition such that the 
    //            word ladder building process continues only if ALL of  
    //            the following conditions are met:
    //                1. the ladder still has at least one available rung
    //                   before reaching the maximum allowed height
    //                2. the user is NOT attempting to stop the word ladder
    //                   building process, which occurs if the entered word 
    //                   [aWord] from the last iteration is "DONE"
    //                3. the ladder is still incomplete; i.e. the last word
    //                   added to ladder is not the final word; 
    //-------------------------------------------------------------------
    while (ladderHeight != maxLadder && strcmp(aWord, "DONE") && !ladderComplete(ladder, ladderHeight, finalWord)) {
        printf("The goal is to reach the final word: %s\n",finalWord);
        printf("The ladder is currently: \n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Current ladder height: %d\n",ladderHeight);
        printf("Maximum ladder height: %d\n",maxLadder);
        printf("Enter the next word (or DONE to stop): ");
        scanf("%s",aWord);
        printf("\n");

        // Make sure the entered word is valid for the next ladder rung;
        // if not, repeatedly allow user to enter another word until one is valid
        while (!validWordCheck(words, numWords, ladder, ladderHeight, aWord)) {
            printf("Enter another word (or DONE to stop): ");
            scanf("%s",aWord);
            printf("\n");
        }

        // add the entered word to the ladder (unless it is "DONE")
        if (strcmp(aWord,"DONE") != 0) {
            addWord(&ladder,&ladderHeight,&maxLadder,aWord);
        }
        printf("\n");

    }
    

    // Check if the built word ladder is complete and 
    // display the word ladder appropriately. 
    if (ladderComplete(ladder, ladderHeight, finalWord)) {
        printf("Word Ladder complete!\n");
        displayCompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you find a shorter Word Ladder next time??? \n");
    } else {
        if (ladderHeight == maxLadder) printf("The Word Ladder reached the maximum height.\n");
        printf("The final Word Ladder is incomplete:\n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you complete the Word Ladder next time??? \n");
    }
    
    
    //-------------------------------------------------------------------
    // Task XI: free all heap-allocated memory to avoid potential 
    //          memory leaks. Since the word length for the word 
    //          ladder is variable (i.e. set by a command-line 
    //          argument or interactive user-input) any array
    //          whose size depends on the word length should be
    //          dynamically heap-allocated, and thus, must be
    //          tracked and freed before program termination
    //-------------------------------------------------------------------
    for(int i = 0; i < numWords; ++i) { // frees all char arrays (strings) in words
        free(words[i]); // frees word
    }
    free(words); // frees the words array

    for(int j = 0; j < ladderHeight; ++j) { // frees all char arrays (strings) in ladder
        free(ladder[j]);
    }
    free(ladder); // frees the ladder array

    return 0;
}