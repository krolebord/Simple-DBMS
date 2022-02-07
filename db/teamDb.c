#include "../db.h"

int getTeamId(const void* teamData) {
    return ((DevTeam*)teamData)->appId;
}

int compareTeam(void* studioIdData, void* teamData) {
    DevTeam* team = (DevTeam*)teamData;
    int studioId = *((int*)studioIdData);
    return team->studioId == studioId;
}

int readTeam(const DataBase* db, int appId, int studioId, DevTeam* team) {
    return readNotUniqueRow(&db->teams, team, TEAM_SIZE, appId, compareTeam, &studioId) != -1;
}

int readTeams(const DataBase* db, int id, DevTeam* teams, int teamsCount) {
    return readRows(&db->teams, teams, TEAM_SIZE, teamsCount, id);
}

void insertTeam(const DataBase* db, DevTeam* team) {
    insertRow(&db->teams, team, TEAM_SIZE, team->appId);
    sortIndex(&db->teams);
}

int updateTeam(const DataBase* db, DevTeam* team) {
    int studioId = team->studioId;
    return updateNotUniqueRow(&db->teams, team, TEAM_SIZE, compareTeam, &studioId) != -1;
}

int deleteTeam(const DataBase* db, int appId, int studioId) {
    return deleteNotUniqueRow(&db->teams, TEAM_SIZE, appId, compareTeam, &studioId) != -1;
}
