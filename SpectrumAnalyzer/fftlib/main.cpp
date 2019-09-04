#include "fft.hpp"
#include <cstdio>
#include <time.h>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;
/*
	Generate a simple sine wave of the given frequency using the given number of points
*/

vector<float> getInputSimple(int freq, int n) {
    vector<float> values;
    for(int i=0; i<n; i++)
        values.push_back(sin(2*M_PI*freq*i/n));
    return values;
}
/*
	Create a list of values representing a sum of nfreq sine waves of frequencies in [1, nfreq] Hz
*/
vector<float> getInputMixed(int nfreq, int n) {

    vector<float> values;

    for(int i=0; i<n; i++)
        values.push_back(0);

    vector<float> freq;

    for(int i=0; i<nfreq; i++)
        freq.push_back(i+1);

    for(float f : freq)
        for(int i=0; i<values.size(); i++)
                values[i] += sin(2*M_PI*f*i/n)*(1/f);
    return values;
}

unsigned microseconds() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec*1000000 + ts.tv_nsec/1000;
}

int main() {
	unsigned t0, t1, t2;
	auto input = getInputMixed(10, 1<<16);

	t0 = microseconds();
	FFTWorker worker{17};
	t1 = microseconds();
	auto v = worker.transform(input);
	t2 = microseconds();

	float maxv=0;
	int j=0;
	for(unsigned int i=0; i<v.size(); i++) {
		if(v[i]>0.0001) {
			printf("%f @ %dHz\n", v[i], i);
		}
	}
	printf("\nStats:\n init: %d\n transform: %d\n", t1-t0, t2-t1);
	return 0;
}
