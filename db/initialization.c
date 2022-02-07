#include <stdio.h>
#include <direct.h>

#include "../db.h"

errno_t openFile(FILE** file, const char* path) {
    errno_t err = fopen_s(file, path, "a+b");
    if (err != 0) {
        return err;
    }

    fclose(*file);
    err = fopen_s(file, path, "rb+");
    if (err != 0) {
        return err;
    }

    return 0;
}

errno_t openTable(
    const char* dbPath,
    const char* indexPath,
    const char* garbagePath,
    TableData* data
) {
    errno_t err;

    err = openFile(&data->dataFile, dbPath);
    if (err != 0) {
        return err;
    }

    err = openFile(&data->indexFile, indexPath);
    if (err != 0) {
        return err;
    }

    err = openFile(&data->garbageFile, garbagePath);
    if (err != 0) {
        return err;
    }

    return 0;
}

errno_t initializeDb(DataBase* db) {
    _mkdir(DB_FOLDER);

    db->apps.getId = getAppId;
    errno_t err = openTable(APP_DB_PATH, APP_INDEX_PATH, APP_GARBAGE_PATH, &db->apps);
    if (err != 0) {
        return err;
    }

    db->teams.getId = getTeamId;
    err = openTable(TEAM_DB_PATH, TEAM_INDEX_PATH, TEAM_GARBAGE_PATH, &db->teams);
    if (err != 0) {
        return err;
    }

    return 0;
}

void closeTable(TableData* tableData) {
    fclose(tableData->dataFile);
    fclose(tableData->indexFile);
    fclose(tableData->garbageFile);
}

void closeDb(DataBase db) {
    clearGarbage(&db.apps, APP_SIZE);
    closeTable(&db.apps);

    clearGarbage(&db.teams, TEAM_SIZE);
    closeTable(&db.teams);
}
