#ifndef _INTERFACE_INITDATA_
#define _INTERFACE_INITDATA_

#include <stdio.h>

/*
 * @attention initData.h
 * @brief used by initData, create the M
 * @param int layers in total, which then will hold the remap table for that layer (a CF dict)
 */
void createRemapsArray(int size);

/*
 * @attention initData.h
 * @brief used by initData,
 */
void createRemapTableForLayer(int layer, int capacity);

/*
 * @attention initData.h
 * @brief used by initData
 */
void addRemapTableEntry(int layer, uint64_t from, uint64_t toOnPress, uint64_t toOnHold);

/*
 * @attention initData.h
 * @brief used by initData
 */
void createStatusTable(int uniqueKeyCodeEntries);

/*
 * @attention initData.h
 * @brief used by initData
 */
void addStatusTableEntry(uint64_t from);

/*
 * @attention initData.h
 * @brief used by initData
 */
void setLayerEntries(int layerEntries);

#endif // _INTERFACE_INITDATA_