#include "mac/quartzEventService.c"

enum USER_OS { MAC, LINUX, WINDOWS };

int main()
{
    // TODO: get user's OS
    enum USER_OS os = MAC;
    //
    switch (os)
    {
    case MAC:
        cJSON* remapTable = createRemapTable();
        cJSON* macRemapTable = cJSON_GetObjectItem(remapTable, "macMapping");
        if(!macStartMonitoring(remapTable)) { printf("error"); return 1; }
        break;
    case LINUX:
        break;
    case WINDOWS:
        break;
    default:
        break;
    }
    return 0;
}