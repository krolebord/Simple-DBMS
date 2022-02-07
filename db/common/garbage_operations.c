#include "../../db.h"
#include <io.h>
#include <malloc.h>
#include <stdlib.h>

void writeGarbageRow(const TableData* table, int row) {
    writeEntryAtEnd(table->garbageFile, &row, GARBAGE_ENTRY_SIZE);
}

int removeLastGarbageRow(const TableData* table) {
    int res = -1;
    if (removeLastEntry(table->garbageFile, &res, GARBAGE_ENTRY_SIZE)) {
        return res;
    }
    return -1;
}

int compareRows (const void* a, const void* b) {
    return ( *(int*)a - *(int*)b );
}

void clearGarbage(const TableData* table, size_t entrySize) {
    size_t garbageFileSize = fileSize(table->garbageFile);
    size_t garbageCount = garbageFileSize / GARBAGE_ENTRY_SIZE;

    if (garbageCount == 0) {
        return;
    }

    int* garbageRows = (int*) malloc(garbageFileSize);

    fseek(table->garbageFile, 0, SEEK_SET);
    fread_s(garbageRows, garbageFileSize, garbageFileSize, 1, table->garbageFile);
    clearFile(table->garbageFile);

    qsort(garbageRows, garbageCount, GARBAGE_ENTRY_SIZE, compareRows);

    int validRowsCount = getIndexCount(table);

    IndexEntry* indexEntries = (IndexEntry*) malloc(INDEX_ENTRY_SIZE * validRowsCount);

    int currGarbageIndex = 0;
    int currGarbageRow = garbageRows[currGarbageIndex];

    void* tempEntry = malloc(entrySize);
    for (int row = 0; row < validRowsCount; ++row) {
        if (currGarbageRow == row && currGarbageIndex < garbageCount) {
            currGarbageIndex++;
            currGarbageRow = garbageRows[currGarbageIndex];
        }

        readEntryAt(table->dataFile, tempEntry, entrySize, row + currGarbageIndex);

        indexEntries[row].id = table->getId(tempEntry);
        indexEntries[row].row = row;

        writeEntryAt(table->dataFile, tempEntry, entrySize, row);
    }

    _chsize_s(_fileno(table->dataFile), validRowsCount * entrySize);

    overwriteEntries(table->indexFile, indexEntries, INDEX_ENTRY_SIZE, validRowsCount);
    sortIndex(table);

    free(tempEntry);
    free(garbageRows);
    free(indexEntries);
}
