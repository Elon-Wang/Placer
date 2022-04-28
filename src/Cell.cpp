#include "Node.h"
#include "Cell.h"
#include "draw.h"

void Cell::draw(){
    int color = 3;
    int x = this->loc->x;
    int y = this->loc->y;
    setcolor(color);
    fillrect(2.2 + x * 10, 2.2 + y * 10, 1.8 + (x + 1) * 10, 1.8 + (y + 1) * 10);

    setfontsize(20);
    setcolor(4);
    char idxNum[10];
    sprintf(idxNum, "%d", this->idx);
    drawtext(7 + x * 10, 7 + y * 10, idxNum, 10);
}