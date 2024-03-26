#include "sched.cpp"
#include "test_util.h"
#include "gtest/gtest.h"

// 각 TEST_P는 SchedulerTest::SetUp() 이후 실행됨
// 각 TEST_P 실행 이후, SchedulerTest::TearDow()이 실행됨
TEST_P(SchedulerTest, FCFS) {
  // 부모 Scheduler 클래스 포인터로 자식 FCFS 클래스를 가리킴
  sched_ = new FCFS(job_queue_, switch_time_);
}

TEST_P(SchedulerTest, SPN) {
  sched_ = new SPN(job_queue_, switch_time_);
}

TEST_P(SchedulerTest, RR_1) {
  sched_ = new RR(job_queue_, switch_time_, /*time slice*/ 1);
}

TEST_P(SchedulerTest, RR_4) {
  sched_ = new RR(job_queue_, switch_time_, /*time slice*/ 4);
}

TEST_P(SchedulerTest, SRT) {
  sched_ = new SRT(job_queue_, switch_time_);
}

TEST_P(SchedulerTest, HRRN) {
  sched_ = new HRRN(job_queue_, switch_time_);
}

TEST_P(SchedulerTest, FeedBack_1) {
  sched_ = new FeedBack(job_queue_, switch_time_, /*is_2i*/ false);
}

TEST_P(SchedulerTest, FeedBack_2i) {
  sched_ = new FeedBack(job_queue_, switch_time_, /*is_2i*/ true);
}

// 각 8가지 TEST_P(스케줄링 기법)는 각각 4가지 경우(workload 2가지 x switch_time 2가지)를 TEST 함 
INSTANTIATE_TEST_CASE_P(Default, SchedulerTest,
  ::testing::Values(
    // std::make_tuple("워크로드 파일", "context_switch 시간"),
    std::make_tuple("A", 0.01),
    std::make_tuple("A", 0.1),
    
    std::make_tuple("B", 0.05),
    std::make_tuple("B", 0.2)
  )
);

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
