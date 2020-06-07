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

const int preflop_strength[13][13] = {
	// A, K,   Q,   J,   T,   9,   8,   7,   6,   5,   4,   3,   2
	{200, 197, 195, 188, 176, 171, 162, 161, 159, 160, 158, 143, 142}, // A
	{196, 199, 193, 183, 173, 162, 146, 146, 118, 104, 98,  96,  91}, // K
	{192, 186, 198, 187, 174, 160, 138, 105, 97,  96,  89,  84,  80}, // Q
	{184, 175, 172, 194, 185, 156, 128, 103, 90,  90,  80,  72,  69}, // J
	{170, 164, 167, 166, 190, 163, 141, 104, 89,  74,  71,  66,  63}, // T
	{156, 148, 147, 150, 154, 189, 155, 141, 97,  92,  58,  54,  50}, // 9
	{144, 99,  92,  101, 126, 136, 188, 155, 140, 102, 59,  45,  43}, // 8
	{116, 94,  81,  87,  76,  124, 134, 182, 154, 140, 64,  49,  33}, // 7
	{115, 85,  72,  60,  57,  55,  122, 132, 181, 154, 139, 79,  37}, // 6
	{113, 84,  67,  51,  41,  39,  44,  121, 130, 180, 140, 107, 78}, // 5
	{111, 75,  62,  46,  35,  24,  27,  29,  119, 129, 179, 138, 106}, // 4
	{109, 70,  52,  37,  31,  22,  13,  14,  18,  20,  9,   178, 105}, // 3
	{107, 65,  48,  33,  25,  16,  5,   2,   4,   11,  7,   0,   177}, // 2
 };

float GetPreflopStrength(vector <Card> hand);

float **GetRangeTable(float p_low, float p_high);

void DeleteRangeTable(float **range);
