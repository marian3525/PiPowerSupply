#include "fft.hpp"
#include <math.h>
#include <cstdio>

using namespace std;

FFTWorker::FFTWorker(int newLogN) {

	if(logN<8 || logN>22) {
		fprintf(stderr, "logN=%d. Must be between 8 and 22. Reverting to default 17\n", newLogN);
		newLogN=17;
	}

	this->nsamples = 1<<newLogN;
	this->logN = newLogN;

	this->mailbox = mbox_open();
	int initCode = gpu_fft_prepare(mailbox, newLogN, GPU_FFT_FWD, 1, &(this->fft));

	switch(initCode) {
		case -1: fprintf(stderr, "Unable to enable V3D. Please check your firmware is up to date.\n");
		case -2: fprintf(stderr, "log2_N=%d not supported.  Try between 8 and 22.\n", newLogN);
		case -3: fprintf(stderr, "Out of memory.  Try a smaller batch or increase GPU memory.\n");
		case -4: fprintf(stderr, "Unable to map Videocore peripherals into ARM memory space.\n");
		case -5: fprintf(stderr, "Can't open libbcm_host.\n");
	}

	GPU_FFT_COMPLEX* base = fft->in;
	for(int i=0; i<nsamples; i++)
		base[i].re = base[i].im = 0;
}

vector<float> FFTWorker::transform(vector<float> waves) {
	/**
	waves: vector of sampled values from a wave or sum of waves
	return: the frequency domain of the input: the sq. root of the sum of the squares
	of the real and imaginary part of the result array
	*/
	vector<float> frequencies;

	GPU_FFT_COMPLEX* base = this->fft->in;
	for(int i=0; i<nsamples; i++) {
		base[i].re = waves[i];
	}

	gpu_fft_execute(this->fft);

	base = fft->out;

	for(int i=0; i<nsamples/2; i++) {
		frequencies.push_back(sqrt(pow(base[i].re, 2) + pow(base[i].im, 2)));
	}
	return frequencies;
}

FFTWorker::~FFTWorker() {
	gpu_fft_release(fft);
}
