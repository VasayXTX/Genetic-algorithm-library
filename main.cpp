#include <stdio.h>
#include <time.h>
#include <iostream>
#include <limits>

#include "problem.h"
#include "genAlg.h"

const bool IS_COMMA = true;
const unsigned int POPULATION_SIZE = 1000;
const unsigned int COUNT_ITERATION = 1000;
const unsigned int SEL_GROUP_SIZE = 2;

int main()
{
    unsigned int m;				//Problems count
    unsigned int n;				//Developers count
    std::vector<Prob> prob;
    std::vector<Dev> dev;

	FILE *fIn = freopen("input.txt", "r", stdin);
	FILE *fOut = freopen("output.txt", "w", stdout);
	if (!fIn) return 1;

	std::cin >> m;

    prob.resize(m);
    char ch;
    short v;
    for (unsigned int i = 0; i < m - 1; ++i)
    {
        std::cin >> v;
        prob[i].type = (ProbType)v;
        if (IS_COMMA) std::cin >> ch;
    }
    std::cin >> v;
    prob[m-1].type = (ProbType)v;
    for (unsigned int i = 0; i < m - 1; ++i)
    {
        std::cin >> prob[i].apTime;
        if (IS_COMMA) std::cin >> ch;
    }
    std::cin >> prob[m-1].apTime;

    std::cin >> n;
    dev.resize(n);
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < PROB_TYPE_COUNT - 1; ++j)
        {
            std::cin >> dev[i].probFactor[j];
            if (IS_COMMA) std::cin >> ch;
        }
        std::cin >> dev[i].probFactor[PROB_TYPE_COUNT-1];
    }

    //srand(time(0));
    time_t tBegin = time(0);

    VecGen vecGen;
    initVecGen(prob, dev, vecGen);

    FuncFitness fFit(prob, dev);

    Population<Individual, FuncFitness> popul(POPULATION_SIZE, m, n, fFit);
    popul.generate();

    SelTournament<Individual, FuncFitness> sel(SEL_GROUP_SIZE);
    Population<Individual, FuncFitness> parentPool(POPULATION_SIZE / SEL_GROUP_SIZE, m, n, fFit);
    CrossPoint<Individual, FuncFitness> cro1(4);

    FuncMut funcMut(vecGen);
    MutUser<Individual, FuncMut> mutMy(funcMut, 10);
    MutRand<Individual> mutRand;

    for (unsigned int i = 0; i < COUNT_ITERATION; ++i)
    {
        sel.Select(parentPool, popul, m / 10);
        if (rand() % 2)
            cro1.cross(popul, parentPool, &mutMy);
        else
            cro1.cross(popul, parentPool, &mutRand);
        popul.print();
    }
    Population<Individual, FuncFitness>::Elem *res = popul.findBest();
    std::cout << res->first << '\n';
    std::cout << time(0) - tBegin;
	fclose(fIn);
	fclose(fOut);

    return 0;
}