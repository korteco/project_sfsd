#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
typedef struct{
    int nbrbloc;
    int nbrsup;
    int taillef;
}entetf;

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
typedef struct {
    Record* r;
    entetf t;
    FILE* physicalFile;
    }fichie;



void lirePhysicalFile(const char* fileName) {
    FILE* physicalFile = fopen(fileName, "rb");

    if (physicalFile == NULL) {
        fprintf(stderr, "Error opening the physical file for reading.\n");
        exit(EXIT_FAILURE);
    }

    entetf fileHeader;
    fread(&fileHeader, sizeof(entetf), 1, physicalFile);

    printf("Number of blocks: %d\n", fileHeader.nbrbloc);
    printf("Number of deleted records: %d\n", fileHeader.nbrsup);
    printf("Total size of records: %d\n", fileHeader.taillef);

    fseek(physicalFile, sizeof(entetf), SEEK_SET);  // Move to the beginning of records

    Record record;

    while (fread(&record.header, sizeof(entet), 1, physicalFile) == 1) {
        fread(record.info, record.header.enreSize, 1, physicalFile);
        printf("Record Size: %d\n", record.header.enreSize);
        printf("Record Data: %s\n", record.info);
        printf("\n");
    }

    fclose(physicalFile);
}
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
fichier* createFile() {
    fichier* newFile = malloc(sizeof(fichier));
    if (newFile == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Set the size and allocate memory for the data
    newRecord->header.enreSize = dataSize;
    strcpy(newRecord->info, data);
    newFile->r = NULL;  // Initialize the record pointer to NULL
    newFile->t.nbrbloc = 0;
    newFile->t.nbrsup = 0;
    newFile->t.taillef = 0;
    newFile->physicalFile = NULL;  // Initialize the FILE pointer to NULL
    return newFile;
}
void openFile(fichier* file, const char* fileName) {
    // Try opening the physical file in read mode to check if it exists
    FILE* tempFile = fopen(fileName, "rb");
    if (tempFile == NULL) {
        // The file doesn't exist, treat it as a new file
        file->physicalFile = fopen(fileName, "wb+");
        if (file->physicalFile == NULL) {
            fprintf(stderr, "Error creating the new physical file.\n");
            exit(EXIT_FAILURE);
        }
        // Initialize the logical file structure
        file->t.nbrbloc = 0;
        file->t.nbrsup = 0;
        file->t.taillef = 0;
        // Write the initial file header
        fwrite(&(file->t), sizeof(entetf), 1, file->physicalFile);
    } else {
        // The file already exists, open it in read-write mode
        fclose(tempFile);
        file->physicalFile = fopen(fileName, "rb+");
        if (file->physicalFile == NULL) {
            fprintf(stderr, "Error opening the existing physical file.\n");
            exit(EXIT_FAILURE);
        }
        // Read the existing file header into the logical file structure
        fread(&(file->t), sizeof(entetf), 1, file->physicalFile);
    }
}
void insertRecord(fichier* file, const char* data, int dataSize) {
    // Create a new record
    Record newRecord;
    newRecord.header.enreSize = dataSize;
    strcpy(newRecord.info, data);

    // Seek to the end of the physical file to append the new record
    fseek(file->physicalFile, 0, SEEK_END);

    // Write the record to the physical file
    fwrite(&newRecord.header, sizeof(entet), 1, file->physicalFile);
    fwrite(newRecord.info, dataSize, 1, file->physicalFile);

    // Update the logical file structure
    file->t.nbrbloc++;
    file->t.taillef += dataSize;

    // Link the new record to the current head of the file
    newRecord->next = -1; // Set the 'next' field to -1 as it's a new record
    newRecord->nb = 1;    // Set the number of records in the block to 1
    newRecord->svt = *file;
    // Seek back to the beginning of the file and update the header
    fseek(file->physicalFile, 0, SEEK_SET);
    fwrite(&(file->t), sizeof(entetf), 1, file->physicalFile);

    // Update the head of the file to the new record
    *file = newRecord;
    // Optional: Seek back to the end of the file for future appends
    fseek(file->physicalFile, 0, SEEK_END);
}
void displayFile(const fichier* file) {
    entetf t = file->t;
    printf("Records in the File: %d\n", t.nbrbloc);

void displayFile(const fichier file) {
    printf("Records in the File:\n");
    fseek(file->physicalFile, sizeof(entetf), SEEK_SET);  // Move to the beginning of records

    // Traverse the linked list starting from the head (file)
    for (const Record* current = file; current != NULL; current = current->svt) {
        printf("Record Size: %d\n", current->header.enreSize);
        printf("Record Data: %s\n", current->info);
        printf("\n");
    Record record;

    while (fread(&record.header, sizeof(entet), 1, file->physicalFile) == 1) {
        fread(record.info, record.header.enreSize, 1, file->physicalFile);
        if (record.nb != -1) {  // Check if the record is not marked as deleted
            printf("Record Size: %d\n", record.header.enreSize);
            printf("Record Data: %s\n", record.info);
            printf("\n");
        }
    }
}

void deleteRecord(fichier* file, int recordIndex) {
    // Check if the record index is within the valid range
    if (recordIndex < 0 || recordIndex >= file->t.nbrbloc) {
        printf("Invalid record index.\n");
        return;
    }

    Record* current = file->r;
    Record* previous = NULL;
    int currentIndex = 0;

    // Traverse the linked list to find the record to delete
    while (current != NULL && currentIndex < recordIndex) {
        previous = current;
        current = current->svt;
        currentIndex++;
    }

    // Check if the record to delete exists
    if (current == NULL || currentIndex != recordIndex) {
        printf("Record not found.\n");
        return;
    }

    // Mark the record as deleted in the linked list
    current->nb = -1;  // Use any negative value to mark the record as deleted

    // Update the logical file structure
    file->t.nbrsup++;

    // Optional: Seek back to the beginning of the file and update the header
    fseek(file->physicalFile, 0, SEEK_SET);
    fwrite(&(file->t), sizeof(entetf), 1, file->physicalFile);

    printf("Record deleted successfully.\n");
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

    // Create a new file
    fichier* f = createFile();

    // Open the physical file and associate it with the logical file structure
    openFile(f, "your_physical_file.bin");

    int recordCounter = 0;
    char continueInput;

    do {
        lireetudiant(etudiant, &recordCounter);
        transmitData(etudiant, strlen(etudiant), buffer);
        insertRecord(&f, buffer, strlen(buffer));
        insertRecord(f, buffer, strlen(buffer));

        printf("Do you want to enter another student record? (y/n): ");
        scanf(" %c", &continueInput); // Note the space before %c to consume the newline character
        scanf(" %c", &continueInput);
        getchar(); // Consume the newline character left in the buffer

    } while (continueInput == 'y' || continueInput == 'Y');

    printf("Total number of records: %d\n", recordCounter);
    displayFile(f);

    // Free memory if needed
    while (f != NULL) {
        Record* temp = f;
        f = f->svt;
        free(temp);
    // Option to delete a record
    int deleteIndex;
    printf("Enter the index of the record to delete (or -1 to skip): ");
    scanf("%d", &deleteIndex);

    if (deleteIndex >= 0) {
        deleteRecord(f, deleteIndex);
        displayFile(f);
    }

    // Close the physical file
    if (f->physicalFile != NULL) {
        fclose(f->physicalFile);
    }

    lirePhysicalFile("your_physical_file.bin");

    // Free memory for the file structure
    free(f->r);
    free(f);

    return 0;
}
