#include "ListLink.h"
#include "gtest/gtest.h"

using namespace std;
using namespace testing;

//基本测试
TEST(ListLinkTest, InitListTest) {
    ListLink<int> list;
    EXPECT_NE(list.getSize(), 0);
    list.InitList();
    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.IsListEmpty());
}

TEST(ListLinkTest, InsertELemTest) {
    ListLink<int> list;
    list.InitList();
    list.InsertELem(1, 1);
    list.InsertELem(2, 2);
    list.InsertELem(3, 3);
    EXPECT_EQ(list.getSize(), 3);
    list.ClearList();
    EXPECT_TRUE(list.IsListEmpty());
}

TEST(ListLinkTest, DeleteElemTest) {
    ListLink<int> list;
    list.InitList();
    list.InsertELem(1, 1);
    list.InsertELem(2, 2);
    list.InsertELem(3, 3);
    int value = 0;
    list.DeleteElem(2, &value);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(value, 2);
}

TEST(ListLinkTest, GetElemTest) {
    ListLink<int> list;
    list.InitList();
    list.InsertELem(1, 1);
    list.InsertELem(2, 3);
    EXPECT_EQ(list.getSize(), 2);
    auto value =list.GetElem(2);
    EXPECT_EQ(value, 3);
}

TEST(ListLinkTest, LocateElemTest) {
    ListLink<int> list;
    list.InitList();
    list.InsertELem(1, 1);
    list.InsertELem(2, 3);
    EXPECT_EQ(list.getSize(), 2);
    auto index =list.LocateElem(3);
    EXPECT_EQ(index, 2);
}

//gtest夹具测试
struct TestListLink:testing::Test
{
    ListLink<int>* list;
    TestListLink() {
        //cout << "TestListLink construct" << endl;
        list = new ListLink<int>();
    }
    ~TestListLink() {
        //cout << "~TestListLink deconstruct" << endl;
        delete list;
    } 
};

TEST_F(TestListLink, InitTest) {
    EXPECT_NE(list->getSize(), 0);
    list->InitList();
    EXPECT_EQ(list->getSize(), 0);
    EXPECT_TRUE(list->IsListEmpty());
}

TEST_F(TestListLink, InsertTest) {
    list->InitList();
    list->InsertELem(1, 1);
    list->InsertELem(2, 2);
    list->InsertELem(3, 3);
    EXPECT_EQ(list->getSize(), 3);
    list->ClearList();
    EXPECT_TRUE(list->IsListEmpty());
}

//夹具类型的值参数化测试
struct Param
{
    Param(int first, int second){
        mapValue.insert(std::pair<int ,int>(first, second));
    }
    /* data */
    using map_int = std::map<int, int>;
    map_int mapValue;
};

struct WithParamTest : public TestListLink, testing::WithParamInterface<Param>
{
    /* data */
    WithParamTest() {
        list->InitList();
    }
};

TEST_P(WithParamTest, InsertTest) {
    EXPECT_TRUE(list->IsListEmpty());
    for(auto& map: GetParam().mapValue) {
        list->InsertELem(map.first, map.second);
    }
    EXPECT_FALSE(list->IsListEmpty());
}
INSTANTIATE_TEST_SUITE_P(Default, WithParamTest, testing::Values(Param(1,1), Param(1,2)));


int main (int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}