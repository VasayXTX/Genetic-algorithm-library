#ifndef GEN_ALG_H
#define GEN_ALG_H

#include <vector>
#include <utility>
#include <list>

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

    void insert(unsigned int aIndex, T &aInd);
    void insert(unsigned int aIndex, const typename Population<T, F>::Elem &aEl);

    void print() const;
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
void Population<T, F>::insert(unsigned int aIndex, T &aInd)
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

template<typename T, typename F>
void Population<T, F>::print() const
{
    for (unsigned int i = 0; i < populSize; ++i)
    {
        std::cout << popul[i]->first << '\t';
    }
    std::cout << "\n\n";
}

/*****************************************************************/
/*************************** Selection ***************************/
/*****************************************************************/

template<typename T, typename F>
class Selection
{
public:
    virtual void Select(Population<T, F> &aParentPool, Population<T, F> &aPopul, unsigned int aMixIterCount) = 0;
};

/*****************************************************************/

template<typename T, typename F>
class SelTournament: public Selection<T, F>
{
private:
    unsigned int grpSize;
public:
    SelTournament(unsigned int aGrpSize);
    void Select(Population<T, F> &aParentPool, Population<T, F> &aPopul, unsigned int aMixIterCount);
};

template<typename T, typename F>
SelTournament<T, F>::SelTournament(unsigned int aGrpSize):
    grpSize(aGrpSize)
{
    //empty
}

template<typename T, typename F>
void SelTournament<T, F>::Select(Population<T, F> &aParentPool, Population<T, F> &aPopul, unsigned int aMixIterCount)
{
    aParentPool.clear();
    aPopul.mix(aMixIterCount);
    Population<T, F>::Elem *el = aPopul.getByIndex(0);
    unsigned int counter = 1;   //Counter for group
    unsigned int iParP = 0;     //Index for parent population
    unsigned int iP = 1;        //Index for population
    while (1)
    {
        if (counter == grpSize)
        {
            aParentPool.insert(iParP++, *el);
            counter = 0;
        }
        if (iP >= aPopul.getPopulSize()) break;
        Population<T, F>::Elem *tmpEl = aPopul.getByIndex(iP++);
        if (el->first > tmpEl->first) el = tmpEl;
        ++counter;
    }
}

/****************************************************************/
/*************************** Mutation ***************************/
/****************************************************************/

template<typename T>
class Mutation
{
protected:
    static const float MUT_PROB;
    static const unsigned int GEN_COUNT = 1;
    unsigned int genCount;                      //Count gens for mutation

    Mutation(unsigned int aGenCount);
    virtual void specMutate(T &aInd) = 0; 
public:
    void mutate(T &aInd, float aMutProb = MUT_PROB); 
};

template<typename T>
const float Mutation<T>::MUT_PROB = 0.1f;

template<typename T>
Mutation<T>::Mutation(unsigned int aGenCount):
    genCount(aGenCount)
{
    //Empty
}

template<typename T>
void Mutation<T>::mutate(T &aInd, float aMutProb = MUT_PROB)
{
    if (((rand() % 10) / 10) > aMutProb) return;
    specMutate(aInd);
}

/********************************************************************/

template<typename T>
class MutRand: public Mutation<T>
{
protected:
    virtual void specMutate(T &aInd);
public:
    MutRand(unsigned int aGenCount = GEN_COUNT);
};

template<typename T>
MutRand<T>::MutRand(unsigned int aGenCount):
    Mutation(aGenCount)
{
    //Empty
}

template<typename T>
void MutRand<T>::specMutate(T &aInd)
{
    aInd.swap(rand() % aInd.size(), rand() % aInd.size());
}

/********************************************************************/

template<typename T, typename M>
class MutUser: public Mutation<T>
{
private:
    M mutFunc;
protected:
    virtual void specMutate(T &aInd);
public:
    MutUser(const M &aMutFunc, unsigned int aGenCount = GEN_COUNT);
};

template<typename T, typename M>
MutUser<T, M>::MutUser(const M &aMutFunc, unsigned int aGenCount):
    Mutation(aGenCount),
    mutFunc(aMutFunc)
{
    //Empty
}

template<typename T, typename M>
void MutUser<T, M>::specMutate(T &aInd)
{
    mutFunc(aInd);
}


/********************************************************************/
/*************************** Crossingover ***************************/
/********************************************************************/

template<typename T, typename F>
class Crossingover
{
protected:
    static const float CROSS_BEST_PROB;
    float crossBestProb;

    Crossingover(float aCrossBestProb);
public:
    virtual void cross(Population<T, F> &aPopul, Population<T, F> &aParentPool, Mutation<T> *aMut = 0) = 0;
};

template<typename T, typename F>
const float Crossingover<T, F>::CROSS_BEST_PROB = 0.3f;

template<typename T, typename F>
Crossingover<T, F>::Crossingover(float aCrossBestProb):
    crossBestProb(aCrossBestProb)
{
    //Empty
}

///********************************************************************/
//
//template<typename F>
//class CrossEven: public Crossingover<F>
//{
//public:
//    void cross(Population<F> &aPopul, Population<F> &aParentPool, Mutation<F> *aMut = 0);
//};
//
//template<typename F>
//void CrossEven<F>::cross(Population<F> &aPopul, Population<F> &aParentPool, Mutation<F> *aMut)
//{
//    aPopul.clear();
//    for (unsigned int i = 0; i < aPopul.getPopulSize(); i += 2)
//    {
//        unsigned int index1 = rand() % aParentPool.getPopulSize();
//        unsigned int index2 = rand() % aParentPool.getPopulSize();
//        Individual ind1(aPopul.getIndSize());
//        Individual ind2(aPopul.getIndSize());
//        for (unsigned int j = 0; j < ind1.size(); ++j)
//        {
//            if (rand() % 2)
//            {
//                ind1[j] = aParentPool.getByIndex(index1).second[j];
//                ind2[j] = aParentPool.getByIndex(index2).second[j];
//            }
//            else
//            {
//                ind2[j] = aParentPool.getByIndex(index1).second[j];
//                ind1[j] = aParentPool.getByIndex(index2).second[j];
//            }
//        }
//        if (aMut)
//        {
//            aMut->mutate(ind1, aPopul.getCodeSize());
//            aMut->mutate(ind2, aPopul.getCodeSize());
//        }
//        F f = aPopul.getFuncFitness();
//        aPopul.insertPtr(i, new PairIndividual(f(ind1), ind1));
//        aPopul.insertPtr(i + 1, new PairIndividual(f(ind2), ind2));
//    }
//}
//
/********************************************************************/

template<typename T, typename F>
class CrossPoint: public Crossingover<T, F>
{
private:
    unsigned int pCount;
public:
    CrossPoint(unsigned int aPointCount, float aCrossBestProb = CROSS_BEST_PROB);
    void cross(Population<T, F> &aPopul, Population<T, F> &aParentPool, Mutation<T> *aMut = 0);
};

template<typename T, typename F>
CrossPoint<T, F>::CrossPoint(unsigned int aPointCount, float aCrossBestProb):
    Crossingover(aCrossBestProb),
    pCount(aPointCount)
{
    //empty
}

template<typename T, typename F>
void CrossPoint<T, F>::cross(Population<T, F> &aPopul, Population<T, F> &aParentPool, Mutation<T> *aMut)
{
    typedef Population<T, F>::Elem PElem;
    aPopul.clear();

    std::list<unsigned int> points;
    points.push_back(0);
    for (unsigned int i = 1; i < pCount + 1; ++i)
        points.push_back(rand() % aPopul.getIndSize());
    points.push_back(aPopul.getIndSize());
    points.sort();

    PElem *elBest = aParentPool.findBest();
    
    for (unsigned int i = 0; i < aPopul.getPopulSize(); i += 2)
    {
        PElem *elPopul1 = ((i % 10) / 10) > crossBestProb ? 
                aParentPool.getByIndex(rand() % aParentPool.getPopulSize()) : elBest;
        PElem *elPopul2 = aParentPool.getByIndex(rand() % aParentPool.getPopulSize());
        T ind1(aPopul.getIndSize(), aPopul.getCodeSize());
        T ind2(aPopul.getIndSize(), aPopul.getCodeSize());
        std::list<unsigned int>::const_iterator it = points.begin();
        while (1)
        {
            std::list<unsigned int>::const_iterator itBegin = it++;
            if (it == points.end()) break;
            bool b = rand() % 2 != 0;
            for (unsigned k = *itBegin; k < *it; ++k)
            {
                if (b = !b)
                {
                    ind1[k] = elPopul1->second[k];
                    ind2[k] = elPopul2->second[k];
                }
                else
                {
                    ind1[k] = elPopul2->second[k];
                    ind2[k] = elPopul1->second[k];
                }
            }
        }
        if (aMut)
        {
            aMut->mutate(ind1);
            aMut->mutate(ind2);
        }
        aPopul.insert(i, ind1);
        aPopul.insert(i + 1, ind2);
    }
}

#endif