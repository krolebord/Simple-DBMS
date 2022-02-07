#include "../../db.h"
#include <io.h>

size_t fileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

size_t entriesCount(FILE* file, size_t entrySize) {
    return fileSize(file) / entrySize;
}

void readEntryAt(FILE* file, void* entry, size_t entrySize, int row) {
    fseek(file, entrySize * row, SEEK_SET);
    fread_s(entry, entrySize, entrySize, 1, file);
}

void writeEntryAt(FILE* file, const void* entry, size_t entrySize, int row) {
    fseek(file, entrySize * row, SEEK_SET);
    fwrite(entry, entrySize, 1, file);
}

void writeEntryAtEnd(FILE* file, const void* entry, size_t entrySize) {
    fseek(file, 0, SEEK_END);
    fwrite(entry, entrySize, 1, file);
}

void overwriteEntries(FILE* file, const void* entries, size_t entrySize, int entriesCount) {
    fseek(file, 0, SEEK_SET);
    fwrite(entries, entrySize, entriesCount, file);
}

int removeLastEntry(FILE* file, void* entry, const size_t entrySize) {
    size_t garbageCount = entriesCount(file, entrySize);

    if (garbageCount == 0)
        return 0;

    fseek(file, -entrySize, SEEK_END);
    fread(entry, entrySize, 1, file);

    _chsize_s(_fileno(file), (garbageCount - 1) * entrySize);

    return 1;
}

void clearFile(FILE* file) {
    _chsize_s(_fileno(file), 0);
}
