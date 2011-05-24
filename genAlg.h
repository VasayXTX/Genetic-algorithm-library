#ifndef GEN_ALG_H
#define GEN_ALG_H

#include <vector>
#include <utility>

/******************************************************************/
/*************************** Population ***************************/
/******************************************************************/

template<typename T, typename F>
class Population
{
public:
    typedef std::pair<double, T> Elem;   //Element in population
    typedef std::vector<Elem> Popul;

private:
    unsigned int populSize, indSize, codeSize;
    F funcFitness;
    Popul popul;

public:
    Population(unsigned int aPopulSize, unsigned int aIndSize, unsigned int aCodeSize, const F &aFuncFitness);
    Population(const Population &aPopup);
    /*~Population();

    void clear();*/
    void generate();

    /*void insertPtr(unsigned int aIndex, PairIndividual *aPInd);
    void insert(unsigned int aIndex, const PairIndividual &aPInd);
    void replace(unsigned int aIndex, const PairIndividual &aPInd);

    void mix(unsigned int aIterCount);
    PairIndividual &getByIndex(unsigned int aIndex) const;

    unsigned int getPopulSize() const;
    unsigned int getIndSize() const;
    unsigned int getCodeSize() const;
    const F &getFuncFitness() const;

    Population &operator =(const Population &aPopul);

    PairIndividual *findRes() const;

    friend std::ostream &operator <<(std::ostream &aOut, const Population<F> &aPopul);

    void print() const;*/
};

template<typename T, typename F>
Population<T, F>::Population(unsigned int aPopulSize, unsigned int aIndSize, unsigned int aCodeSize, const F &aFuncFitness):
    populSize(aPopulSize),
    indSize(aIndSize),
    codeSize(aCodeSize),
    funcFitness(aFuncFitness)
{
    popul.resize(populSize);
    for (unsigned int i = 0; i < populSize; ++i)
    {
        popul[i].second.setSize(aIndSize);
        popul[i].second.setCodeSize(aCodeSize);
    }
}

template<typename T, typename F>
Population<T, F>::Population(const Population &aPopup)
{
    *this = aPopup;
}

template<typename T, typename F>
void Population<T, F>::generate()
{
    for (Popul::iterator it = popul.begin(); it != popul.end(); ++it)
    {
        for (unsigned int i = 0; i < indSize; ++i)
            (*it).second[i] = rand() % codeSize;
        (*it).first = funcFitness((*it).second);
    }
}

#endif