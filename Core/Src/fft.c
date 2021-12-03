
#include "fft.h"


//FFT 初始化
void fft_init(uint32_t len)
{
	arm_rfft_fast_init_f32(&fft_S,128);
}
