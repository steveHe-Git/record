#include "sample_prime_tables.h"
#include "gtest/gtest.h"

namespace{


template<typename T>
PrimeTable* CreatePrimeTable();

template<>
PrimeTable* CreatePrimeTable<OnTheFlyPrimeTable>(){
    return new OnTheFlyPrimeTable();
}

template<>
PrimeTable* CreatePrimeTable<PreCalculatedPrimeTable>(){
    return new PreCalculatedPrimeTable(10000);
}

template<typename T>
class PrimeTableTest: public testing::Test {
protected:
    PrimeTableTest():table(CreatePrimeTable<T>()){}
    ~PrimeTableTest(){delete table;}

    PrimeTable* const table;
};

using testing::Types;
typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable> Implementations;

TYPED_TEST_SUITE(PrimeTableTest, Implementations);

TYPED_TEST(PrimeTableTest, ReturnFalseForNotPrime) {
    EXPECT_FALSE(this->table->IsPrime(-5));
    EXPECT_FALSE(this->table->IsPrime(0));
    EXPECT_FALSE(this->table->IsPrime(1));
    EXPECT_FALSE(this->table->IsPrime(4));
    EXPECT_FALSE(this->table->IsPrime(6));
    EXPECT_FALSE(this->table->IsPrime(100));
}

TYPED_TEST(PrimeTableTest, ReturnTrueforPrime) {
    EXPECT_TRUE(this->table->IsPrime(2));
    EXPECT_TRUE(this->table->IsPrime(3));
    EXPECT_TRUE(this->table->IsPrime(5));
    EXPECT_TRUE(this->table->IsPrime(7));
    EXPECT_TRUE(this->table->IsPrime(11));
    EXPECT_TRUE(this->table->IsPrime(13));
    EXPECT_TRUE(this->table->IsPrime(131));
}

TYPED_TEST(PrimeTableTest, CanGetNextPrime) {
    EXPECT_EQ(2, this->table->GetNextPrime(0));
    EXPECT_EQ(3, this->table->GetNextPrime(2));
    EXPECT_EQ(5, this->table->GetNextPrime(3));
    EXPECT_EQ(7, this->table->GetNextPrime(5));
    EXPECT_EQ(131, this->table->GetNextPrime(128));
}

/*另外一种不需要预先指定类型，更加灵活*/
TYPED_TEST_SUITE_P(PrimeTableTest);
TYPED_TEST_P(PrimeTableTest, ReturnFalseForNotPrimeTest) {
    EXPECT_FALSE(this->table->IsPrime(-5));
    EXPECT_FALSE(this->table->IsPrime(0));
    EXPECT_FALSE(this->table->IsPrime(1));
    EXPECT_FALSE(this->table->IsPrime(4));
    EXPECT_FALSE(this->table->IsPrime(6));
    EXPECT_FALSE(this->table->IsPrime(100));
}

TYPED_TEST_P(PrimeTableTest, ReturnTrueforPrimeTest) {
    EXPECT_TRUE(this->table->IsPrime(2));
    EXPECT_TRUE(this->table->IsPrime(3));
    EXPECT_TRUE(this->table->IsPrime(5));
    EXPECT_TRUE(this->table->IsPrime(7));
    EXPECT_TRUE(this->table->IsPrime(11));
    EXPECT_TRUE(this->table->IsPrime(13));
    EXPECT_TRUE(this->table->IsPrime(131));
}

TYPED_TEST_P(PrimeTableTest, CanGetNextPrimeTest) {
    EXPECT_EQ(2, this->table->GetNextPrime(0));
    EXPECT_EQ(3, this->table->GetNextPrime(2));
    EXPECT_EQ(5, this->table->GetNextPrime(3));
    EXPECT_EQ(7, this->table->GetNextPrime(5));
    EXPECT_EQ(131, this->table->GetNextPrime(128));
}

REGISTER_TYPED_TEST_SUITE_P(
    PrimeTableTest,
    ReturnFalseForNotPrimeTest, ReturnTrueforPrimeTest, CanGetNextPrimeTest
);
typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable> PrimeTableImplementations;
INSTANTIATE_TYPED_TEST_SUITE_P(OnTheFlyAndPreCalculated,
                               PrimeTableTest,
                               PrimeTableImplementations
);


}//nameapce