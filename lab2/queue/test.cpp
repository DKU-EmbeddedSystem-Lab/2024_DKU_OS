#include "queue.h"
#include "queue_impl.h"
#include "test_util.h"
#include "gtest/gtest.h"
#include <map>

/// @brief Test Queue without Lock in single-thread
TEST_P(QueueSingleThreadTest, Single) {
  queue_ = new Queue();
}

/// @brief Test Queue with coarse-grained lock in single-thread
TEST_P(QueueSingleThreadTest, Coarse) {
  queue_ = new CoarseQueue();
}

// /// @brief  Test Queue with fine-grained lock in single-thread
// TEST_P(QueueSingleThreadTest, Fine) {  
//   queue_ = new FineQueue();
// }

/// @brief  QueueSingleThreadTest Test Case and Parameters
INSTANTIATE_TEST_CASE_P(Default, QueueSingleThreadTest,
  ::testing::Values(
    // Workload Type, Request Num, Num of Threads
    std::make_tuple(ENQ_THEN_DEQ, 500000, 1),
    std::make_tuple(ENQUEUE_DEQUEUE, 1000000, 1)
  )
);

/// @brief  Test Queue with coarse-grained lock in multi-thread
TEST_P(QueueMultiThreadTest, Coarse) {
  queue_ = new CoarseQueue();
}

// /// @brief  Test Queue with fine-grained lock in multi-thread
// TEST_P(QueueMultiThreadTest, Fine) {
//   queue_ = new FineQueue();
// }

/// @brief  QueueMultiThreadTest Test Case and Parameters
INSTANTIATE_TEST_CASE_P(Default, QueueMultiThreadTest,
  ::testing::Values(
    // Workload Type, Request Num, Num of Threads
    std::make_tuple(ENQ_THEN_DEQ, 500000, 2),
    std::make_tuple(ENQ_THEN_DEQ, 500000, 4),
    std::make_tuple(ENQ_THEN_DEQ, 500000, 8),
    std::make_tuple(ENQUEUE_DEQUEUE, 1000000, 2),
    std::make_tuple(ENQUEUE_DEQUEUE, 1000000, 4),
    std::make_tuple(ENQUEUE_DEQUEUE, 1000000, 8)
  )
);

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
