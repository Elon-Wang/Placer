#pragma once
#include "Node.h"
#include "draw.h"

struct Cell{
    int idx;
    Node* loc;
    Cell(){};
    Cell(int cnt, Node* nodeIdx): idx(cnt){
        nodeIdx->isOccupied = true;
        nodeIdx->cellIdx = idx;
        loc = nodeIdx;
    }
    void draw();
};