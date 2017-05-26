#include "ChunkPOC.cuh"

template <unsigned int N>
float testChunksMultiply(unsigned int size)
{
	unsigned int * pentanomialCoefficients = irreducibles[irreducibleIndex(SIZE)];
	setPentanomial(pentanomialCoefficients);
	unsigned int (*chunksAHost)[ROUNDED(N)] = new unsigned int[size][ROUNDED(N)];
	unsigned int (*chunksBHost)[ROUNDED(N)] = new unsigned int[size][ROUNDED(N)];

	for(unsigned int i = 0 ; i < size ; ++i)
	{
		for(unsigned int j = 0 ; j < ROUNDED(N) ; ++j)
		{
			chunksAHost[i][j]=0;
			chunksBHost[i][j]=0;
		}
		chunksAHost[i][N/2]=2;
		chunksBHost[i][N/2]=2;
	}

	unsigned int (*chunksA)[ROUNDED(N)];
	unsigned int (*chunksB)[ROUNDED(N)];

	cudaMalloc(&chunksA, sizeof(unsigned int)*ROUNDED(N)*size);
	cudaMalloc(&chunksB, sizeof(unsigned int)*ROUNDED(N)*size);

	cudaMemcpy(chunksA, chunksAHost, sizeof(unsigned int)*ROUNDED(N)*size, cudaMemcpyHostToDevice);
	cudaMemcpy(chunksB, chunksBHost, sizeof(unsigned int)*ROUNDED(N)*size, cudaMemcpyHostToDevice);

	unsigned int blocksNum = (size*GROUP_SIZE(N)+THREAD_BLOCK_SIZE(N)-1)/THREAD_BLOCK_SIZE(N);
	cudaEvent_t start, end;
	cudaEventCreate(&start);
	cudaEventCreate(&end);

	cudaEventRecord(start);
	finiteFieldMultiplyArrays<N><<<
			blocksNum,
			THREAD_BLOCK_SIZE(N)>>>
			((unsigned int (*)[ROUNDED(N)])chunksA, reinterpret_cast<unsigned int (*)[ROUNDED(N)]>(chunksB), (unsigned int (*)[ROUNDED(N)])chunksA, size);
	cudaEventRecord(end);
	gpuErrchk(cudaPeekAtLastError());

	cudaMemcpy(chunksAHost, chunksA, sizeof(unsigned int)*N*size, cudaMemcpyDeviceToHost);


	float ms;
	cudaEventElapsedTime(&ms, start, end);
	delete[] chunksAHost;
	delete[] chunksBHost;

	cudaFree(chunksA);
	cudaFree(chunksB);
	return ms;
}
