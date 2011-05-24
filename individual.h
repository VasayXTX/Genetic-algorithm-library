#include <vector>

class Individual
{
private:
    std::vector<unsigned int> ind;
public:
    Individual(const Individual &aInd);
    
    void resize(unsigned int aSize);
    unsigned int size() const;
    unsigned int operator [](unsigned int aIndex);
};