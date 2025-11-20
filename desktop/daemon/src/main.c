#include "../header/main.h"

int main()
{
    printf("\n");
    atexit(cleanup);
    openJSON("config/my_remaps.json");
    initEventQueue();
    initMyRemaps();
    initStatusTable();
    deleteJSON();
    printf("from %d to %d\n", 0, getKeyInfo(0)->codeOnHold);
    printf("from %d to %d\n", 2, getKeyInfo(2)->codeOnHold);
    printf("from %d to %d\n", 26, getKeyInfo(26) ? getKeyInfo(26)->codeOnHold : -1);
    
    start();
}

void cleanup()
{
    printf("CLEANUP :D \n\n");
}