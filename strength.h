/* 
 * strength.h
 */

#pragma once

#include "card.h"
#include "pokerhand.h"

float GetHandStrength(Card *ph, Card *com, int nComCards, float **range);

float GetHandEquity(Card *a, Card *b, Card *com, int nComCards);



