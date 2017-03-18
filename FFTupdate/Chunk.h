/*
 * Chunk.h
 *
 *  Created on: Mar 18, 2017
 *      Author: matt
 */

#ifndef CHUNK_H_
#define CHUNK_H_

template<unsigned int N>
class Chunk {
private:
	unsigned int c[N];
public:
	/**
	 * Initializes the chunk using the elements from array pointed by e of length len.
	 * If len > 32, using the first 32 elements.
	 * If len <= 32, inserting the elements to the chunk in a round robin fashion.
	 */
	Chunk(const std::shared_ptr<F2XE<N>>& e, unsigned int len);
	~Chunk();
};

template<unsigned int N>
Chunk<N>::Chunk(const std::shared_ptr<F2XE<N>>& e, unsigned int len)
{
	unsigned int inserted = 0;
	unsigned int i = 0;
	while(inserted < 32)
	{
		F2X p = e.get()[i].toStdRepr();
		for(unsigned int j = 0 ; j < N ; ++j)
		{
			if(p.getCoefficient(j).val())
			{
				this->c[j] |= (1<<(inserted));
			}
		}
		i = (i+1) % len;
		++inserted;
	}
}

#endif /* CHUNK_H_ */
