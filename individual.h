#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>

/*************************** Individual ***************************/

class Individual
{
private:
    std::vector<unsigned int> ind;
    unsigned int cSize;
public:
    Individual(unsigned int aSize, unsigned int aCodeSize);
    Individual(const Individual &aInd);
    
    void resize(unsigned int aSize);
    unsigned int size() const;
    unsigned int codeSize() const;
    unsigned int operator [](unsigned int aIndex);
};

#endif