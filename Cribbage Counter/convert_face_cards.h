/*
 * convert_face_cards.h
 *
 *  Created on: Mar 18, 2015
 *      Author: zackday
 */

#ifndef CONVERT_FACE_CARDS_H_
#define CONVERT_FACE_CARDS_H_

/****************************
 * Returns numeric value of face cards given a character input
 ****************************/
char convert_face_cards (char input)
{
  switch (input) {
    case 'k': case 'K': return 13; // Kings
    case 'q': case 'Q': return 12; // Queens
    case 'j': case 'J': return 11; // Jacks
    case 'a': case 'A': return 1; // Aces
    default: return 0;
  }
}

#endif /* CONVERT_FACE_CARDS_H_ */
