#include <stdio.h>
#include "../io.h"

DevTeam inputTeam() {
    DevTeam team;

    printf_s("\nEnter DevTeam data:");

    printf_s("\n\tEnter AppId: ");
    scanf_s("%d", &team.appId);

    printf_s("\n\tEnter StudioId: ");
    scanf_s("%d", &team.studioId);

    printf_s("\n\tEnter TeamSize: ");
    scanf_s("%d", &team.teamSize);

    return team;
}

void inputUpdateTeam(DevTeam* team) {
    printf_s("Select column to update: \n");
    printf_s("0 - StudioId\n");
    printf_s("1 - TeamSize\n");

    int column;
    scanf_s(" %d", &column);

    switch (column) {
        case 0: {
            printf_s("Enter StudioId: ");
            scanf_s("%d", &team->studioId);
        } break;
        case 1: {
            printf_s("Enter TeamSize: ");
            scanf_s("%d", &team->teamSize);
        } break;
        default:
            printf_s("It is not a column");
            waitEnter();
            return;
    }
}

void outputTeam(const DevTeam* team) {
    printf_s("DevTeam:");
    printf_s("\n\tAppId: %i", team->appId);
    printf_s("\n\tStudioId: %i", team->studioId);
    printf_s("\n\tTeamSize: %d\n", team->teamSize);
}
