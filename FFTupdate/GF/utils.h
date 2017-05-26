/*
 * utils.h
 *
 *  Created on: Jan 3, 2016
 *      Author: matan
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdio>
#include <cstdlib>
#include <cuda_runtime_api.h>

#ifdef __CUDACC__
#define DEVICE_FUNCTION __device__
#define HOST_FUNCTION __host__
#define HOST_DEVICE_FUNCTION __host__ __device__
#else
#define DEVICE_FUNCTION
#define HOST_FUNCTION
#define HOST_DEVICE_FUNCTION
#endif

#define WARP_SIZE 32
#define LOG_WARP_SIZE 5

#define BITS_IN_BYTE 8

#ifndef MAX_THREADBLOCK_SIZE
#define MAX_THREADBLOCK_SIZE 1024
#endif

// Computes the ciel of x/y.
#define CIEL(x,y) (((x)+(y) - 1) / (y))

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
#define regsPerThread(n) (((n) + (WARP_SIZE - 1))/WARP_SIZE)

inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stdout, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}


#endif /* UTILS_H_ */
