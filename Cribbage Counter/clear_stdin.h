/*
 * stdin_clear.h
 *
 *  Created on: Mar 18, 2015
 *      Author: zackday
 */

#include <stdio.h>

#ifndef STDIN_CLEAR_H_
#define STDIN_CLEAR_H_

/*********************************
 * Steps through stdin until newline is found
 *********************************/
void clear_stdin (void)
{
  char temp [2] = {0};

  do
    {
      fgets (temp, 2, stdin);
    }
  while (strchr (temp, '\n') == NULL);

  return;
}


#endif /* STDIN_CLEAR_H_ */
