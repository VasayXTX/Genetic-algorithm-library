#include "individual.h"

Individual::Individual(unsigned int aSize, unsigned int aCodeSize):
    cSize(aCodeSize)
{
    ind.resize(aSize);
}

Individual::Individual(const Individual &aInd):
    ind(aInd.ind),
    cSize(aInd.cSize)
{
    //empty
}

unsigned int Individual::operator [](unsigned int aIndex)
{
    return ind[aIndex];
}

void Individual::resize(unsigned int aSize)
{
    ind.resize(aSize);
}

unsigned int Individual::size() const
{
    return ind.size();
}

unsigned int Individual::codeSize() const
{
    return cSize;
}