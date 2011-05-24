#include "individual.h"

Individual::Individual()
{
    //Empty
}

Individual::Individual(unsigned int aSize, unsigned int aCodeSize):
    cSize(aCodeSize)
{
    ind.resize(aSize);
}

Individual::Individual(const Individual &aInd):
    ind(aInd.ind),
    cSize(aInd.cSize)
{
    //Empty
}

unsigned int &Individual::operator [](unsigned int aIndex)
{
    return ind[aIndex];
}

Individual &Individual::operator =(const Individual &aInd)
{
    ind = aInd.ind;
    return *this;
}

void Individual::setSize(unsigned int aSize)
{
    ind.resize(aSize);
}

void Individual::setCodeSize(unsigned int aCodeSize)
{
    cSize = aCodeSize;
}

unsigned int Individual::size() const
{
    return ind.size();
}

unsigned int Individual::codeSize() const
{
    return cSize;
}