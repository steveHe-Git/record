#include "sample_prime_tables.h"
#include "gtest/gtest.h"

namespace{


class HybridPrimeTable:public PrimeTable
{
private:
    /* data */
    OnTheFlyPrimeTable* OnTheFly_impl;
    PreCalculatedPrimeTable *precalc_impl;
    int maxPrecalculated;
public:
    HybridPrimeTable(bool only_on_the_fly, int max_precalculated_){
        OnTheFly_impl = new OnTheFlyPrimeTable();
        precalc_impl = only_on_the_fly?nullptr:new PreCalculatedPrimeTable(max_precalculated_);
        maxPrecalculated = max_precalculated_;
    }

    ~HybridPrimeTable(){
        delete OnTheFly_impl;
        delete precalc_impl;
    };

    bool IsPrime(int n)const override{
        if (precalc_impl != nullptr && n < maxPrecalculated) {
            return precalc_impl->IsPrime(n);
        } else {
            return OnTheFly_impl->IsPrime(n);
        }
    }

    int GetNextPrime(int p) const override {
        int next_prime = -1;
        if (precalc_impl != nullptr && p < maxPrecalculated) {
            next_prime = precalc_impl->GetNextPrime(p);
        }

        return next_prime==-1?next_prime:OnTheFly_impl->GetNextPrime(p);
    }
};

using testing::TestWithParam;
using testing::Bool;
using testing::Combine;
using testing::Values;

class PrimeTablesTest:public TestWithParam<::std::tuple<bool,int>>
{
private:
    /* data */
public:
    PrimeTablesTest(/* args */);
    ~PrimeTablesTest();
protected:
    
};






}//namespace
