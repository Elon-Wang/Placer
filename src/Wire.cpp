#include "Wire.h"
#include "Node.h"
#include "Cell.h"

void Wire::drawWire(int groupIdx){
    int cnt = cellsGroup.size();
    int ii, jj;
    int color = (4 + groupIdx % 7);
    setcolor(color);
    for (ii = 0; ii < cnt; ii++) {
        Node* node1 = cellsGroup[ii]->loc;
        for (jj = ii + 1; jj < cnt; jj++) {
            Node* node2 = cellsGroup[jj]->loc;
            drawline(7 + node1->x * 10, 7 + node1->y * 10 , 7 + node2->x * 10, 7 + node2->y * 10);
        }
    }
}

int Wire::getWirelength(){
    int cnt = cellsGroup.size();
    int x = cellsGroup[0]->loc->x;
    int y = cellsGroup[0]->loc->y;
    minX = x;
    maxX = x;
    minY = y;
    maxY = y;
    for (int i = 1; i < cnt; i++) {
        x = cellsGroup[i]->loc->x;
        y = cellsGroup[i]->loc->y;
        minX = (minX > x)? x: minX;
        minY = (minY > y)? y: minY;
        maxX = (maxX < x)? x: maxX;
        maxY = (maxY < y)? y: maxY;
    }

    return (maxX + maxY - minX - minY);
}