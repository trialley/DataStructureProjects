#include "skiplist.h"


int main () {
	zskiplist* zsl = zslCreate ();
	zslInsert (zsl, 1, 0);
	zslInsert (zsl, 2, (robj*)0234);
	zslInsert (zsl, 5, (robj*)34);
	zslDeleteByScore (zsl, 1);
	return 0;
}