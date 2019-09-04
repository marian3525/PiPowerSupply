#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <fstream>

#include "mailbox.h"
#include "gpu_fft.h"

using namespace std;

char Usage[] =
    "Usage: hello_fft.bin log2_N [jobs [loops]]\n"
    "log2_N = log2(FFT_length),       log2_N = 8...22\n"
    "jobs   = transforms per batch,   jobs>0,        default 1\n"
    "loops  = number of test repeats, loops>0,       default 1\n";


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

vector<float> getInputSimple(int freq, int n) {
    vector<float> values;
    for(int i=0; i<n; i++)
	values.push_back(sin(2*M_PI*freq*i/n));
    return values;
}

unsigned microseconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec*1000000 + ts.tv_nsec/1000;
}

int main(int argc, char *argv[]) {
    int i, j, k, ret, loops, freq, log2_N, jobs, N;
    int mailbox = mbox_open();      // mailbox descriptor
    int t0, t1;
    /*
    struct GPU_FFT_COMPLEX {
    float re, im;
    };
    */
    struct GPU_FFT_COMPLEX *base;

    /*
    struct GPU_FFT {
    struct GPU_FFT_BASE base;
    struct GPU_FFT_COMPLEX *in, *out;
    int x, y, step;
    };
    */
    struct GPU_FFT *fft;
    // use 17
    log2_N = atoi(argv[2]); //argc>1? atoi(argv[1]) : 12; // 8 <= log2_N <= 22
    jobs   = 1;	//argc>2? atoi(argv[2]) : 1;  // transforms per batch
    loops  = 1;	//argc>3? atoi(argv[3]) : 1;  // test repetitions

    N = 1<<log2_N; // FFT length
    // init the fields if GPU_FFT* fft
    ret = gpu_fft_prepare(mailbox, log2_N, GPU_FFT_FWD, jobs, &fft); // call once

    switch(ret) {
        case -1: printf("Unable to enable V3D. Please check your firmware is up to date.\n"); return -1;
        case -2: printf("log2_N=%d not supported.  Try between 8 and 22.\n", log2_N);         return -1;
        case -3: printf("Out of memory.  Try a smaller batch or increase GPU memory.\n");     return -1;
        case -4: printf("Unable to map Videocore peripherals into ARM memory space.\n");      return -1;
        case -5: printf("Can't open libbcm_host.\n");                                         return -1;
    }

    // run 1 job (jobs = 1, j=0) for 1 loop (k=0)
    // convert to forward FFT, input: an array of sin(2*pi*freq*i/N)
    //                                  i=0, N-1;
    //                          output: the frequency domain of the input:

    // set the input data
    base = fft->in + 0*fft->step; // input buffer

    auto values = getInputMixed(atoi(argv[1]), N);

    for (int i=0; i<N; i++) {
        base[i].re = values[i]/10;
	base[i].im = 0;
    }

	//int f = atoi(argv[1]);
	//base[f].re = 1;
	//base[N-f].im = 1;
	//base[3].re = 1;
	//base[N-3].re = 1;

    usleep(1); // Yield to OS
    // timing call
    t0=microseconds();
    gpu_fft_execute(fft);   // do the transform
    t1=microseconds();

    //j=0, N=FFT length
    base = fft->out + 0*fft->step; // output buffer
/*
    // print results
    vector<float> reals, imaginaries, s;

    for(int i=0; i<N; i++) {
	printf("%f+%fi ", base[i].re, base[i].im);
	fr<<base[i].re<<",";
	fi<<base[i].im<<",";
        reals.push_back(base[i].re);
	imaginaries.push_back(base[i].im);
    }

    printf("%d ", N);

    printf("\n\n\nReals:");
    for(float& re : reals)
	printf("%f ", re);

    printf("\n\n\nImaginaries:");
    for(float& im : imaginaries)
	printf("%f ", im);

    printf("\n\n\nSum:");

    for(int i=0; i<N; i++)
	printf("%f ", pow((pow(reals[i], 2)+pow(imaginaries[i], 2)), 0.5));
*/
    printf("%dus", t1-t0);

    gpu_fft_release(fft); // Videocore memory lost if not freed !
    return 0;
}
