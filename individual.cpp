#include "individual.h"

Individual::Individual(const Individual &aInd):
    ind(aInd.ind)
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