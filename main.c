#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "io.h"
#include "db.h"

void get_m(DataBase* db);

void get_s(DataBase* db);

void get_many_s(DataBase* db);

void update_m(DataBase* db);

void update_s(DataBase* db);

void insert_m(DataBase* db);

void insert_s(DataBase* db);

void del_m(DataBase* db);

void del_s(DataBase* db);

int main() {
    DataBase database;
    DataBase* db = &database;

    errno_t initializationError = initializeDb(db);

    if (initializationError != 0) {
        char errBuffer[64];
        strerror_s(errBuffer, 64, initializationError);
        printf_s("\nCouldn't initialize db\nError: %s\n", errBuffer);
        waitEnter();
        return initializationError;
    }

    int action;
    while (1) {
        system("cls");
        printf_s("%s\n", "Choose action");
        printf_s("%s\n", "0. Get App");
        printf_s("%s\n", "1. Get Team");
        printf_s("%s\n", "2. Get App Teams");
        printf_s("%s\n", "3. Delete App");
        printf_s("%s\n", "4. Delete Team");
        printf_s("%s\n", "5. Update App");
        printf_s("%s\n", "6. Update Team");
        printf_s("%s\n", "7. Add App");
        printf_s("%s\n", "8. Add Team");
        printf_s("%s\n", "999. Quit");

        scanf_s(" %d", &action);

        switch (action) {
            case 0:
                get_m(db);
                break;
            case 1:
                get_s(db);
                break;
            case 2:
                get_many_s(db);
                break;
            case 3:
                del_m(db);
                break;
            case 4:
                del_s(db);
                break;
            case 5:
                update_m(db);
                break;
            case 6:
                update_s(db);
                break;
            case 7:
                insert_m(db);
                break;
            case 8:
                insert_s(db);
                break;

            case 999:
                closeDb(database);
                db = NULL;
                return 0;

            default:
                printf_s("%s\n", "Invalid action");
                waitEnter();
                break;
        }
    }
}

void insert_m(DataBase* db) {
    App app = inputApp();
    insertApp(db, &app);
    printf_s("Inserted with Id: %d\n", app.id);
    waitEnter();
}

void insert_s(DataBase* db) {
    DevTeam team = inputTeam();
    insertTeam(db, &team);
    printf_s("Inserted with AppId: %d\n", team.appId);
    waitEnter();
}

void update_m(DataBase* db) {
    App app;
    app.id = 0;
    printf_s("Enter AppId: ");
    scanf_s("%d", &app.id);
    readApp(db, app.id, &app);

    inputUpdateApp(&app);

    if (updateApp(db, &app)) {
        printf_s("\nSuccessfully updated app");
    }
    else {
        printf_s("\nUnable to update app");
    }

    waitEnter();
}

void update_s(DataBase* db) {
    DevTeam team;

    printf_s("Enter AppId: ");
    scanf_s("%d", &team.appId);

    printf_s("Enter StudioId: ");
    scanf_s("%d", &team.studioId);

    readTeam(db, team.appId, team.studioId, &team);

    inputUpdateTeam(&team);

    if (updateTeam(db, &team)) {
        printf_s("\nSuccessfully updated team");
    }
    else {
        printf_s("\nUnable to update team");
    }

    waitEnter();
}

void get_m(DataBase* db) {
    int id = 0;

    printf_s("\nEnter id: ");
    scanf_s(" %d", &id);

    App app;
    if(readApp(db, id, &app)) {
        outputApp(&app);
    }
    else {
        printf_s("App not found\n");
    }

    waitEnter();
}

void get_s(DataBase* db) {
    int appId = 0;
    printf_s("\nEnter AppId: ");
    scanf_s(" %d", &appId);

    int studioId = 0;
    printf_s("\nEnter StudioId: ");
    scanf_s(" %d", &studioId);

    DevTeam team;
    if(readTeam(db, appId, studioId, &team)) {
        outputTeam(&team);
    }
    else {
        printf_s("Team not found\n");
    }

    waitEnter();
}

void get_many_s(DataBase* db) {
    int appId = 0;
    printf_s("\nEnter AppId: ");
    scanf_s(" %d", &appId);

    int teamsCount = countRows(&db->teams, appId);
    DevTeam* teams = (DevTeam*) malloc(TEAM_SIZE * teamsCount);
    readTeams(db, appId, teams, teamsCount);

    if(teamsCount == 0) {
        printf_s("Teams not found\n");
    }
    else {
        for (int i = 0; i < teamsCount; ++i) {
            outputTeam(&teams[i]);
        }
    }

    waitEnter();
    free(teams);
}

void del_m(DataBase* db) {
    int id = 0;

    printf_s("\nEnter id: ");
    scanf_s(" %d", &id);

    if(deleteApp(db, id)) {
        printf_s("App deleted\n");
    }
    else {
        printf_s("App not found\n");
    }

    waitEnter();
}

void del_s(DataBase* db) {
    int appId = 0;
    printf_s("\nEnter AppId: ");
    scanf_s(" %d", &appId);

    int studioId = 0;
    printf_s("\nEnter StudioId: ");
    scanf_s(" %d", &studioId);

    if(deleteTeam(db, appId, studioId)) {
        printf_s("Team deleted\n");
    }
    else {
        printf_s("Team not found\n");
    }

    waitEnter();
}
