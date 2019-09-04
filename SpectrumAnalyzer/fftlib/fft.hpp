#include <vector>

#include "mailbox.h"
#include "gpu_fft.h"

class FFTWorker {
	public:
		FFTWorker(int logN=17);
		void setLogN(int logN);
		std::vector<float> transform(std::vector<float> wave);
		~FFTWorker();

	private:
		int mailbox;

		GPU_FFT* fft=nullptr;
		int nsamples = 1<<17;
		int logN = 17;
};
