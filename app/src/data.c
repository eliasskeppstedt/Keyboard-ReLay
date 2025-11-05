#include "./../header/data.h"

int getCodeOnPress(int code, int activeLayer)
{
    int codeOnPress = remapTable[activeLayer][code].codeOnPress;
    if (codeOnPress == NO_VALUE)
    {
        return code;
    }
    return codeOnPress;
}

int getCodeOnHold(int code, int activeLayer)
{
    int codeOnHold = remapTable[activeLayer][code].codeOnHold;
    if (codeOnHold == NO_VALUE)
    {
        return code;
    }
    return codeOnHold;
}

void changeToLayer(int layer, int* activeLayer)
{
    if (layer < layerEntries)
    {
        *activeLayer = layer;
    }
}