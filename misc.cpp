#include "misc.h"
#include <vector>
#include <string>
#include "card.h"

using namespace std;

vector <string> split(string str, char delim) {
	vector <string> splitStrings;
	size_t curr, prev = 0;
	curr = str.find(delim);
	while (curr != string::npos) {
		splitStrings.push_back(str.substr(prev, curr - prev));
		prev = curr + 1;
		curr = str.find(delim, prev);
	}
	splitStrings.push_back(str.substr(prev, curr - prev));
	return splitStrings;
}

// secure open file function
ifstream open_file(string filename) {
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) {
		cerr << "Cannot open " << filename << "!" << endl;
		exit(-1);
	}
	return fin;
}

// convert card to index
int conv(Card c) {
	return (13 * (c.GetSuit() - 1) + c.GetRank() - 1);
}

double relu(double x) {
	if (x < 0) return 0;
//if (x > 1) return 1;
	return x;
}
