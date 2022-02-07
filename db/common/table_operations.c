#include "../../db.h"
#include <malloc.h>
#include "../../algorithms.h"

int findRowNumber(const TableData* table, int id) {
    int indexCount = getIndexCount(table);

    if (indexCount == 0) {
        return -1;
    }

    IndexEntry* index = (IndexEntry*)malloc(indexCount * INDEX_ENTRY_SIZE);
    readIndex(table, index, indexCount);

    int entryIndex = findFirstRowIndex(index, indexCount, id);

    if (entryIndex == -1) {
        return -1;
    }

    int row = index[entryIndex].row;

    free(index);

    return row;
}

int findRowNumbers(const TableData* table, int* numbers, int numbersCount, int id) {
    int indexCount = getIndexCount(table);

    if (indexCount == 0) {
        return 0;
    }

    IndexEntry* index = (IndexEntry*)malloc(indexCount * INDEX_ENTRY_SIZE);
    readIndex(table, index, indexCount);

    int entryIndex = findFirstRowIndex(index, indexCount, id);

    if (entryIndex == -1) {
        return 0;
    }

    for (int i = 0; i < numbersCount; ++i) {
        numbers[i] = index[entryIndex + i].row;
    }

    free(index);

    return numbersCount;
}

int readRow(const TableData* table, void* entry, int id) {
    int row = findRowNumber(table, id);

    if(row == -1) {
        return -1;
    }

    readEntryAt(table->dataFile, entry, table->entrySize, row);
    return row;
}

int readNotUniqueRow(const TableData* table, void* entry, int id, int (*compare)(void* compareData, void* b), void* compareData) {
    int rowsCount = countRows(table, id);

    int* rowNumbers = (int*) malloc(sizeof(int) * rowsCount);
    findRowNumbers(table, rowNumbers, rowsCount, id);

    for (int i = 0; i < rowsCount; ++i) {
        int rowNumber = rowNumbers[i];
        readEntryAt(table->dataFile, entry, table->entrySize, rowNumber);

        if (!compare(compareData, entry)) {
            continue;
        }

        free(rowNumbers);
        return rowNumber;
    }

    return -1;
}

int countRows(const TableData* table, int id) {
    int indexCount = getIndexCount(table);

    if (indexCount == 0) {
        return 0;
    }

    IndexEntry* index = (IndexEntry*)malloc(indexCount * INDEX_ENTRY_SIZE);
    readIndex(table, index, indexCount);

    int entryIndex = findFirstRowIndex(index, indexCount, id);

    if (entryIndex == -1) {
        return 0;
    }

    int count = 0;
    while (index[entryIndex++].id == id) {
        count++;
    }

    free(index);

    return count;
}

int readRows(const TableData* table, void* entries, int entriesCount, int id) {
    int* rowNumbers = (int*) malloc(sizeof(int) * entriesCount);
    findRowNumbers(table, rowNumbers, entriesCount, id);

    for (int i = 0; i < entriesCount; ++i) {
        readEntryAt(table->dataFile, ((char *) entries) + i * table->entrySize, table->entrySize, rowNumbers[i]);
    }

    free(rowNumbers);

    return entriesCount;
}

void insertRow(const TableData* table, void* entry, int id) {
    int row = removeLastGarbageRow(table);

    if (row == -1) {
        row = (int)(fileSize(table->dataFile) / table->entrySize);
        writeEntryAtEnd(table->dataFile, entry, table->entrySize);
    }
    else {
        writeEntryAt(table->dataFile, entry, table->entrySize, row);
    }

    IndexEntry indexEntry = { id, row };
    writeEntryAtEnd(table->indexFile, &indexEntry, INDEX_ENTRY_SIZE);
}

int updateRow(const TableData* table, void* entry) {
    int row = findRowNumber(table, table->getId(entry));

    if (row == -1) {
        return -1;
    }

    writeEntryAt(table->dataFile, entry, table->entrySize, row);

    return row;
}

int updateNotUniqueRow(const TableData* table, void* entry, int (*compare)(void* compareData, void* b), void* compareData) {
    void* tempEntry = malloc(table->entrySize);
    int row = readNotUniqueRow(table, tempEntry, table->getId(entry), compare, compareData);
    free(tempEntry);

    if (row == -1) {
        return -1;
    }

    writeEntryAt(table->dataFile, entry, table->entrySize, row);

    return row;
}

int deleteRow(const TableData* table, int id) {
    int indexCount = getIndexCount(table);

    if (indexCount == 0) {
        return -1;
    }

    IndexEntry* index = (IndexEntry*)malloc(indexCount * INDEX_ENTRY_SIZE);
    readIndex(table, index, indexCount);

    int entryIndex = findFirstRowIndex(index, indexCount, id);

    if (entryIndex == -1) {
        return -1;
    }

    int row = index[entryIndex].row;

    if (row == -1) {
        free(index);
        return -1;
    }

    for (int i = entryIndex; i < indexCount - 1; ++i) {
        writeEntryAt(table->indexFile, &index[i + 1], INDEX_ENTRY_SIZE, i);
    }
    free(index);

    IndexEntry tempEntry;
    removeLastEntry(table->indexFile, &tempEntry, INDEX_ENTRY_SIZE);
    writeGarbageRow(table, row);

    return row;
}

void deleteRows(const TableData* table, int id) {
    while (deleteRow(table, id) != -1);
}

int deleteNotUniqueRow(const TableData* table, int id, int (*compare)(void* compareData, void* b), void* compareData) {
    int indexCount = getIndexCount(table);

    if (indexCount == 0) {
        return -1;
    }

    int entriesCount = countRows(table, id);

    IndexEntry* index = (IndexEntry*)malloc(indexCount * INDEX_ENTRY_SIZE);
    readIndex(table, index, indexCount);

    int entryIndex = findFirstRowIndex(index, indexCount, id);

    if (entryIndex == -1) {
        return -1;
    }

    int row = -1;
    void* tempEntry = malloc(table->entrySize);
    for(int i = 0; i < entriesCount; ++i) {
        readEntryAt(table->dataFile, tempEntry, table->entrySize, index[entryIndex + i].row);

        if (!compare(compareData, tempEntry)) {
            continue;
        }

        entryIndex = entryIndex + i;
        row = index[entryIndex].row;
        break;
    }
    free(tempEntry);

    if (row == -1) {
        free(index);
        return -1;
    }

    for (int i = entryIndex; i < indexCount - 1; ++i) {
        writeEntryAt(table->indexFile, &index[i + 1], INDEX_ENTRY_SIZE, i);
    }
    free(index);

    IndexEntry tempIndex;
    removeLastEntry(table->indexFile, &tempIndex, INDEX_ENTRY_SIZE);
    writeGarbageRow(table, row);

    return row;
}
