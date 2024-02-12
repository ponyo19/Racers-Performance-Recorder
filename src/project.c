#include "project.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  // The magic starts here
  int running = 1;

  // Initialize the head node of the linked list

  driverHead.surname = NULL;
  driverHead.team = NULL;
  driverHead.hours = 0;
  driverHead.minutes = 0;
  driverHead.seconds = 0;
  driverHead.next = NULL;

  while (running == 1) {
    char buffer[1000];
    fgets(buffer, sizeof(buffer), stdin);

    running = doCommand(buffer, &driverHead);
  }
}

// Process user input
int doCommand(char *buffer, struct driver_list *q) {
  switch ((int)buffer[0]) {
    case 'A': {
      char cmd;
      char name[1000];
      memset(name, '\0', sizeof(name));
      char team[1000];
      memset(team, '\0', sizeof(team));
      int argumentsRead = sscanf(buffer, "%c %s %s", &cmd, name, team);

      // Check the number of arguments received
      if (argumentsRead != 3) {
        printf("A should be followed by exactly 2 arguments.\n");
      } else if (add_driver(q, name, team) == 1)
        printf("SUCCESS\n");
      break;
    }
    case 'U': {
      char cmd;
      int hour = 0;
      int minute = 0;
      int second = 0;
      char name[1000];
      memset(name, '\0', sizeof(name));
      int argumentsRead =
          sscanf(buffer, "%c %s %d %d %d", &cmd, name, &hour, &minute, &second);

      // Check the number of arguments received
      if (argumentsRead != 5) {
        printf("U should be followed by exactly 4 arguments.\n");
      } else if (updateTotalTime(q, name, hour, minute, second) == 1) {
        printf("SUCCESS\n");
      }
      break;
    }
    case 'L': {
      char cmd;
      int argumentsRead = sscanf(buffer, "%c", &cmd);

      // Check the number of arguments received
      if (argumentsRead != 1) {
        printf("L should not be followed by any argument.\n");
      } else if (printRace(q, stdout) == 1) {
        printf("SUCCESS\n");
      }
      break;
    }
    case 'W': {
      char cmd;
      char filename[1000];
      int argumentsRead = sscanf(buffer, "%c %s", &cmd, filename);
      if (argumentsRead != 2) {
        printf("W should be followed by exactly 1 argument.\n");
      }
      FILE *stream = fopen(filename, "w");
      if (stream == NULL) {
        break;
      }
      if (printRace(q, stream) == 1) printf("SUCCESS\n");
      fclose(stream);
      break;
    }
    case 'O': {
      char cmd;
      char filename[1000];
      int argumentsRead = sscanf(buffer, "%c %s", &cmd, filename);
      if (argumentsRead != 2) {
        printf("O should be followed by exactly 1 argument.\n");
      }

      FILE *stream = fopen(filename, "r");
      if (stream == NULL) {
        printf("Cannot open file %s for reading.\n", filename);
        break;
      }

      if (loadFromFile(q, stream) == 1) printf("SUCCESS\n");
      fclose(stream);
      break;
    }
    case 'Q':
      if (freeAll(q) == 1) printf("SUCCESS\n");
      return 0;
      break;
    default:
      printf("Invalid command %c\n", buffer[0]);
      return 1;
  }
  return 1;
}

// Add driver, return 1 on success, return 0 on failure
int add_driver(struct driver_list *q, char *name, char *team) {
  struct driver_list *thisDriver =
      (struct driver_list *)malloc(sizeof(struct driver_list));
  if (!thisDriver) {
    printf("Error: memory allocation failed\n");
    free(thisDriver);
    return 0;
  }
  thisDriver->surname = (char *)malloc((strlen(name) + 1) * sizeof(char));
  if (!thisDriver->surname) {
    printf("Error: memory allocation failed\n");
    free(thisDriver->surname);
    free(thisDriver);
    return 0;
  }
  strcpy(thisDriver->surname, name);

  thisDriver->team = (char *)malloc((strlen(team) + 1) * sizeof(char));
  if (!thisDriver->team) {
    printf("Error: memory allocation failed\n");
    free(thisDriver->team);
    free(thisDriver->surname);
    free(thisDriver);
    return 0;
  }
  strcpy(thisDriver->team, team);

  // Initially, driver has 0 driving time
  thisDriver->hours = 0;
  thisDriver->minutes = 0;
  thisDriver->seconds = 0;

  thisDriver->next = NULL;
  // Move to the last slot
  while (q) {
    if (q->surname != NULL &&
        strcmp(q->surname, name) == 0) {  // If the name already exists
      printf("Driver \"%s\" is already in the database.\n", name);
      free(thisDriver->team);
      free(thisDriver->surname);
      free(thisDriver);
      return 0;
    }
    if (q->next == NULL) {
      q->next = thisDriver;
      break;
    } else
      q = q->next;
  }
  return 1;
}

// Update total driving time. Return 1 on success, 0 on failure
int updateTotalTime(struct driver_list *q, char *name, int hour, int minute,
                    int second) {
  if (hour < 0) {
    printf("Hour cannot be negative.\n");
    return 0;
  }

  if (minute < 0 || minute > 59) {
    printf("Minute cannot be negative or greater than 59.\n");
    return 0;
  }

  if (second < 0 || second > 59) {
    printf("Second cannot be negative or greater than 59.\n");
    return 0;
  }

  while (q) {
    if (q->surname != NULL && strcmp(q->surname, name) == 0) {
      q->hours += hour;

      if (q->minutes + minute >= 60) {
        q->hours += 1;
        q->minutes = q->minutes + minute - 60;
      } else
        q->minutes += minute;

      if (q->seconds + second >= 60 && q->minutes == 59) {
        q->hours += 1;
        q->minutes = 0;
        q->seconds = q->seconds + second - 60;
      } else if (q->seconds + second >= 60) {
        q->minutes += 1;
        q->seconds = q->seconds + second - 60;
      } else
        q->seconds += second;
      return 1;
    } else
      q = q->next;
  }

  // If the driver is not found in the database
  printf("Driver \"%s\" is not in the database.\n", name);
  return 0;
}

// Displays the race time table in order of driving time. Return 1 on success, 0
// on failure.
int printRace(struct driver_list *q, FILE *stream) {
  // Count the number of added drivers so far
  struct driver_list *origin = q;
  int count = 0;
  while (q) {
    if (q->surname != NULL) count += 1;
    q = q->next;
  }

  // Reset the address of q
  q = origin;

  // Array of driving time
  int drivingTime[count];
  memset(drivingTime, -1, sizeof(drivingTime));

  int i = 0;
  while (q) {
    if (q->surname != NULL) {
      drivingTime[i] = toDrivingTime(q->hours, q->minutes, q->seconds);
      for (int z = 0; z < i; z++) {
        if (drivingTime[z] == drivingTime[i]) {
          drivingTime[i] = -1;
        }
      }
      i++;
    }
    q = q->next;
  }

  // Sort array of driving time
  qsort(drivingTime, count, sizeof(int), cmpfunc);

  // Reset address of q
  q = origin;
  // Print the drivers
  for (int j = 0; j < count; j++) {
    while (q) {
      if (q->surname != NULL) {
        if (toDrivingTime(q->hours, q->minutes, q->seconds) == drivingTime[j]) {
          fprintf(stream, "%s %s %i %i %i\n", q->surname, q->team, q->hours,
                  q->minutes, q->seconds);
        }
      }
      q = q->next;
    }
    q = origin;
  }
  return 1;
}

int loadFromFile(struct driver_list *q, FILE *stream) {
  // Clear previous data while keeping the head node
  struct driver_list *origin = q;
  struct driver_list *next = q;
  q = q->next;
  while (q) {
    next = q->next;
    free(q->surname);
    free(q->team);
    free(q);
    q = next;
  }
  q = origin;
  q->next = NULL;

  char buffer[1000];
  char name[1000];
  char team[1000];
  int hours;
  int minutes;
  int seconds;
  while (fgets(buffer, sizeof(buffer), stream)) {
    sscanf(buffer, "%s %s %i %i %i", name, team, &hours, &minutes, &seconds);
    struct driver_list *thisDriver =
        (struct driver_list *)malloc(sizeof(struct driver_list));
    if (!thisDriver) {
      printf("Error: memory allocation failed\n");
      free(thisDriver);
      return 0;
    }
    thisDriver->surname = (char *)malloc((strlen(name) + 1) * sizeof(char));
    if (!thisDriver->surname) {
      printf("Error: memory allocation failed\n");
      free(thisDriver->surname);
      free(thisDriver);
      return 0;
    }
    strcpy(thisDriver->surname, name);

    thisDriver->team = (char *)malloc((strlen(team) + 1) * sizeof(char));
    if (!thisDriver->team) {
      printf("Error: memory allocation failed\n");
      free(thisDriver->team);
      free(thisDriver->surname);
      free(thisDriver);
      return 0;
    }
    strcpy(thisDriver->team, team);

    // Set driving time
    thisDriver->hours = hours;
    thisDriver->minutes = minutes;
    thisDriver->seconds = seconds;

    thisDriver->next = NULL;
    // Move to the last slot
    while (q) {
      if (q->next == NULL) {
        q->next = thisDriver;
        break;
      } else
        q = q->next;
    }
  }
  return 1;
}

// Free all members of the linked list. Return 1 on success.
int freeAll(struct driver_list *q) {
  q = q->next;
  struct driver_list *next = q;
  while (q) {
    next = q->next;
    free(q->surname);
    free(q->team);
    free(q);
    q = next;
  }
  return 1;
}

// Auxiliary functions
int toDrivingTime(int hours, int minutes, int seconds) {
  return hours * 3600 + minutes * 60 + seconds;
}

int cmpfunc(const void *a, const void *b) { return (*(int *)a - *(int *)b); }