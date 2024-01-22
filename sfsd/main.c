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

} entet;

typedef struct Record {
    entet header;
    char info[200];
    int next;
    int nb;

    struct Record* svt;
} Record;

typedef struct {
    Record* r;
    entetf t;
    FILE* physicalFile;
    }fichier;
/*void lirePhysicalFile(const char* fileName) {
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

    for (int i = 0; i < fileHeader.nbrbloc; i++) {
        fread(&record.header, sizeof(entet), 1, physicalFile);
        fread(record.info, record.header.enreSize, 1, physicalFile);

        printf("Record Size: %d\n", record.header.enreSize);
        printf("Record Data: %s\n", record.info);
        printf("\n");
    }

    fclose(physicalFile);
}*/
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

    while (fread(&record, sizeof(Record), 1, physicalFile) == 1) {
        if (record.nb != -1) {  // Check if the record is not marked as deleted
            printf("Record Size: %d\n", record.header.enreSize);
            printf("Record Data: %s\n", record.info);
            printf("\n");
        } else {
            // Optionally, you can indicate that the record is deleted
            printf("Record at index %d is marked as deleted.\n", i);
        }
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
fichier* createFile() {
    fichier* newFile = malloc(sizeof(fichier));
    if (newFile == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    newFile->r = NULL;  // Initialize the record pointer to NULL
    newFile->t.nbrbloc = 0;
    newFile->t.nbrsup = 0;
    newFile->t.taillef = 0;
    newFile->physicalFile = NULL;  // Initialize the FILE pointer to NULL
    return newFile;
}
void readRecordFromBuffer(const char buffer[], Record* record) {
    int offset = 0;

    // Copy the header from the buffer
    memcpy(&record->header, buffer + offset, sizeof(entet));
    offset += sizeof(entet);

    // Copy the info from the buffer
    memcpy(record->info, buffer + offset, record->header.enreSize);
}

void writeRecordToBuffer(const Record* record, char buffer[]) {
    int offset = 0;

    // Copy the header to the buffer
    memcpy(buffer + offset, &record->header, sizeof(entet));
    offset += sizeof(entet);

    // Copy the info to the buffer
    memcpy(buffer + offset, record->info, record->header.enreSize);
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

    // Use strncpy to copy data into info and ensure null termination
    strncpy(newRecord.info, data, sizeof(newRecord.info) - 1);
    newRecord.info[sizeof(newRecord.info) - 1] = '\0';

    // Seek to the end of the physical file to append the new record
    fseek(file->physicalFile, 0, SEEK_END);

    // Write the record to the physical file
    fwrite(&newRecord.header, sizeof(entet), 1, file->physicalFile);
    fwrite(newRecord.info, dataSize, 1, file->physicalFile);

    // Update the logical file structure
    file->t.nbrbloc++;
    file->t.taillef += dataSize;

    // Seek back to the beginning of the file and update the header
    fseek(file->physicalFile, 0, SEEK_SET);
    fwrite(&(file->t), sizeof(entetf), 1, file->physicalFile);

    // Optional: Seek back to the end of the file for future appends
    fseek(file->physicalFile, 0, SEEK_END);
}

void displayFile(const fichier* file) {
    entetf t = file->t;
    printf("Records in the File: %d\n", t.nbrbloc);

    fseek(file->physicalFile, sizeof(entetf), SEEK_SET);  // Move to the beginning of records

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
void rechercheEtudiant(const fichier* file, const char* matricule) {
    fseek(file->physicalFile, sizeof(entetf), SEEK_SET);  // Move to the beginning of records

    Record record;
    int recordIndex = 0;

    while (fread(&record.header, sizeof(entet), 1, file->physicalFile) == 1) {
        fread(record.info, record.header.enreSize, 1, file->physicalFile);

        // Check if the record is not marked as deleted
        if (record.nb != -1) {
            // Extract matricule from the record's info and compare
            char extractedMatricule[20];  // Adjust the size accordingly
            int result = sscanf(record.info, "%*[^#]#%*[^#]#%[^#!]", extractedMatricule);

            if (result == 1) {
                if (strcmp(extractedMatricule, matricule) == 0) {
                    // Student found, display information
                    printf("Student found at index %d:\n", recordIndex);
                    printf("Record Size: %d\n", record.header.enreSize);
                    printf("Record Data: %s\n", record.info);
                    return;
                }
            } else {
                printf("Error extracting matricule from record %d.\n", recordIndex);
            }
        }

        recordIndex++;
    }

    // If the loop completes, the student was not found
    printf("Student with matricule %s not found.\n", matricule);
}



void deleteRecord(fichier* file, int recordIndex) {
    // Check if the record index is within the valid range
    if (recordIndex < 0 || recordIndex >= file->t.nbrbloc) {
        printf("Invalid record index.\n");
        return;
    }

    Record record;

    // Move to the beginning of the file
    fseek(file->physicalFile, sizeof(entetf), SEEK_SET);

    FILE *tmpFile = fopen("tmpfile", "wb");
    if (tmpFile == NULL) {
        fclose(file->physicalFile);
        fprintf(stderr, "Error creating temporary file.\n");
        return;  // Failed to create the temporary file
    }

    int currentIndex = 0;

    // Traverse the file to find the record to delete
    while (fread(&record.header, sizeof(entet), 1, file->physicalFile) == 1) {
        fread(record.info, record.header.enreSize, 1, file->physicalFile);

        // Check if the record is not marked as deleted
        if (record.nb != -1) {
            if (currentIndex == recordIndex) {
                // Found the record, mark it as deleted
                record.nb = -1;

                // Write the updated record to mark it as deleted to the temporary file
                fwrite(&record.header, sizeof(entet), 1, tmpFile);
                fwrite(record.info, record.header.enreSize, 1, tmpFile);

                // Update the logical file structure
                file->t.nbrsup++;

                printf("Record deleted successfully.\n");
            } else {
                // Write non-deleted records to the temporary file
                fwrite(&record.header, sizeof(entet), 1, tmpFile);
                fwrite(record.info, record.header.enreSize, 1, tmpFile);
            }

            currentIndex++;
        }
    }

    fclose(file->physicalFile);
    fclose(tmpFile);

    // Remove the original file
    if (remove("your_physical_file.bin") != 0) {
        fprintf(stderr, "Error removing the original file.\n");
        return;
    }

    // Rename the temporary file to the original file name
    if (rename("tmpfile", "your_physical_file.bin") != 0) {
        fprintf(stderr, "Error renaming the temporary file.\n");
        return;
    }

    // Reopen the physical file
    file->physicalFile = fopen("your_physical_file.bin", "rb+");
    if (file->physicalFile == NULL) {
        fprintf(stderr, "Error reopening the physical file.\n");
        exit(EXIT_FAILURE);
    }

    // Optional: Seek back to the beginning of the file and update the header
    fseek(file->physicalFile, 0, SEEK_SET);
    fwrite(&(file->t), sizeof(entetf), 1, file->physicalFile);
}

void deleteRecordByName(fichier* file, const char* name) {
    Record record;

    // Move to the beginning of the file
    fseek(file->physicalFile, sizeof(entetf), SEEK_SET);

    FILE *tmpFile = fopen("tmpfile", "wb");
    if (tmpFile == NULL) {
        fclose(file->physicalFile);
        fprintf(stderr, "Error creating temporary file.\n");
        return;  // Failed to create the temporary file
    }

    int currentIndex = 0;

    // Traverse the file to find the record to delete
    while (fread(&record.header, sizeof(entet), 1, file->physicalFile) == 1) {
        fread(record.info, record.header.enreSize, 1, file->physicalFile);

        // Check if the record is not marked as deleted
        if (record.nb != -1) {
            // Extract name from the record's info and compare
            char extractedName[100];  // Adjust the size accordingly
            int result = sscanf(record.info, "%[^#!]", extractedName);

            if (result == 1) {
                if (strcmp(extractedName, name) == 0) {
                    // Found the record, mark it as deleted
                    record.nb = -1;

                    // Write the updated record to mark it as deleted to the temporary file
                    fwrite(&record.header, sizeof(entet), 1, tmpFile);
                    fwrite(record.info, record.header.enreSize, 1, tmpFile);

                    // Update the logical file structure
                    file->t.nbrsup++;

                    printf("Record deleted successfully.\n");
                } else {
                    // Write non-deleted records to the temporary file
                    fwrite(&record.header, sizeof(entet), 1, tmpFile);
                    fwrite(record.info, record.header.enreSize, 1, tmpFile);
                }
            } else {
                printf("Error extracting name from record %d.\n", currentIndex);
            }

            currentIndex++;
        }
    }

    fclose(file->physicalFile);
    fclose(tmpFile);

    // Remove the original file
    if (remove("your_physical_file.bin") != 0) {
        fprintf(stderr, "Error removing the original file.\n");
        return;
    }

    // Rename the temporary file to the original file name
    if (rename("tmpfile", "your_physical_file.bin") != 0) {
        fprintf(stderr, "Error renaming the temporary file.\n");
        return;
    }

    // Reopen the physical file
    file->physicalFile = fopen("your_physical_file.bin", "rb+");
    if (file->physicalFile == NULL) {
        fprintf(stderr, "Error reopening the physical file.\n");
        exit(EXIT_FAILURE);
    }

    // Optional: Seek back to the beginning of the file and update the header
    fseek(file->physicalFile, 0, SEEK_SET);
    fwrite(&(file->t), sizeof(entetf), 1, file->physicalFile);
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

    // Create a new file
    fichier* f = createFile();
    char name[20];
    printf("entrer le nom de fichier\n");
    scanf("%s",name);

    // Open the physical file and associate it with the logical file structure
    openFile(f, name);

    int recordCounter = 0;
    char continueInput;


   do {
        lireetudiant(etudiant, &recordCounter);
        transmitData(etudiant, strlen(etudiant), buffer);
        insertRecord(f, buffer, strlen(buffer));

        printf("Do you want to enter another student record? (y/n): ");
        scanf(" %c", &continueInput);
        getchar(); // Consume the newline character left in the buffer

    } while (continueInput == 'y' || continueInput == 'Y');

    system("cls");
     lirePhysicalFile(name);


    rechercheEtudiant(f,"741852963");
    deleteRecordByName(f,"kamiri");
    lirePhysicalFile(name);

    //Close the physical file
    if (f->physicalFile != NULL) {
        fclose(f->physicalFile);
    }
    // Free memory for the file structure
    free(f->r);
    free(f);

    return 0;
}
