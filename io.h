#ifndef LABA_IO_H
#define LABA_IO_H

#include "structs.h"
#include "string.h"

void waitEnter();

App inputApp();
void inputUpdateApp(App* app);
void outputApp(const App* app);

DevTeam inputTeam();
void inputUpdateTeam(DevTeam* team);
void outputTeam(const DevTeam* team);

#endif //LABA_IO_H
