#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct {
    int enreSize;      // Taille effective de l'enregistrement en octets
    char enreType[100]; // Type d'enregistrement
    // Autres informations de contrôle
} entet;

typedef struct Record {
    entet header;
    char info[200];
    int next;
    int nb;

    struct Record* svt;
} Record;

typedef Record* fichier;

void lireetudiant(char etudiant[], int* recordCounter) {
    char temp[60];

    do {
        printf("Enter the name of the student and end with #:\n");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0'; // Remove the newline character
    } while (temp[strlen(temp) - 1] != '#');

    strcpy(etudiant, temp);

    do {
        printf("Enter the surname and end with #:\n");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';
    } while (temp[strlen(temp) - 1] != '#');

    strcat(etudiant, temp);

    int validMatricule = 0;
    do {
        printf("Enter the matricule as a number and end with !:\n");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = '\0';

        // Check if the input is a number
        validMatricule = 1;
        for (int i = 0; i < strlen(temp) - 1; i++) {
            if (!isdigit(temp[i])) {
                validMatricule = 0;
                break;
            }
        }

        if (temp[strlen(temp) - 1] == '!') {
            (*recordCounter)++;
        }

        if (!validMatricule) {
            printf("Invalid input. Matricule must be a number.\n");
        }

    } while (temp[strlen(temp) - 1] != '!' || !validMatricule);

    strcat(etudiant, temp);
}

void insertRecord(fichier* file, const char* data, int dataSize) {
    // Create a new record
    Record* newRecord = malloc(sizeof(Record));
    if (newRecord == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Set the size and allocate memory for the data
    newRecord->header.enreSize = dataSize;
    strcpy(newRecord->info, data);

    // Link the new record to the current head of the file
    newRecord->next = -1; // Set the 'next' field to -1 as it's a new record
    newRecord->nb = 1;    // Set the number of records in the block to 1
    newRecord->svt = *file;

    // Update the head of the file to the new record
    *file = newRecord;
}

void displayFile(const fichier file) {
    printf("Records in the File:\n");

    // Traverse the linked list starting from the head (file)
    for (const Record* current = file; current != NULL; current = current->svt) {
        printf("Record Size: %d\n", current->header.enreSize);
        printf("Record Data: %s\n", current->info);
        printf("\n");
    }
}

void transmitData(const char* data, int dataSize, char buffer[]) {
    int i, j;

    for (i = 0, j = 0; i < dataSize; ++i, ++j) {
        if (j == BUFFER_SIZE - 1) {
            buffer[j] = '\0'; // Null-terminate the buffer
            printf("Transmitting: %s\n", buffer);

            j = 0;
        }

        buffer[j] = data[i];
    }

    if (j > 0) {
        buffer[j] = '\0';
        printf("Transmitting: %s\n", buffer);
    }
}

int main() {
    char etudiant[100];
    char buffer[BUFFER_SIZE];
    fichier f = NULL;

    int recordCounter = 0;
    char continueInput;
    do {
        lireetudiant(etudiant, &recordCounter);
        transmitData(etudiant, strlen(etudiant), buffer);
        insertRecord(&f, buffer, strlen(buffer));

        printf("Do you want to enter another student record? (y/n): ");
        scanf(" %c", &continueInput); // Note the space before %c to consume the newline character
        getchar(); // Consume the newline character left in the buffer

    } while (continueInput == 'y' || continueInput == 'Y');

    printf("Total number of records: %d\n", recordCounter);
    displayFile(f);

    // Free memory if needed
    while (f != NULL) {
        Record* temp = f;
        f = f->svt;
        free(temp);
    }

    return 0;
}
