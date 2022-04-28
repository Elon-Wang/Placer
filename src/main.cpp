#include <bits/stdc++.h>
#include <vector>
#include "Wire.h"
#include "Placer.h"
#include <sys/time.h>

using namespace std;
void drawBlock(int x, int y, int idx);

int main(int argc, char* argv[]) {
    // command line check
    if (argc != 2 && argc != 4 ) {
        printf("Usage: %s <benchmark-name> [--detail <1-3>]\n"
            "Example: %s cm150a --detail 3\n", argv[0], argv[0]);
        return 0;
    }
    char prefix[15] ="../benchmarks/";
	char postfix[10]   =".txt";
	char fileName[50];
	strcpy(fileName,prefix);
	strcat(fileName,argv[1]);
	strcat(fileName,postfix);

    int detail_level = 2;
    if (argc ==4 && strcmp(argv[2], "--detail") == 0) {
        sscanf(argv[3],"%d",&detail_level);  
        assert(detail_level >=1 && detail_level <= 3);
    }

    Placer target;
    if  (!target.readFile(fileName,detail_level)) {
        printf("Cannot open file: %s\n",fileName); 
        return 0;
    }

    struct timeval start, end;
    gettimeofday(&start,NULL);

    target.annealing(detail_level);

    gettimeofday(&end,NULL);
    if (detail_level ==1) {
        double total_time = (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000000.0); // get the run time by microsecond
        printf("time: %lf s\n", total_time);
    }
    freopen("/dev/tty","r",stdin);
    printf("program exit successfully\n");
    return 0;
}

