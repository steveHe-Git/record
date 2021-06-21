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

        return next_prime!=-1?next_prime:OnTheFly_impl->GetNextPrime(p);
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

protected:
    void SetUp() override {
        bool force_on_the_fly;
        int max_precalculated;
        std::tie(force_on_the_fly, max_precalculated) = GetParam();
        table = new HybridPrimeTable(force_on_the_fly, max_precalculated);
    }

    void TearDown() override {
        delete table;
        table = nullptr;
    }

    HybridPrimeTable* table;
};


TEST_P(PrimeTablesTest, ReturnsFalseForNonPrimes){
    EXPECT_FALSE(this->table->IsPrime(-5));
    EXPECT_FALSE(this->table->IsPrime(0));
    EXPECT_FALSE(this->table->IsPrime(1));
    EXPECT_FALSE(this->table->IsPrime(4));
    EXPECT_FALSE(this->table->IsPrime(6));
    EXPECT_FALSE(this->table->IsPrime(100));
}


TEST_P(PrimeTablesTest, ReturnTrueforPrime) {
    EXPECT_TRUE(this->table->IsPrime(2));
    EXPECT_TRUE(this->table->IsPrime(3));
    EXPECT_TRUE(this->table->IsPrime(5));
    EXPECT_TRUE(this->table->IsPrime(7));
    EXPECT_TRUE(this->table->IsPrime(11));
    EXPECT_TRUE(this->table->IsPrime(13));
    EXPECT_TRUE(this->table->IsPrime(131));
}

TEST_P(PrimeTablesTest, CanGetNextPrime) {
    EXPECT_EQ(2, this->table->GetNextPrime(0));
    EXPECT_EQ(3, this->table->GetNextPrime(2));
    EXPECT_EQ(5, this->table->GetNextPrime(3));
    EXPECT_EQ(7, this->table->GetNextPrime(5));
    EXPECT_EQ(131, this->table->GetNextPrime(128));
}

INSTANTIATE_TEST_SUITE_P(MeaningfulTestParameters, PrimeTablesTest, 
    Combine(Bool(), Values(1, 10)));


}//namespace
