#include "../db.h"

int getAppId(const void* appData) {
    return ((App*)appData)->id;
}

int readApp(const DataBase* db, int id, App* app) {
    return readRow(&db->apps, app, APP_SIZE, id) != -1;
}

int insertApp(const DataBase* db, App* app) {
    int id = getNextId(&db->apps);
    app->id = id;

    insertRow(&db->apps, app, APP_SIZE, id);

    return app->id;
}

int updateApp(const DataBase* db, App* app) {
    return updateRow(&db->apps, app, APP_SIZE) != -1;
}

int deleteApp(const DataBase* db, int id) {
    deleteRows(&db->teams, id);
    return deleteRow(&db->apps, id) != -1;
}
