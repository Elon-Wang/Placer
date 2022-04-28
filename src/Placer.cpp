#include "Placer.h"

bool Placer::readFile(const char* testbench, int detail_level) {
    
    if (!freopen(testbench, "r", stdin)) {
        freopen("/dev/tty", "r", stdin);
        printf("Error opening file\n"); 
        return false;
    }
    if (detail_level >=2 ) { 
        printf("reading file\n");
    }
    
    scanf("%d %d %d %d", &numOfCell, &numOfWire, &xSize, &ySize);

    for (int cnt = 0; cnt < xSize * ySize; cnt++){
        NN[cnt] = Node(cnt, xSize, ySize);
    }
    if (detail_level >=2 ) { 
        printf("random initialization\n");
    }
    
    // random initial placement 
    for (int cnt = 0; cnt < numOfCell; cnt++){
        int random = rand() % (xSize * ySize);
        while (NN[random].isOccupied) {
            random = rand() % (xSize * ySize);
        }
        cells[cnt] = Cell(cnt, &NN[random]);
    }

    if (detail_level >=2 ) { 
        printf("reading wires\n");
    }
    
    srand( (unsigned)time( NULL ) );
    for (int cnt =0; cnt < numOfWire; cnt++){
        scanf("%d", &cellsInWire);
        for(int i=0; i < cellsInWire; i++) {
            scanf("%d", &cellIdx);
            wires[cnt].cellsGroup.push_back(&cells[cellIdx]);
        }
    }
    return true;
}

void Placer::annealing(int detail_level){
    int timeOfRandomPlacement = 50;
    // perform 50 times random
    int wirelengthBuffer[timeOfRandomPlacement];
    // float avg = 0;
    for (int rdmCnt = 0; rdmCnt < timeOfRandomPlacement; rdmCnt++) {
        // initial all the nodes
        for(int cnt = 0; cnt< xSize* ySize; cnt ++){
            NN[cnt].isOccupied = false;
        }

        // random placement
        for (int cnt = 0; cnt < numOfCell; cnt++){
            int random = rand() % (xSize * ySize);
            while (NN[random].isOccupied) {
                random = rand() % (xSize * ySize);
            }
            
            // Node *tmp = &NN[rand() % (xSize * ySize)];
            cells[cnt] = Cell(cnt, &NN[random]);
        }

        // count the wirelength
        totalWL = getGlobalWirelength(numOfWire, wires);
        
        wirelengthBuffer[rdmCnt] = totalWL;
        avg += totalWL;
    }

    //calculate deviation and initial temperature and iteration per temperature
    avg = avg / (float)timeOfRandomPlacement;
    // float sum = 0;

    for (int i = 0; i< timeOfRandomPlacement; i++) {
        sum += (wirelengthBuffer[i] - avg) * (wirelengthBuffer[i] - avg);
    }
    deviation = sqrt(sum / (float)timeOfRandomPlacement);
    Temp = 20 * deviation;
    iter = ceil(10 * powf(numOfCell, 1.333));
    if (detail_level>=2) {
        //TODO modify the text
        printf("avg:%f\nsum:%f\ndeviation:%f\nTemp:%f\niter:%d\n",avg,sum,deviation,Temp,iter);    
        printf("start annealing!\n");
    }

    // char text1[50];
    // char text2[100];
    // long long swapCnt =0;

    if (detail_level == 3) {
        sprintf(text1,"Initial Placement. Total_iter:%d",iter);
        sprintf(text2,"iter:%lld  Temp:%f  Wirelength:%d",swapCnt,Temp,totalWL);
        //drawing
        this->initGraph();
        this->updateGraph(text1, text2);
    }

    // annealing process
    while (Temp >0) {
        for (int cnt = 0; cnt < iter; cnt++) {
            swapCnt++;
            int oldCost = getGlobalWirelength(numOfWire, wires);
            //randomly choose one cell
            Node* swap1 = cells[rand() % (numOfCell)].loc;
            Node* swap2 = &NN[rand() % (xSize * ySize)];
            
            //randomly choose the other cell / node to swap
            swap(cells, swap1, swap2);
            int newCost = getGlobalWirelength(numOfWire, wires);
            int deltaCost = newCost - oldCost;
            float r = (rand()%1000 +1) / 1000.0;
            double rst = exp(-deltaCost / Temp);
            if ( r > rst) {
                swap(cells,swap1,swap2);
            } else {
                if (Temp <0.08 && deltaCost>0 && detail_level>=2) {
                    printf("swapped\nr:%f\t exp:%lf\n",r, rst);
                    printf("temp:%lf\tdeltacost:%d\trst:%lf\tr:%f\n",Temp,deltaCost,rst,r);
                }
            }
            if(swapCnt%5000==0 && detail_level>=2) {
                printf("iter:%lld\tTemp:%f\tcost:%d\n",swapCnt,Temp,oldCost);
                if (detail_level==3) {
                    sprintf(text2,"iter:%lld  Temp:%f  Wirelength:%d",swapCnt,Temp,oldCost);
                    this->updateGraph(text2, text2);
                }
            }
        }
        Temp = 0.7 * Temp -0.01;
    }

    printf("annealing finished\ntotal swapcnt:%lld\titer:%d\n",swapCnt,iter);
    if (detail_level==3) {
        totalWL = getGlobalWirelength(numOfWire, wires);
        sprintf(text1,"Final placement layout");
        sprintf(text2,"iter:%lld  Temp:%f  Wirelength:%d",swapCnt,0.0,totalWL);
        this->updateGraph(text1, text2);
    }
}

void Placer::initGraph(){
    char txt[10] = "Placer";
    init_graphics(txt);
    init_world(0,0,5+xSize*10, 5+ySize*10);
    setlinewidth(2);
    setlinestyle(SOLID);
    setcolor(BLACK);
    clearscreen();
}

void Placer::updateGraph(char* text1, char* text2){
    clearscreen();
    for(int i=0;i<=xSize;i++){
        drawline(2+i*10,2,2+i*10, 2+ySize*10);
    }
    for(int j=0;j<=ySize;j++){
        drawline(2,2+j*10,2+xSize*10,2+j*10);
    }
    for (int i= 0; i < numOfCell; i++) {
        cells[i].draw();
    }
    update_message(text1);
    event_loop(button_press,drawscreen);
    for(int i = 0; i < numOfWire; i++) {
        wires[i].drawWire(i);
    }
    update_message(text2);
    event_loop(button_press,drawscreen); 
}

int getGlobalWirelength(int numOfWire, Wire* wires){
    int totalWL = 0;
    for(int i = 0; i < numOfWire; i++) {
            totalWL += wires[i].getWirelength();
    }
    return totalWL;
}

void swap(Cell* cells,Node * a, Node * b) {
    if (a->isOccupied) {
        cells[a->cellIdx].loc = b;
    }
    if (b->isOccupied) {
        cells[b->cellIdx].loc = a;
    }
    int aIdx = (a->isOccupied)? a->cellIdx: -1;
    int bIdx = (b->isOccupied)? b->cellIdx: -1;
    b->cellIdx = aIdx;
    a->cellIdx = bIdx;
    a->isOccupied = (bIdx == -1)? false: true;
    b->isOccupied = (aIdx == -1)? false: true;
}