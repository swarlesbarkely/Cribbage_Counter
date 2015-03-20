/*
 * Cribbage_Counter.c
 *
 *  Created on: Mar 16, 2015
 *      Author: zackday
 */


#include <stdio.h>
#include <string.h>
#include "verify.h"
#include "clear_stdin.h"
#include "convert_face_cards.h"

char score_from_15s = 0;
char score_from_runs = 0;
char score_from_pairs = 0; // Integrate w/ count15s
char score_from_flushes = 0;


typedef struct
{
  char face_values [5];
  char count_values [5];
  char suits [5];
} card_attributes;

card_attributes hand;


char get_suit (void)
/*********************************
 * Returns single character representing a card suit
 ********************************/
{
  char temp [2] = {0};

  fgets (temp,2,stdin);
  while (1) // will be broken by return statement
    {
      if (temp[0] == 'h' || temp[0] == 'H') return 'H'; // Replace w/ switch statement?
      else if (temp[0] == 'd' || temp[0] == 'D') return 'D';
      else if (temp[0] == 's' || temp[0] == 'S') return 'S';
      else if (temp[0] == 'c' || temp[0] == 'C') return 'C';
      else {
	  printf ("Please enter h, d, s, or c\n");
	  if (temp [1] != '\n') clear_stdin (); // Clear stdin if more than 1 letter entered
	  fgets (temp,2,stdin);
      }
    }
}

void arrange_ascending (void)
/***************************
 * Arranges face values in ascending order
 **************************/
{
  char i = 0, x = 0; // counting vars
  char temp = 0;

  for (i = 0; i < 5; i++) {

      for (x = 1; i < (5 - i); i++) {

	  if (hand.face_values [i] > hand.face_values [i+x]) {

	      temp = hand.face_values [i]; // store larger value in temp var
	      hand.face_values [i] = hand.face_values [i+x]; // move smaller value to left
	      hand.face_values [i+x] = temp; // move larger value to right
	  }
      }
  }

  return;
}

void get_hand_attributes (void)
/*******************************
 * Gets values and suits of cards in hand
 *******************************/
{
  char const counter [5][4] = {
      {"1st"},
      {"2nd"},
      {"3rd"},
      {"4th"},
      {"cut"}
  };

  char i = 0; // counting var

  for (i = 0; i < 5; i++)
    {
      printf ("Please enter the value of your %s card\n", counter [i]);
      hand.face_values [i] = (char) verify (13);

      if (hand.face_values [i] > 10) hand.count_values [i] = 10; // assigns count value of 10 for face cards
      else hand.count_values [i] = hand.face_values [i];

      printf ("Please enter the suit of your %s card\n", counter [i]);
      hand.suits [i] = get_suit ();

      printf ("\n");
      clear_stdin ();
    }
}

void count_15s_pairs (void)
/***********************************
 * Goes through each 2, 3, 4, and 5 card combinations and sees if they add up to 15
 * If the selected cards add up to 15, then score from 15s is incremented by 3
 **********************************/
{
  char counter2 = 0, counter3 = 0, counter4 = 0, stop_counter = 0, i = 0; // counting vars
  char sum = 0;

  stop_counter = 5;


  for (i = 0; i < 5; i++)
    {
      for (counter2 = 1; counter2 < stop_counter; counter2++)
	{
	  if (hand.count_values[i] + hand.count_values[counter2] == 15) score_from_15s += 3; // Combinations of 2 (10 of them)
	  else if (hand.count_values[i] == hand.count_values[counter2]) score_from_pairs += 2; // Counts pairs (else if is used since 2 cards that add up to 15 cannot be the same)

	  for (counter3 = 1; counter3 < (stop_counter-1); counter3++)
	    {
	      if (counter2+counter3+i > 4) continue; // Prevents counting past end of card array
	      if (hand.count_values[i] + hand.count_values[counter2] + hand.count_values[counter2+counter3] == 15) score_from_15s += 3; // Combinations of 3 (10 of them)

	      for (counter4 = 1; counter4 < (stop_counter-2); counter4++)
		{
		  if (counter2+counter3+counter4+i > 4) continue; // Prevents counting past end of card array
		  if (hand.count_values[0] + hand.count_values[counter2] + hand.count_values[counter2+counter3] + hand.count_values[counter2+counter3+counter4] == 15) score_from_15s += 3; // Combinations of 4 (5 of them)
		}
	    }
	}

      stop_counter--;

    }

  for (i = 0; i < 5; i++) sum += hand.count_values[i]; // Counts all cards together
  if (sum == 15) score_from_15s += 3;
}

void count_runs (void)
/******************************
 * Counts runs of 3, 4, and 5 -- arrange_ascending called to simplify this function
 *****************************/
{
  if ((hand.face_values [0] + 1) == hand.face_values [1] && (hand.face_values [1] + 1) == hand.face_values [2]) { // Looks for runs starting with lowest valued card
      score_from_runs = 3;
      if ((hand.face_values [2] + 1) == hand.face_values [3]) { // Looks for run of 4
	  score_from_runs = 4;
	  if ((hand.face_values [3] + 1) == hand.face_values [4]) { // Looks for run of 5
	      score_from_runs = 5;
	      return;
	  }
      }
  }

  else if ((hand.face_values [1] + 1) == hand.face_values [2] && (hand.face_values [2] + 1) == hand.face_values [3]) { // Looks for runs starting with second lowest valued card
      score_from_runs = 3;
      if ((hand.face_values [3] + 1) == hand.face_values [4]) {  // Looks for run of 4; Run of 5 not possible
	  score_from_runs = 4;
	  return;
      }
  }

  else if ((hand.face_values [2] + 1) == hand.face_values [3] && (hand.face_values [3] + 1) == hand.face_values [4]) score_from_runs = 3; // Last possible run of 3

  return;
}

void count_flush (void)
/**********************
 * Finds if current card contains a flush
 **********************/
{
  if (hand.suits[0] == hand.suits[1] && hand.suits[1] == hand.suits[2] && hand.suits[2] == hand.suits[3]) { // Checks for flush in dealt hand
      score_from_flushes += 4;
      if (hand.suits[0] == hand.suits[4]) score_from_flushes++; // Checks for 5 card flush w/ cut card
  }
}

int main (void)
{
  char i = 0;
  get_hand_attributes ();
  arrange_ascending ();
  count_15s_pairs ();
  count_runs ();
  count_flush ();

  for (i = 0; i < 5; i++) {
      switch (hand.face_values [i]) { // Looking for face card values to convert to letters for display --> ex. replace 13 (king) with K
	case 11: printf ("\tJ"); continue;
	case 12: printf ("\tQ"); continue;
	case 13: printf ("\tK"); continue;
	case 1: printf ("\tA"); continue;
	default: break;
      }

      printf ("\t%d", hand.face_values [i]);
  }
  printf ("\n");
  for (i = 0; i < 5; i++) printf ("\t%c", hand.suits [i]);

  printf ("\n\nScore from 15s: %d\n", score_from_15s);
  printf ("\nScore from runs: %d\n", score_from_runs);
  printf ("\nScore from flushes: %d\n", score_from_flushes);

  return 0;
}
