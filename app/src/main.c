#include "./../header/main.h"

int main()
{
    // TODO: get user's OS
    enum USER_OS os = MAC_OS;
    //
    switch (os)
    {
    case MAC_OS: // mac
    struct keyData* pRemapTable = createRemapTable(os);
    if(!macStartMonitoring(pRemapTable)) { printf("error"); return 1; }
        break;
    default:
        break;
    }
    return 0;
}