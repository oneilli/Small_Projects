/* Program: hangman.c
   Written by: Ian O'Neill
   Written:  1/12/2020
   Description: Program to implement the hangman game in C
                User begins by inputting the name of a text file containing a single word
                Program reads in this word and initiates the game
                The user then continuously guess either the word or letters in the word.
                Input is taken until either the full word is revealed by letter guesses or the user correctly guesses the word
***NOTE: TEXT FILE MUST BE WITHIN SAME DIRECTORY AS PROGRAM***
*/

/*Preprocessing Directives*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_OF_GUESSES 10
#define READONLY "r"
#define MAX_LEN 60
#define CHAR_SIZE sizeof(char)

typedef enum {TRUE, FALSE} Boolean; /*Boolean type to be used for a function*/

/*Function Protoypes*/
Boolean str_charcheck(char c,const char str1[],char str2[],int str1_len);
int wordread(char *array_in);
void malloc_check(char *str_x);
void ignore_input(void);

int main()
{
    /*Define variables*/
    char *temp_array = malloc(MAX_LEN*CHAR_SIZE); /*defined with malloc so we can free it when its not needed anymore*/
    malloc_check(temp_array); /* Function to ensure malloc worked - details below*/
    int word_length,i,guesses = NO_OF_GUESSES;
    word_length = wordread(temp_array); /*Function to read in a word from the file. Once used we have a pointer to the string and an integer length*/
                                        /*Function explained properly at bottom of file*/
    char *word = malloc(word_length*CHAR_SIZE),*hidden_word = malloc(word_length*CHAR_SIZE); /*Malloc used to reduce memory used*/
    char word_guess[MAX_LEN];
    char letter_guess;

    malloc_check(word);
    malloc_check(hidden_word);

    strcpy(word,temp_array); /*We copy the temp_arry to word as word uses less memory*/
    free(temp_array); /*this array is no longer required*/

    /*Loop to create a string which is equal in length to the word but consists solely of asterisks*/
    for (i=0;word[i] != '\0';i++)
    {
        hidden_word[i] = '*';
    }
    hidden_word[i] = '\0';

   printf("Ready to Start!\n");
   while(guesses > 0)
   {
       /*Display game state and take user input*/
        printf("The word is: %s\n\n",hidden_word);
        printf("Number of turns remaining: %d\n",guesses);
        printf("Would you like to guess the word[w] or guess a letter[l]: ");
        do {letter_guess = getchar();} while(letter_guess == '\n' || letter_guess == ' ' ); /* Get letter input from user*/
        ignore_input(); /*Clear the input buffer to prevent error*/

        /*Case where user chooses to guess word*/
        if(letter_guess == 'w')
        {
            /*Take word string input*/
            printf("What is your guessed word?: ");
            scanf("%s",word_guess);
            printf("\n***********************************************\n");

            /*If the input is the same as the unknown word the game is over*/
            if (strcmp(word,word_guess) == 0)
            {
                printf("Congratulations!\n");
                break;
            }
            else
            {
                printf("Incorrect\n");
                guesses--;
            }
        }
        /*Case where user chooses to guess a letter*/
        else if (letter_guess == 'l')
        {
            /*Take user input for letter character*/
            printf("What letter have you chosen?: ");
            /*This do-while loop is used to ensure that the input is a lower case letter*/
            do
            {
                scanf("%c",&letter_guess);
                if ((letter_guess < 97 || letter_guess > 122) && (letter_guess != '\n' || letter_guess != ' '))
                    printf("Please input a lower-case character\n");
                ignore_input();
            }while(letter_guess < 97 || letter_guess > 122);
            printf("\n***********************************************\n");

            /*Check if letter is in word and reveal matching letters*/
            switch(str_charcheck(letter_guess,word,hidden_word,strlen(word))) /*Function has boolean type, so we use TRUE/FALSE*/
            {
                case TRUE:
                    printf("Good Choice!\n");
                    break;
                case FALSE:
                    printf("Incorrect\n");
                    guesses--;
                    break;
            }
            /*Ends game in case where user completely reveals word through letter guesses*/
            if (strcmp(word,hidden_word) == 0)
            {
                printf("Congratulations! Your letter guesses have fully revealed the word!\n");
                break;
            }
        }
        else
        {
            printf("Please input 'w' or 'l'\n");
        }
    }
    free(word);
    free(hidden_word);
    return 0;
}

/*checks whether the chosen letter is in the array*/
Boolean str_charcheck(char c,const char str1[],char str2[],int str1_len)
{
    int i;
    Boolean check = FALSE;
    /*Loop goes through the string str1, i.e. the non hidden word*/
    /*If the character input matches, the correspond element in the hidden_word representation changes*/
    for(i=0;i<str1_len;i++)
    {
        if (str1[i] == c)
        {
            str2[i] = c;
            check = TRUE;
        }
    }
    return check;
}

/*Function to read in the word from the text file*/
/*Will return both the string from the file using pointers and the length of the word using return*/
int wordread(char *array_in)
{
    /*Define file pointer*/
    FILE *ip_file;
    char filename[FILENAME_MAX + 1];
    int length;

    /*take user input*/
    printf("Give the filename with the unknown word: ");
    scanf("%s",filename);

    /*Open file and check that it worked*/
    if((ip_file = fopen(filename,READONLY)) == NULL)
    {
        fprintf(stderr,"ERROR: Could not open file!\n");
        exit(EXIT_FAILURE);
    }

    /*Copy string in file to the string array input, check that it worked*/
    /*This is done using pointers, so the input string change carries over into main*/
    if (fgets(array_in,MAX_LEN,ip_file) == NULL)
        fprintf(stderr,"ERROR: Could not read word from file!\n");

    fclose(ip_file);
    length = strlen(array_in); /*obtain the length of the string that has been read in and return it as an integer to be used in dynamic allocation later*/
    return length;
}

/*function to check if dynamic allocation was successful, as writing it out in full each time is confusing and time-consuming*/
void malloc_check(char *str_x)
{
    if(str_x == NULL)
    {
        fprintf(stderr, "\nFailed to allocate memory.");
        exit(EXIT_FAILURE);
    }
}
/*Buffer clearing input*/
void ignore_input(void)
{
    while(getchar() != '\n'){}
}
