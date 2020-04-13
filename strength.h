/* 
 * strength.h
 */

#pragma once

#include "card.h"
#include "pokerhand.h"
#include <vector>

float GetImmediateStrength(vector <Card> hand, vector <Card> board, float **range);

float GetEffectiveStrength(vector <Card> hand, vector <Card> board, float **range);

float GetHandEquity(vector <Card> a, vector <Card> b, vector <Card> board);



