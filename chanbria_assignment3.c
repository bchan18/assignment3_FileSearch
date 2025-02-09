#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

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

char* largestFile(){
    char* largestFile = NULL;
    off_t totalSize = 0;
    int len;

    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent *entry;
    struct stat dirStat;

    // Go through all the entries
    while((entry = readdir(currDir)) != NULL){
        // Checks if file contains "movies_"
        if(strncmp(entry->d_name, "movies_", 7) == 0){
            len = strlen(entry->d_name);
            // Checks if file contains ".csv"
            if(strcmp(entry->d_name + len - 4, ".csv") == 0){
                // Get meta-data for the current entry
                stat(entry->d_name, &dirStat);
                // Checks and updates the largest file
                if(dirStat.st_size > totalSize){
                    totalSize = dirStat.st_size;
                    largestFile = strdup(entry->d_name);
                }
            }
        }
    }
    // Close the directory
    closedir(currDir);
    return largestFile;
}

char* smallestFile(){
    char* smallestFile = NULL;
    off_t totalSize = 0;
    int len;

    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent *entry;
    struct stat dirStat;

    // Go through all the entries
    while((entry = readdir(currDir)) != NULL){
        // Checks if file contains "movies_"
        if(strncmp(entry->d_name, "movies_", 7) == 0){
            len = strlen(entry->d_name);
            // Checks if file contains ".csv"
            if(strcmp(entry->d_name + len - 4, ".csv") == 0){
                // Get meta-data for the current entry
                stat(entry->d_name, &dirStat);
                if(totalSize == 0){
                    totalSize = dirStat.st_size;
                }
                // Checks and updates the smallest file
                if(dirStat.st_size < totalSize){
                    totalSize = dirStat.st_size;
                    smallestFile = strdup(entry->d_name);
                }
            }
        }
    }
    // Close the directory
    closedir(currDir);
    return smallestFile;
}

int fileExist(const char* fileName){
    DIR* currDir = opendir(".");
    struct dirent *entry;

    while((entry = readdir(currDir)) != NULL){
        if(strcmp(entry->d_name, fileName) == 0){
            closedir(currDir);
            return 1;
        }
    }
    closedir(currDir);
    return 0;
}

void processFile(){

}

int main(void){
    int choice;
    int choiceFile;
    char* file;
    char fileName[256];

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
                    file = largestFile();
                    break;
                }
    
                if(choiceFile == 2){
                    file = smallestFile();
                    break;
                }
    
                if(choiceFile == 3){
                    printf("Enter the complete file name: ");
                    scanf("%s", fileName);
                    if(fileExist(fileName)){
                        file = fileName;
                        break;
                    }
                    printf("\nThe file %s was not found. Try again\n", fileName);
                    continue;
                }
            }
        }
        printf("Now processing the chosen file named %s\n", file);
        
    }
    return 0;
}