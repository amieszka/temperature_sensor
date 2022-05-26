#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"
#define  _GNU_SOURCE

/* global variables declaration */
int method;

int main()
{
  printf("Choose method:\n");
  printf("1. Data from binary file.\n");
  printf("2. Data from TCP/IP.\n");
  scanf("%d", &method);
  if (method == 1) {
    read_file();
  }
  else if (method == 2) {
    printf("In progress :)");
  }
  else {
    printf("Given number is wrong, try again. :(");
  }
  
  return(0);
}
