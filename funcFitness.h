#ifndef FUNC_FITNESS_H
#define FUNC_FITNESS_H

#include <vector>

enum ProbType
{
	pType1 = 1,
	pType2,
	pType3,
	pType4
};

const unsigned short PROB_TYPE_COUNT = pType4;

//Struct for Problem
struct Prob
{
	ProbType type;
	double apTime;
};

//Struct for Developer
struct Dev
{
	std::vector<double> probFactor;
	Dev() { probFactor.resize(PROB_TYPE_COUNT); }
};

typedef std::vector<Prob> VecProb;
typedef std::vector<Dev> VecDev;

template<typename T>
class FuncFitness
{
private:
    VecProb prob;
    VecDev dev;
public:
    FuncFitness(const VecProb &aProb, const VecDev &aDev);
    double operator ()(const T &aInd);
};


template<typename T>
FuncFitness<T>::FuncFitness(const VecProb &aProb, const VecDev &aDev): 
    prob(aProb), 
    dev(aDev)
{
    //empty
}

template<typename T>
double FuncFitness<T>::operator ()(const T &aInd)
{
    std::vector<double> devTime(dev.size());
    for (unsigned int i = 0; i < aInd.size(); ++i)
        devTime[aInd[i]] += dev[aInd[i]].probFactor[prob[i].type-1] * prob[i].apTime;
    return *std::max_element(devTime.begin(), devTime.end());
}

#endif