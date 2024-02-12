# Racers Performance Recorder

This application implements a data system for a race. 

It stores the completion time of drivers from different teams. Each driver is represented by their surname, team, total hours completed, minutes, and seconds for stage completion.

## How to run this project
Step 1: Clone this repository
```
git clone https://github.com/ponyo19/Racers-Performance-Recorder.git
```

Step 2: Go to src/ directory and build the project
```
cd src
```

Step 3: Build and run the project
```
make
./project.out
```

## Commands

### Add Driver Command

Adds a driver to the system. All related statistics are initialize to 0.
```
A <surname> <team>
```
- `<surname>`: Driver’s surname (arbitrary length string).
- `<team>`: Name of the team (arbitrary length string).

Example:
```
A Lewis Mercedes
```

### Update Total Time Command

Adds specified time to the driver’s total time.
```
U <surname> <hour> <minute> <second>
```

- `<surname>`: Driver’s surname.
- `<hour>`: Number of hours to be added (non-negative integer).
- `<minute>`: Number of minutes to be added (0-59).
- `<second>`: Number of seconds to be added (0-59).

Example:
```
U Lewis 1 21 40
```

### Print Race Command
```
L
```
Displays the race time table in format 
```
<surname> <team> <hour> <minute> <second>
```

Example Output:
```
Lewis 1 21 40
Mario 1 22 10
```

### Save to File Command

Saves the database to a text file.
```
W <filename>
```

- `<filename>`: Name of the text file.

### Load from File Command

Loads drivers from a text file previously saved using Save to File command.
```
O <filename>
```

- `<filename>`: Name of the text file.

Example:
```
O test.txt
```

### Quit Program Command

Releases all allocated memory and exits the program.

```
Q
```

## Note
- If an error occurs, appropriate error messages will be displayed.
- The database entries are printed in the order of total race time, with the driver having a shorter race time printed first.




