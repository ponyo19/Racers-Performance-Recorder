#ifndef _PROJECT__H_
#define _PROJECT__H_
#include <stdio.h>
#endif  //! _PROJECT__H_

struct driver_list {
  char* surname;
  char* team;
  int hours;
  int minutes;
  int seconds;
  struct driver_list* next;
} driverHead;

int doCommand(char* buffer, struct driver_list* q);
int add_driver(struct driver_list* q, char* name, char* team);
int freeAll(struct driver_list* q);
int updateTotalTime(struct driver_list* q, char* name, int hour, int minute,
                    int second);

int printRace(struct driver_list* q, FILE* stream);
int loadFromFile(struct driver_list* q, FILE* stream);

int toDrivingTime(int hours, int minutes, int seconds);
int cmpfunc(const void* a, const void* b);