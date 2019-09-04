S = hex/shader_256.hex \
    hex/shader_512.hex \
    hex/shader_1k.hex \
    hex/shader_2k.hex \
    hex/shader_4k.hex \
    hex/shader_8k.hex \
    hex/shader_16k.hex \
    hex/shader_32k.hex \
    hex/shader_64k.hex \
    hex/shader_128k.hex \
    hex/shader_256k.hex \
    hex/shader_512k.hex \
    hex/shader_1024k.hex \
    hex/shader_2048k.hex \
    hex/shader_4096k.hex

C = mailbox.c gpu_fft.c gpu_fft_base.c gpu_fft_twiddles.c gpu_fft_shaders.c

H = mailbox.h 

F = -lrt -lm -ldl -O3 -flto -fpermissive

all:	fft_rev.bin fft_fwd.bin

fft_rev.bin:	$(S) $(C) $(H) fft_rev.c
	g++ -o fft_rev.bin $(F) $(C) fft_rev.c

fft_fwd.bin:	$(S) $(C) $(H) fft_fwd.c
	g++ -o fft_fwd.bin $(F) $(C) fft_fwd.c

clean:
	rm -f fft_rev.bin fft_fwd.bin

