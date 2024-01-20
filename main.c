void deleteRecordByName(fichier* file, const char* name) {
    Record* current = file->r;
    Record* previous = NULL;
    int currentIndex = 0;

    // Traverse the linked list to find the record to delete
    while (current != NULL) {
        if (strcmp(current->info, name) == 0) {
            // Found the record, delete it
            if (previous == NULL) {
                // The record is the first one, update the file header
                file->r = current->svt;
            } else {
                previous->svt = current->svt;
            }

            // Mark the record as deleted in the file header
            file->t.nbrsup++;
            break;
        }

        previous = current;
        current = current->svt;
        currentIndex++;
    }

    // Optional: Seek back to the beginning of the file and update the header
    fseek(file->physicalFile, 0, SEEK_SET);
    fwrite(&(file->t), sizeof(entetf), 1, file->physicalFile);

    printf("Record deleted successfully.\n");
}
