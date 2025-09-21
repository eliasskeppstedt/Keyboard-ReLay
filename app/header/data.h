#ifndef _DATA_
#define _DATA_

#include <stdbool.h>

typedef struct lookUpTables {
    int* pWebToOS;
    int* pOSToWeb;
    int webEntries;
    int osEntries;
} lookUpTables;

typedef struct keyData {
    int keyCode;
    int keyCodeOnPress;
    int keyCodeOnHold;
    bool isPressed;
} keyData;

typedef struct layers {
    char* layerName;
    int layerNr;
    struct keyData* pRemapTable;
} layers;

typedef enum OS {
    MAC
} OS;

#define JSON_LABEL_WEB_ENTRIES "webEntries"
#define JSON_LABEL_MAC_ENTRIES "macEntries"
#define JSON_LABEL_KEYS "keys"
#define JSON_LABEL_KEY_NAME "keyName"
#define JSON_LABEL_KEY_CODE "keyCode"
#define JSON_LABEL_WEB "web"
#define JSON_LABEL_MACOS "mac"
#define JSON_LABEL_DECIMAL "dec"
#define JSON_LABEL_HEX_STRING "hexString"
#define JSON_LABEL_LAYER_ENTRIES "layerEntries"
#define JSON_LABEL_LAYERS "layers"
#define JSON_LABEL_LAYER_NAME "layerName"
#define JSON_LABEL_LAYER_NUMBER "layerNr"
#define JSON_LABEL_FROM "from"
#define JSON_LABEL_TO_ON_PRESS "toOnPress"
#define JSON_LABEL_TO_ON_HOLD "toOnHold"
#define JSON_LABEL_FROM_S "fromS"
#define JSON_LABEL_TO_ON_PRESS_S "toOnPressS"
#define JSON_LABEL_TO_ON_HOLD_S "toOnHoldS"
#define NO_VALUE -1

#endif // _DATA_