#include "sample_prime_tables.h"
#include "gtest/gtest.h"

namespace {



using testing::Values;
using testing::TestWithParam;

typedef PrimeTable* CreatPrimeTableFunc();

PrimeTable* CreatOnTheFlyPrimeTable(){
    return new OnTheFlyPrimeTable();
}

template<size_t precalculated>
PrimeTable* CreatPreCalculatedPrimeTable() {
    return new PreCalculatedPrimeTable(precalculated);
}

class PrimeTablesTestSample7:public TestWithParam<CreatPrimeTableFunc*>{
public:
    ~PrimeTablesTestSample7() override {
        if (table) {
            delete table;
        }
    }
    void SetUp() override {table = (*GetParam())();}
    void TearDown() override {
        delete table;
        table = nullptr;
    }
protected:
    PrimeTable* table;
};

TEST_P(PrimeTablesTestSample7, ReturnsFalseForNonPrimes){
    EXPECT_FALSE(this->table->IsPrime(-5));
    EXPECT_FALSE(this->table->IsPrime(0));
    EXPECT_FALSE(this->table->IsPrime(1));
    EXPECT_FALSE(this->table->IsPrime(4));
    EXPECT_FALSE(this->table->IsPrime(6));
    EXPECT_FALSE(this->table->IsPrime(100));
}

TEST_P(PrimeTablesTestSample7, ReturnTrueforPrime) {
    EXPECT_TRUE(this->table->IsPrime(2));
    EXPECT_TRUE(this->table->IsPrime(3));
    EXPECT_TRUE(this->table->IsPrime(5));
    EXPECT_TRUE(this->table->IsPrime(7));
    EXPECT_TRUE(this->table->IsPrime(11));
    EXPECT_TRUE(this->table->IsPrime(13));
    EXPECT_TRUE(this->table->IsPrime(131));
}

TEST_P(PrimeTablesTestSample7, CanGetNextPrime) {
    EXPECT_EQ(2, this->table->GetNextPrime(0));
    EXPECT_EQ(3, this->table->GetNextPrime(2));
    EXPECT_EQ(5, this->table->GetNextPrime(3));
    EXPECT_EQ(7, this->table->GetNextPrime(5));
    EXPECT_EQ(131, this->table->GetNextPrime(128));
}

INSTANTIATE_TEST_SUITE_P(
    OnTheFlyAndPreCalculated,
    PrimeTablesTestSample7,
    Values(
        &CreatOnTheFlyPrimeTable,
        &CreatPreCalculatedPrimeTable<1000>
    ));





}//namespace