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

//static int getNextTask(int *counter, int stride)
//{
//	int nextTask;
//	nextTask = *counter;
//	*counter += stride;
//	return nextTask;
//}

static void mandelbrot_kernel(int taskNo, struct static_data* sd)
{
	double y;
	double x;

	std::complex<double> Z;
	std::complex<double> C;

	int k;
	
	unsigned char (*img)[sd->x_resolution][3]
		= (unsigned char (*)[sd->x_resolution][3]) sd->img;
	int i = taskNo;
    for (int j = 0; j < sd->x_resolution; j++)
    {
        y = sd->view_y1 - i * sd->y_stepsize;
        x = sd->view_x0 + j * sd->x_stepsize;
        
        Z.real(0); Z.imag(0);
        C.real(x); C.imag(y);

        k = 0;

        do
        {
            Z = Z * Z + C;
            k++;
        } while ((Z.real()*Z.real() + Z.imag()*Z.imag()) < 4 && k < sd->max_iter);// while (cabs(Z) < 2 && k < sd->max_iter);

        if (k == sd->max_iter)
        {
            memcpy(img[i][j], "\0\0\0", 3);
        }
        else
        {
            int index = (k + sd->palette_shift)
                        % (sizeof(colors) / sizeof(colors[0]));
            memcpy(img[i][j], colors[index], 3);
        }
    }
}

void mandelbrot_draw_hpx_fine(int x_resolution, int y_resolution, int max_iter,
                                double view_x0, double view_x1, double view_y0, double view_y1,
                                double x_stepsize, double y_stepsize,
                                int palette_shift, unsigned char *img,
                                int num_threads)
{
    // Create thread-specific data
    struct static_data staticData;
    int taskCounter = 0;
    // Initializing static data
    staticData.x_resolution = x_resolution;
    staticData.y_resolution = y_resolution;
    staticData.max_iter = max_iter;
    staticData.view_x0 = view_x0;
    staticData.view_x1 = view_x1;
    staticData.view_y0 = view_y0;
    staticData.view_y1 = view_y1;
    staticData.x_stepsize = x_stepsize;
    staticData.y_stepsize = y_stepsize;
    staticData.palette_shift = palette_shift;
    staticData.img = (unsigned char *) img;
    // Initialize tasks
    int maxTasks = y_resolution;
    int taskStride = 1;
    std::vector<hpx::future<void>> futures;
//    int currentTask = getNextTask(&taskCounter, taskStride);
    int currentTask = taskCounter;
    while (currentTask < maxTasks)
    {
        futures.push_back(hpx::async(&mandelbrot_kernel, currentTask, &staticData));
//        currentTask = getNextTask(&taskCounter, taskStride);
        ++currentTask;
    }
    hpx::wait_all(futures);
}
