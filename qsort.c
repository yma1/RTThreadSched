#include "main.h"

struct myStringStruct {
  char qstring[128];
};

struct my3DVertexStruct {
  int x, y, z;
  double distance;
};

int compare(const void *elem1, const void *elem2)
{
  int result;
  
  result = strcmp((*((struct myStringStruct *)elem1)).qstring, (*((struct myStringStruct *)elem2)).qstring);

  return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}

int compare3D(const void *elem1, const void *elem2)
{
  double distance1, distance2;

  distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
  distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

  return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}

int qsort1D(char *filePath) {
  struct myStringStruct array[MAXARRAY];
  FILE *fp;
  int i,count=0;

  fp = fopen(filePath,"r");
  while((fscanf(fp, "%s", &array[count].qstring) == 1) && (count < MAXARRAY))
    count++;

//  printf("\nSorting %d elements.\n\n",count);
  qsort(array,count,sizeof(struct myStringStruct),compare);
//  for(i=0;i<count;i++)
//    printf("%s\n", array[i].qstring);
  return 0;
}

int qsort3D(char *filePath) {
  struct my3DVertexStruct array[MAXARRAY];
  FILE *fp;
  int i,count=0;
  int x, y, z;

  fp = fopen(filePath,"r");
  
  while((fscanf(fp, "%d", &x) == 1) && 
        (fscanf(fp, "%d", &y) == 1) && 
        (fscanf(fp, "%d", &z) == 1) &&  
        (count < MAXARRAY)) 
  {
    array[count].x = x;
    array[count].y = y;
    array[count].z = z;
    array[count].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    count++;
  }
//  printf("\nSorting %d vectors based on distance from the origin.\n\n",count);
  qsort(array,count,sizeof(struct my3DVertexStruct),compare3D);
  
//  for(i=0;i<count;i++)
//    printf("%d %d %d\n", array[i].x, array[i].y, array[i].z);
  return 0;
}
