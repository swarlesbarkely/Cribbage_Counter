/*
 * verify.h
 *
 *  Created on: Mar 18, 2015
 *      Author: zackday
 */

#include <stdlib.h>
#include <ctype.h>
#include "clear_stdin.h"
#include "convert_face_cards.h"

#ifndef VERIFY_H_
#define VERIFY_H_

/*******************************
 * Prompts user for input until 0 < input <= limit
 ******************************/
unsigned short int verify (unsigned int limit)
{
  int check = 0;
  char temp [3];


  fgets(temp, 3, stdin);

  if (isalpha (temp [0]) && temp [1] == '\n') { // Checks for k, q, j, or a
      check = convert_face_cards (temp [0]);
  }
  else if (isdigit (temp [0])) { // Checks if first character entered is number
      check = atoi(temp);
      if (temp [1] != '\n') clear_stdin (); // Clear stdin if number entered was 2 digits
  }
  else check = 0; // Input not valid --> will trigger input loop below

  while (check > limit || check <= 0)
    {
      clear_stdin ();

      if (check <= 0)
	{
	  printf("Please enter a number greater than 0\n");
	}

      else if (check > limit)
	{
	  printf("That is not an option, please enter a number less than %d\n", limit + 1);
	}

      fgets(temp, 3, stdin);
      if (isalpha (temp [0]) && temp [1] == '\n') check = convert_face_cards (temp [0]);
      else check = atoi(temp);
    }

  //clear_stdin ();

  return check;
}


#endif /* VERIFY_H_ */
