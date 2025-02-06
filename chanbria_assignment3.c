#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

/* struct for movie information */
struct movie{
  char* title;
  int year;
  char* languages[5];
  float rating;
  // pointer to the next element in the linked list
  struct movie* next;
};

/* Create a movie structure using dynamic memory allocation */
struct movie* createMovie(char* line){
    char* token;
    char* tokenLang;
    char* savePtr;
    char* savePtrLang;
    
    struct movie* currMovie = malloc(sizeof(struct movie));

    // Allocate memory for the title in the structure
    token = strtok_r(line, ",", &savePtr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    // Copy the value of token into the movie title in the structure
    strcpy(currMovie->title, token);

    // The value of saveptr tells strtok_r which string it should parse
    // Token contain year
    token = strtok_r(NULL, ",", &savePtr);
    currMovie->year = atoi(token);

    // Token contain language
    token = strtok_r(NULL, ",", &savePtr);
    if (token[0] == '['){
        token++;
        int i = 0;
        tokenLang = strtok_r(token, ";]", &savePtrLang);
        while (tokenLang && i < 5){
            // Allocate memory for language in the structure
            currMovie->languages[i] = calloc(strlen(tokenLang) + 1, sizeof(char));
            strcpy(currMovie->languages[i], tokenLang);
            tokenLang = strtok_r(NULL, ";]", &savePtrLang);
            i++;
        }
        // Fill the rest of the array with NULL
        while (i < 5) currMovie->languages[i++] = NULL;
    }
    
    // Token contain ratings
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
    printf("Processed file %s and parsed data for %d movies\n", filePath, count);
    return head;
}

void searchMoviesYear(struct movie* list){
    struct movie* temp = list;
    int year;
    int found = 0;

    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);
    // Prints movies that matches the year
    while (temp) {
        if (temp->year == year) {
            printf("%s\n", temp->title);
            found = 1;
        }
        temp = temp->next;
    }
    // If no matching year found
    if (found == 0) {
        printf("No data about movies released in the year %d\n", year);
    }
}

void highestRatedMovie(struct movie* list){
    int startYear = 1900;
    int endYear = 2021;
    
    // Creates an array to contain movies with highest ratings for each year
    for(int i = startYear; i <= endYear; i++ ){
        struct movie* bestMovie = NULL;

        for(struct movie* currMovie = list; currMovie != NULL; currMovie = currMovie->next){
            if (currMovie->year == i){
                if(bestMovie == NULL || currMovie->rating > bestMovie->rating){
                    bestMovie = currMovie;
                }
            }
        }
        // Print all movies with the highest ratings each year
        if (bestMovie != NULL){
            printf("%d %.1f %s\n", bestMovie->year, bestMovie->rating, bestMovie->title);
        }
    }
}

void searchMoviesLanguage(struct movie* list){
    struct movie* temp = list;
    char language[21];
    int found = 0;

    printf("Enter the language for which you want to see movies: ");
    scanf("%s", language);
    printf("\n");

    while(temp){
        // Iterates the langauge array
        for(int i = 0; i < 5; i++){
            if(temp->languages[i] != NULL && strcmp(temp->languages[i], language) == 0){
                printf("%d %s\n", temp->year, temp->title);
                found = 1;
                break;
            }
        }
        temp = temp->next;
    }
    if (found == 0){
        printf("No data about movies released in %s\n", language);
    }
}

int main ( int argc, char **argv ){
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies.csv\n");
        return EXIT_FAILURE;
    }
    struct movie* movieList = processMovieFile(argv[1]);
    
    int choiceFirst;
    int choiceSecond;

    do {
        printf("\n1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("\nEnter a choice from 1 to 2: ");
        scanf("%d", &choiceFirst);

        if (choiceFirst == 1){
            while(1) {
                printf("\nWhich file you want to process?\n");
                printf("Enter 1 to pick the largest file\n");
                printf("Enter 2 to pick the smallest file\n");
                printf("Enter 3 to specify the name of a file\n");
                printf("\nEnter a choice from 1 to 3: ");
                scanf("%d", &choiceSecond);

                if (choiceSecond == 1){
                    break;
                }

                if (choiceSecond == 2){
                    break;
                }

                if (choiceSecond == 3){
                    break;
                }
                printf("You entered an incorrect choice. Try again.\n");
            } 
        }

        if (choiceFirst != 1 && choiceFirst != 2){
            printf("You entered an incorrect choice. Try again.\n");
        }
    } while(choiceFirst != 2);

    return EXIT_SUCCESS;
}