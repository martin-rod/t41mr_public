#pragma once

#include "SDT.h"

FLASHMEM void SelectBearingMap();

constexpr const char *mapFilename = "/my_map.txt";
void LoadMapFiles(const char *filename);

FLASHMEM void CheckMapList();


void DrawBearingMap(void);
