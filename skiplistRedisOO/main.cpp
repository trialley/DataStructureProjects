#include"skiplist.h"


#include<iostream>
using namespace std;


int main () {
	skiplist sl;
	sl.skiplistInsert (1, 0);
	sl.skiplistInsert (2, 0);
	sl.skiplistInsert (3, 0);
	sl.skiplistDeleteByScore (2);
	return 0;
}