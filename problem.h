#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <algorithm>

#include "individual.h"

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

class FuncFitness
{
private:
    VecProb prob;
    VecDev dev;
public:
    FuncFitness(const VecProb &aProb, const VecDev &aDev);
    double operator ()(Individual &aInd);
};

FuncFitness::FuncFitness(const VecProb &aProb, const VecDev &aDev): 
    prob(aProb), 
    dev(aDev)
{
    //empty
}

double FuncFitness::operator ()(Individual &aInd)
{
    std::vector<double> devTime(dev.size());
    for (unsigned int i = 0; i < aInd.size(); ++i)
        devTime[aInd[i]] += dev[aInd[i]].probFactor[prob[i].type-1] * prob[i].apTime;
    return *std::max_element(devTime.begin(), devTime.end());
}

typedef std::vector<unsigned int> VecGen;

class FuncMut
{
private:
    VecGen vecGen;
public:
    FuncMut(const VecGen &aVecGen): vecGen(aVecGen) {}
    void operator ()(Individual &aInd)
    {
        unsigned int index = rand() % aInd.size();
        aInd[index] = vecGen[index];
    }
};

void initVecGen(const VecProb &aProb, const VecDev &aDev, VecGen &aVecGen)
{
    aVecGen.resize(aProb.size());
    for (unsigned int i = 0; i < aVecGen.size(); ++i)
    {
        double minF = DBL_MAX;
        for (unsigned int j = 0; j < aDev.size(); ++j)
        {
            double tmp = aDev[j].probFactor[aProb[i].type-1] * aProb[i].apTime;
            if (minF > tmp)
            {
                aVecGen[i] = j;
                minF = tmp;
            }
        }
    }
}

#endif