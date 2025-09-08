#include "./../header/main.h"

int main()
{
    // TODO: get user's OS
    enum USER_OS os = MAC_OS;
    //
    switch (os)
    {
    case MAC_OS: // mac
    struct keyData* remapTable = createRemapTable(os);
    if(!macStartMonitoring(remapTable)) { printf("error"); return 1; }
        break;
    default:
        break;
    }
    return 0;
}