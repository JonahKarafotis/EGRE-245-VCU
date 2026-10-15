#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEED 21 // arbitrary
#define MAX_NUM_BLAHS 8

int main(int argc, char *argv[]) {

  if(argc < 2) { printf("Error - the number of items to generate must be on command line\n"); exit(EXIT_FAILURE); }
  int count;
  sscanf(argv[1],"%d",&count);
  if(count < 2) { printf("Error: number of items must be >= 2\n"); exit(EXIT_FAILURE); }
  srand(time(NULL));
  //  srand(SEED);
  for(int i = 1; i <= count; i++) {
    /*
      typedef struct wType {
        int id; // widget id number
        int num; // number in stock
        float cost; // our unit cost
        char *des;  // description of item
      } widget;
    */
    printf("%d\n",rand()%1000);  // id
    printf("%d\n",rand()%10000); // num
    printf("%f\n",(float)rand()/(float)(RAND_MAX)); // cost
    for(int k = 1; k <= (rand() % MAX_NUM_BLAHS)+1; k++)
      printf("%sblah",k==1?"":" ");
    printf("\n\n");
  }
}

