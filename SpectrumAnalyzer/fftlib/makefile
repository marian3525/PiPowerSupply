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

F = -lrt -lm -ldl -O3 -flto -DNDEBUG -std=c++17 -finline-limit=150 -march=native

main:    $(S) $(C) $(H) fft.cpp
	g++ -o main $(F) $(C) fft.cpp main.cpp

clean:
	rm -f main




