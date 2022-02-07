#include <stdio.h>
#include "../io.h"

App inputApp() {
    while (getchar() != '\n');
    App app;

    app.id = 0;

    printf_s("\nEnter App data:");

    printf_s("\n\tEnter Title: ");
    scanf_s("%63[^\n]", app.title, TITLE_LENGTH);

    printf_s("\n\tEnter ReleaseYear: ");
    scanf_s(" %d", &app.releaseYear);

    printf_s("\n\tEnter Price: ");
    scanf_s(" %d", &app.price);

    return app;
}

void inputUpdateApp(App* app) {
    printf_s("Select column to update: \n");
    printf_s("0 - Title\n");
    printf_s("1 - ReleaseYear\n");
    printf_s("2 - Price\n");

    int column;
    scanf_s(" %d", &column);

    switch (column) {
        case 0: {
            while (getchar() != '\n');
            printf_s("Enter Title: ");
            scanf_s("%63[^\n]", app->title, TITLE_LENGTH);
        } break;
        case 1: {
            printf_s("Enter ReleaseYear: ");
            scanf_s("%d", &app->releaseYear);
        } break;
        case 2: {
            printf_s("Enter Price: ");
            scanf_s("%d", &app->price);
        } break;
        default:
            printf_s("It is not a column");
            waitEnter();
            return;
    }
}

void outputApp(const App* app) {
    printf_s("App:");
    printf_s("\n\tId: %i", app->id);
    printf_s("\n\tTitle: %s", app->title);
    printf_s("\n\tReleaseYear: %d", app->releaseYear);
    printf_s("\n\tPrice: %d\n", app->price);
}
