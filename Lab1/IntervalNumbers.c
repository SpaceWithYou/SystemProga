#include "IntervalNumbers.h"

IntervalNumbers getIntervalFromInput() {
	int k1 = -1, k2 = -1, k3 = -1;
	scanf("%d %d %d", &k1, &k2, &k3);
	IntervalNumbers interval = { k1, k2, k3 };
	return interval;
}