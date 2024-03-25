#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "sched.h"
#include "gtest/gtest.h"

class SchedulerTest : public ::testing::TestWithParam<std::tuple<std::string, double>> {
 protected:
  Scheduler* sched_;
  std::queue<Job> job_queue_;
  std::vector<Job> end_jobs_;
  std::vector<int> sched_log_;
  std::string workload_name_ = std::get<0>(GetParam());
  double switch_time_ = std::get<1>(GetParam());
 
 public:
  // TEST_P 실행 전, 실행 됨
  void SetUp() override;

  // TEST_P 실행 후, 실행 됨
  void TearDown() override;
  
  // 스케줄러 실행
  void run_sched (Scheduler* sched_);

  // 워크로드 로드
  void load_workload();

  // 스케줄링 정보 Terminal 출력
  void print_stat();

  // 스케줄링 순서 Terminal 출력
  void print_order();

  // 정답 확인
  void check_answer(const std::string& scheduler_name, std::string suffix);
};





