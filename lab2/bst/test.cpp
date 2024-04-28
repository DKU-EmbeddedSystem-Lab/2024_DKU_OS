#include "bst.h"
#include "bst_impl.h"
#include "test_util.h"
#include "gtest/gtest.h"

/// @brief Test BST without lock in single-thread
TEST_P(BstSingleThreadTest, Single) {
  tree_ = new BST();
}

/// @brief Test BST with coarse-grained lock in single-thread
TEST_P(BstSingleThreadTest, Coarse) {
  tree_ = new CoarseBST();
}

/// @brief Test BST with fine-grained lock in single-thread
TEST_P(BstSingleThreadTest, Fine) {  
  tree_ = new FineBST();
}

/// @brief BstSingleThreadTest Test Case and Parameters
INSTANTIATE_TEST_CASE_P(Default, BstSingleThreadTest,
  ::testing::Values(
    // Workload Type, Request Num, Num of Threads
    std::make_tuple(INSERT_ONLY, 1000000, 1),
    std::make_tuple(INSERT_LOOKUP, 1000000, 1),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 1)
  )
);

/// @brief Test BST with coarse-grained lock in multi-thread
TEST_P(BstMultiThreadTest, Coarse) {
  // Test BST with coarse-grained lock
  tree_ = new CoarseBST();
}

/// @brief Test BST with fine-grained lock in multi-thread
TEST_P(BstMultiThreadTest, Fine) {
  tree_ = new FineBST();
}

/// @brief BstMultiThreadTest Test Case and Parameters
INSTANTIATE_TEST_CASE_P(Default, BstMultiThreadTest,
  ::testing::Values(
    // Workload Type, Request Num, Num of Threads
    std::make_tuple(INSERT_ONLY, 1000000, 2),
    std::make_tuple(INSERT_ONLY, 1000000, 4),
    std::make_tuple(INSERT_ONLY, 1000000, 8),
    std::make_tuple(INSERT_LOOKUP, 1000000, 2),
    std::make_tuple(INSERT_LOOKUP, 1000000, 4),
    std::make_tuple(INSERT_LOOKUP, 1000000, 8),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 2),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 4),
    std::make_tuple(INSERT_LOOKUP_DELETE, 1000000, 8)
  )
);

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
