#pragma once
#include <bits/stdc++.h>
#include "Node.h"
#include "Cell.h"

struct Wire{
    std::vector<Cell*> cellsGroup;
    int cost;
    int minX, minY, maxX, maxY;
    Wire(){}; 
    void drawWire(int groupIdx);
    int getWirelength();
};