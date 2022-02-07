#include "../../db.h"
#include <stdlib.h>
#include <io.h>
#include <malloc.h>

int getNextId(const TableData* table) {
    FILE* file = table->indexFile;

    if (fileSize(file) < INDEX_ENTRY_SIZE) {
        return 1;
    }

    fseek(file, -INDEX_ENTRY_SIZE, SEEK_END);

    IndexEntry lastEntry;
    size_t entriesRead = fread(&lastEntry, INDEX_ENTRY_SIZE, 1, file);

    if (entriesRead < 1) {
        return 1;
    }

    return lastEntry.id + 1;
}

int getIndexCount(const TableData* table) {
    size_t indexSize = fileSize(table->indexFile);

    if (indexSize == 0) {
        return 0;
    }

    return (int)(indexSize / INDEX_ENTRY_SIZE);
}

void readIndex(const TableData* table, IndexEntry* entries, int count) {
    fseek(table->indexFile, 0, SEEK_SET);
    fread(entries, INDEX_ENTRY_SIZE, count, table->indexFile);
}

int indexCompare(const void* a, const void* b) {
    int firstId = ((IndexEntry*)a)->id;
    int secondId = ((IndexEntry*)b)->id;
    return (firstId - secondId);
}

void sortIndex(const TableData* table) {
    int indexCount = getIndexCount(table);

    if (indexCount == 0) {
        return;
    }

    IndexEntry* entries = (IndexEntry*) malloc(INDEX_ENTRY_SIZE * indexCount);
    readIndex(table, entries, indexCount);

    qsort(entries, indexCount, INDEX_ENTRY_SIZE, indexCompare);
    overwriteEntries(table->indexFile, entries, INDEX_ENTRY_SIZE, indexCount);
}
