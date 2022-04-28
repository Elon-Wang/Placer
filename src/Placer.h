#include "Node.h"
#include "Cell.h"
#include "Wire.h"


struct Placer{
    int numOfCell, numOfWire, xSize, ySize;
    int cellsInWire;
    int cellIdx;
    Node NN[10000];
    Cell cells[10000];
    Wire wires[10000];
    float avg = 0;
    int totalWL =0;
    float sum = 0;
    float deviation;
    double Temp;
    int iter;
    char text1[50];
    char text2[100];
    long long swapCnt =0;

    bool readFile(const char* testbench, int detail_level);
    void annealing(int detail_level);
    void initGraph();
    void updateGraph(char* text1, char* text2);
//    int run(const char* testbench);
};

int getGlobalWirelength(int numOfWire, Wire* wires);
void swap(Cell* cells,Node * a, Node * b);