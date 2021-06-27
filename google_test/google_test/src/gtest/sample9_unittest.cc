#include <stdio.h>
#include "gtest/gtest.h"

using ::testing::EmptyTestEventListener;
using ::testing::TestEventListeners;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::UnitTest;
using ::testing::TestInfo;
using ::testing::TestPartResult;

namespace {

class TersePrinter : public EmptyTestEventListener
{
private:
    /* data */
    //在所有的测试案例执行前调用
    void OnTestProgramStart(const UnitTest&) override {}
    //在所有的测试案例结束后调用
    void OnTestProgramEnd(const UnitTest& unit_test) override {
        fprintf(stdout, "Test %s\n", unit_test.Passed() ? "PASSED" : "FAILED");
        fflush(stdout);
    }

    //在任意一个测试案例开始时调用
    void OnTestStart(const TestInfo& test_info) override {
        fprintf(stdout, "*** Test %s.%s starting\n", 
            test_info.test_case_name(),
            test_info.name());
        fflush(stdout);
    }

    //在失败断言和SUCCEED() 中会被调用
    void OnTestPartResult(const TestPartResult& test_part_result) override {
        fprintf(stdout, "%s in %s:%d\n%s\n",
            test_part_result.failed() ? "***Failure" : "Success",
            test_part_result.file_name(),
            test_part_result.line_number(),
            test_part_result.summary());
        fflush(stdout);
    }

    //在任意一个测试案例开始时调用
    void OnTestEnd(const TestInfo& test_info) override {
        fprintf(stdout, "*** Test %s.%s ending\n", 
            test_info.test_case_name(),
            test_info.name());
        fflush(stdout);
    }

};

TEST(CustomOutputTest, PrintMsg) {
    printf("Printing something from the test body...\n");
}

TEST(CustomOutputTest, Succeeds) {
    SUCCEED() << "SUCCEED() has been invoked from here";
}

TEST(CustomOutputTest, Fails) {
    EXPECT_EQ(1, 2) << "This test fails in order to demonstrate alternative failure messages";
}

}//namespace


int main(int argc, char** argv) {
    InitGoogleTest(&argc, argv);

    printf("### statting main()\n");

    bool terse_output = false;
    if (argc > 1 && strcmp(argv[1], "--terse_output") == 0) {
        terse_output = true;
    } else {
        printf("%s\n", "Run this program with --terse_output to change the way"
            "it prints its output.");
    }

    UnitTest& unit_test = *UnitTest::GetInstance();

    if (terse_output) {
        TestEventListeners& listeners = unit_test.listeners();

        delete listeners.Release(listeners.default_result_printer());

        listeners.Append(new TersePrinter);
    }

    int ret_val = RUN_ALL_TESTS();

    //统计名字不为Fails中其他的所有的测试案例失败的测试
    int unexpectedly_failed_tests = 0;
    for (int i =0 ; i < unit_test.total_test_suite_count(); ++i) {
        const testing::TestSuite& test_suit = *unit_test.GetTestSuite(i);
        for (int j = 0; j < test_suit.total_test_count(); ++j) {
            const TestInfo& test_info = *test_suit.GetTestInfo(j);
            if (test_info.result()->Failed() &&
                strcmp(test_info.name(), "Fails") !=0 ) {
                ++unexpectedly_failed_tests;
            } 
        }
    }

    printf("fail count: %d\n", unexpectedly_failed_tests);

    return unexpectedly_failed_tests;
}