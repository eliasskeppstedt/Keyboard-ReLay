#include "./../header/main.h"

enum USER_OS { MAC, LINUX, WINDOWS };

int main()
{
    // TODO: get user's OS
    int os = 1;
    //
    switch (os)
    {
    case 1: // mac
        cJSON* remapTable = createRemapTable();
        cJSON* macRemapTable = cJSON_GetObjectItem(remapTable, "macMapping");
        if(!macStartMonitoring(remapTable)) { printf("error"); return 1; }
        break;
    default:
        break;
    }
    return 0;
}