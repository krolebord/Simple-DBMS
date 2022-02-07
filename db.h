#ifndef LABA_DB_H
#define LABA_DB_H

#define GARBAGE_ENTRY_SIZE sizeof(int)

#define DB_FOLDER "./database"

#define APP_DB_PATH "./database/app.fl"
#define APP_INDEX_PATH "./database/app.indx"
#define APP_GARBAGE_PATH "./database/app.garbg"

#define TEAM_DB_PATH "./database/team.fl"
#define TEAM_INDEX_PATH "./database/team.indx"
#define TEAM_GARBAGE_PATH "./database/team.garbg"

#include <errno.h>
#include <stdio.h>
#include "structs.h"

typedef struct TableData {
    FILE* dataFile;
    FILE* indexFile;
    FILE* garbageFile;

    int (*getId)(const void* entry);
} TableData;

typedef struct DataBase {
    TableData apps;
    TableData teams;
} DataBase;

// FILE Operations

size_t fileSize(FILE* file);

size_t entriesCount(FILE* file, size_t entrySize);

void readEntryAt(FILE* file, void* entry, size_t entrySize, int row);

void writeEntryAt(FILE* file, const void* entry, size_t entrySize, int row);
void writeEntryAtEnd(FILE* file, const void* entry, size_t entrySize);
void overwriteEntries(FILE* file, const void* entries, size_t entrySize, int entriesCount);

int removeLastEntry(FILE* file, void* entry, size_t entrySize);

void clearFile(FILE* file);

// Table Operations

int findRowNumber(const TableData* table, int id);
int findRowNumbers(const TableData* table, int* numbers, int numbersCount, int id);

int readRow(const TableData* table, void* entry, size_t entrySize, int id);
int readNotUniqueRow(const TableData* table, void* entry, size_t entrySize, int id, int (*compare)(void* compareData, void* b), void* compareData);

int countRows(const TableData* table, int id);
int readRows(const TableData* table, void* entries, size_t entrySize, int entriesCount, int id);

void insertRow(const TableData* table, void* entry, size_t entrySize, int id);

int updateRow(const TableData* table, void* entry, size_t entrySize);
int updateNotUniqueRow(const TableData* table, void* entry, size_t entrySize, int (*compare)(void* compareData, void* b), void* compareData);

int deleteRow(const TableData* table, int id);
void deleteRows(const TableData* table, int id);
int deleteNotUniqueRow(const TableData* table, size_t entrySize, int id, int (*compare)(void* compareData, void* b), void* compareData);

int getIndexCount(const TableData* table);
int getNextId(const TableData* table);
void readIndex(const TableData* table, IndexEntry* entries, int count);
void sortIndex(const TableData* table);

void writeGarbageRow(const TableData* table, int row);
int removeLastGarbageRow(const TableData* table);

void clearGarbage(const TableData* table, size_t entrySize);

// DataBase Operations

errno_t initializeDb(DataBase* db);
void closeDb(DataBase db);

int getAppId(const void* appData);
int readApp(const DataBase* db, int id, App* app);
int insertApp(const DataBase* db, App* app);
int updateApp(const DataBase* db, App* app);
int deleteApp(const DataBase* db, int id);

int getTeamId(const void* teamData);
int readTeam(const DataBase* db, int appId, int studioId, DevTeam* team);
int readTeams(const DataBase* db, int id, DevTeam* teams, int teamsCount);
void insertTeam(const DataBase* db, DevTeam* team);
int updateTeam(const DataBase* db, DevTeam* team);
int deleteTeam(const DataBase* db, int appId, int studioId);

#endif //LABA_DB_H
