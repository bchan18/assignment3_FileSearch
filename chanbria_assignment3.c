#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define ONID "chanbria"

/* struct for movie information */
struct movie{
    char* title;
    int year;
    char* languages[5];
    float rating;
    struct movie* next;
};

/* Create a movie structure using dynamic memory allocation */
struct movie* createMovie(char* line){
    char* token;
    char* tokenLang;
    char* savePtr;
    char* savePtrLang;
    
    struct movie* currMovie = malloc(sizeof(struct movie));

    // Parsing for title
    token = strtok_r(line, ",", &savePtr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // Parsing for year
    token = strtok_r(NULL, ",", &savePtr);
    currMovie->year = atoi(token);

    // Parsing for language
    token = strtok_r(NULL, ",", &savePtr);
    if (token[0] == '['){
        token++;
        int i = 0;
        tokenLang = strtok_r(token, ";]", &savePtrLang);
        while (tokenLang && i < 5){
            currMovie->languages[i] = calloc(strlen(tokenLang) + 1, sizeof(char));
            strcpy(currMovie->languages[i], tokenLang);
            tokenLang = strtok_r(NULL, ";]", &savePtrLang);
            i++;
        }
        while (i < 5) currMovie->languages[i++] = NULL;
    }
    
    // Parsing for rating
    token = strtok_r(NULL, ",", &savePtr);
    currMovie->rating = atof(token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;
    return currMovie;
}

struct movie* processMovieFile(char* filePath){
    char *currLine = NULL;
    size_t len = 0;
    int count = 0;

    // The head of the linked list
    struct movie* head = NULL;

    // The tail of the linked list
    struct movie* tail = NULL;

    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    // Skips headerline
    getline(&currLine, &len, movieFile);

    // Read the file line by line
    while(getline(&currLine, &len, movieFile) != -1)
    {
        struct movie* currMovie = createMovie(currLine);

        // Add the movie to the linked list
        if(head == NULL){
            head = currMovie;
            tail = currMovie;
        } else{
            tail->next = currMovie;
            tail = currMovie;
        }
        count++;
    }

    // Free the memory allocated by getline for currLine
    free(currLine);

    // Close the file
    fclose(movieFile);
    return head;
}

int main(void){
    int choice;
    int choiceFile;

    while(1){
        printf("\n1. Select file to process");
        printf("\n2. Exit the program\n");
        printf("\nEnter a choice 1 or 2: ");
        scanf("%d", &choice);

        if(choice != 1 && choice != 2){
            printf("\nYou entered an incorrect choice. Try again.\n");
            continue;
        }

        if(choice == 2){
            break;
        }

        if(choice == 1){
            while(1){
                printf("\nWhich file you want to process?");
                printf("\nEnter 1 to pick the largest file");
                printf("\nEnter 2 to pick the smallest file");
                printf("\nEnter 3 to specify the name of a file\n");
                printf("\nEnter a choice from 1 to 3: ");
                scanf("%d", &choiceFile);
    
                if(choiceFile != 1 && choiceFile != 2 && choiceFile != 3){
                    printf("\nYou entered an incorrect choice. Try again.\n");
                }
    
                if(choiceFile == 1){
                    break;
                }
    
                if(choiceFile == 2){
                    break;
                }
    
                if(choiceFile == 3){
                    break;
                }
            }
        }
        continue;
    }
    return 0;
}