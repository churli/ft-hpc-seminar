#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <complex>
#include <pthread.h>
#include <cerrno>
#include "hpx/hpx_main.hpp"
#include "hpx/include/iostreams.hpp"

#include "mandelbrot_set.h"

struct static_data
{
	int x_resolution;
	int y_resolution;
	int max_iter;
	double view_x0;
	double view_x1;
	double view_y0;
	double view_y1;
	double x_stepsize;
	double y_stepsize;
	int palette_shift;
	unsigned char* img;
};

void mandelbrot_microkernel(unsigned char *rawImg, int x_resolution, int maxIter, int paletteShift, double x, double y, int i, int j)
{
    std::complex<double> Z;
    std::complex<double> C;
    int k;
    unsigned char (*img)[x_resolution][3]
                    = (unsigned char (*)[x_resolution][3]) rawImg;
    
    Z.real(0);
    Z.imag(0);
    C.real(x);
    C.imag(y);
    
    k = 0;
    
    do
            {
                Z = Z * Z + C;
                k++;
            } while ((Z.real() * Z.real() + Z.imag() * Z.imag()) < 4 &&
                     k < maxIter);// while (cabs(Z) < 2 && k < sd->max_iter);
    
    if (k == maxIter)
            {
                memcpy(img[i][j], "\0\0\0", 3);
            }
            else
            {
                int index = (k + paletteShift)
                            % (sizeof(colors) / sizeof(colors[0]));
                memcpy(img[i][j], colors[index], 3);
            }
}

void mandelbrot_draw_hpx_ultrafine(int x_resolution, int y_resolution, int max_iter, double view_x0, double view_x1,
                              double view_y0, double view_y1, double x_stepsize, double y_stepsize, int palette_shift,
                              unsigned char *img, int num_threads, int taskStride)
{
    // Initialize tasks
    std::vector<hpx::future<void>> futures;
    for (int i=0; i<x_resolution; ++i)
    {
        for (int j=0; j<y_resolution; ++j)
        {
            double y = view_y1 - i * y_stepsize;
            double x = view_x0 + j * x_stepsize;
            futures.push_back(hpx::async(&mandelbrot_microkernel, img, x_resolution, max_iter, palette_shift, x, y, i, j));
        }
    }
    hpx::wait_all(futures);
}
