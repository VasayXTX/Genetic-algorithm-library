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
    typedef std::vector<Elem *> Popul;

private:
    unsigned int populSize, indSize, codeSize;
    F funcFitness;
    Popul popul;

public:
    Population(unsigned int aPopulSize, unsigned int aIndSize, unsigned int aCodeSize, const F &aFuncFitness);
    Population(const Population &aPopup);
    ~Population();

    void clear();

    Population &operator =(const Population &aPopul);

    unsigned int getPopulSize() const;
    unsigned int getIndSize() const;
    unsigned int getCodeSize() const;

    void mix(unsigned int aIterCount);

    void generate();

    Elem *findBest() const;
    Elem *getByIndex(unsigned int aIndex);

    void insert(unsigned int aIndex, const T &aInd);
    void insert(unsigned int aIndex, const typename Population<T, F>::Elem &aEl);

    //void print() const;
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
        popul[i] = 0;
}

template<typename T, typename F>
Population<T, F>::Population(const Population &aPopup)
{
    *this = aPopup;
}

template<typename T, typename F>
Population<T, F>::~Population()
{
    clear();
}

template<typename T, typename F>
void Population<T, F>::clear()
{
    for (Popul::iterator it = popul.begin(); it != popul.end(); ++it)
        delete *it;
}

template<typename T, typename F>
void Population<T, F>::generate()
{
    clear();
    for (Popul::iterator it = popul.begin(); it != popul.end(); ++it)
    {
        *it = new Elem();
        (*it)->second.setSize(indSize);
        (*it)->second.setCodeSize(codeSize);
        for (unsigned int i = 0; i < indSize; ++i)
        {
            (*it)->second[i] = rand() % codeSize;
        }
        (*it)->first = funcFitness((*it)->second);
    }
}

template<typename T, typename F>
typename Population<T, F> &Population<T, F>::operator =(const Population &aPopul)
{
    populSize = aPopul.populSize;
    indSize = aPopul.indSize;
    codeSize = aPopul.codeSize;
    funcFitness = aPopul.funcFitness;
    clear();
    popul.resize(aPopul.populSize);
    for (unsigned int i = 0; i < populSize; ++i)
        popul[i] = new Elem(*aPopul.popul[i]);
    return *this;
}

template<typename T, typename F>
void Population<T, F>::mix(unsigned int aIterCount)
{
    for (unsigned int i = 0; i < aIterCount; ++i)
    {
        unsigned int index1 = rand() % populSize;
        unsigned int index2 = rand() % populSize;
        Population<T, F>::Elem *p = popul[index1];
        popul[index1] = popul[index2];
        popul[index2] = p;
    }
}

template<typename T, typename F>
unsigned int Population<T, F>::getPopulSize() const
{
    return populSize;
}

template<typename T, typename F>
unsigned int Population<T, F>::getIndSize() const
{
    return indSize;
}

template<typename T, typename F>
unsigned int Population<T, F>::getCodeSize() const
{
    return codeSize;
}

template<typename T, typename F>
typename Population<T, F>::Elem *Population<T, F>::findBest() const
{
    Elem *res = popul[0];
    for (unsigned int i = 1; i < populSize; ++i)
    {
        if (res->first > popul[i]->first)
            res = popul[i];
    }
    return res;
}

template<typename T, typename F>
void Population<T, F>::insert(unsigned int aIndex, const T &aInd)
{
    insert(aIndex, Elem(funcFitness(aInd), aInd));
}

template<typename T, typename F>
void Population<T, F>::insert(unsigned int aIndex, const typename Population<T, F>::Elem &aEl)
{
    popul[aIndex] = new Elem(aEl);
}

template<typename T, typename F>
typename Population<T, F>::Elem *Population<T, F>::getByIndex(unsigned int aIndex)
{
    return popul[aIndex];
}

#endif