#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <unistd.h>
#include <openssl/sha.h>
#include "gtest/gtest.h"
#include "test_util.h"
#include "sched.h"

// TEST_P 실행 전, 실행 됨
void SchedulerTest::SetUp() {
  load_workload();
}

// TEST_P 실행 후, 실행 됨
void SchedulerTest::TearDown() {
  run_sched(sched_);
  print_order();
  print_stat();
  check_answer(sched_->get_name(), "stat");
  check_answer(sched_->get_name(), "order");
  delete sched_;
}

// 스케줄러 실행함수
void SchedulerTest::run_sched (Scheduler* sched_){
  int current_job = 0;
  
  // 반복문을 통해, 모든 잡이 완료될 때까지 실행
  // 모든 작업이 끝나면, 스케줄러가 -1을 반환
  do {
    current_job = sched_->run();
    // 스케줄링 작업 저장
    sched_log_.push_back(current_job);
  } while(current_job != -1);
  
  // 스케줄링 작업 정보 저장
  end_jobs_ = sched_->get_jobs_end();
  return;
}

void SchedulerTest::load_workload() {
    std::string file_name = "./data/workload_" + workload_name_ + ".csv";
    std::ifstream file(file_name);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << workload_name_ << std::endl;
        return; // Return empty queue if file cannot be opened
    }

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream linestream(line);
        std::string cell;
        Job job;

        // Read name
        std::getline(linestream, cell, ',');
        job.name = std::stoi(cell);

        // Read arrival_time
        std::getline(linestream, cell, ',');
        job.arrival_time = std::stoi(cell);

        // Read service_time
        std::getline(linestream, cell, ',');
        job.service_time = std::stoi(cell);
        job.remain_time = std::stoi(cell);

        job_queue_.push(job);
    }
    return;
}

// 통계정보 출력함수 Job 구조체에 저장된 정보를 통해 계산하고 출력
void SchedulerTest::print_stat() {
    // 테이블 형식 출력을 위한 메타 데이터 설정
    int nameWidth = 4; 
    int colWidth = 16; 
    int rowWidth = 6 * colWidth + 5 * 3 + 4;
    
    // 프로세스 이름순 정렬
    std::sort(end_jobs_.begin(), end_jobs_.end(),
        [](const Job& a, const Job& b) {
            return a.name < b.name;
        }
    );

    // 테이블 테두리 행 출력
    std::cout << std::setfill('-') << std::setw(rowWidth) << "-" << std::endl;
    std::cout << std::setfill(' ');

    // 테이블 헤더 출력
    std::cout << std::left << std::setw(nameWidth) << "Name" << " | "
            << std::setw(colWidth) << "Arrival Time" << " | "
            << std::setw(colWidth) << "Service Time" << " | "
            << std::setw(colWidth) << "First Run Time" << " | "
            << std::setw(colWidth) << "Completion Time" << " | "
            << std::setw(colWidth) << "Turn Around Time" << " | "
            << std::setw(colWidth) << "Response Time" << std::endl;
    
    // 테이블 테두리 행 출력
    std::cout << std::setfill('-') << std::setw(rowWidth) << "-" << std::endl;
    std::cout << std::setfill(' ');

    // 평균 작업 통계 정보 계산을 위한, 변수 초기화
    double sum_arrival_time = 0;
    double sum_service_time = 0;
    double sum_first_run_time = 0;
    double sum_completion_time = 0;
    double sum_turn_around_time = 0;
    double sum_response_time = 0;

    // 각 작업 별 통계 정보 계산 및 합산 
    for (const Job& job : end_jobs_) {
        double turn_around_time = job.completion_time - job.arrival_time;
        double response_time = job.first_run_time - job.arrival_time;
        
        sum_arrival_time += job.arrival_time;
        sum_service_time += job.service_time;
        sum_first_run_time += job.first_run_time;
        sum_completion_time += job.completion_time;
        sum_turn_around_time += turn_around_time;
        sum_response_time += response_time;

        std::cout << std::left << std::setw(nameWidth) << 'P' + std::to_string(job.name) << " | "
                << std::setw(colWidth) << job.arrival_time << " | "
                << std::setw(colWidth) << job.service_time << " | "
                << std::setw(colWidth) << job.first_run_time << " | "
                << std::setw(colWidth) << job.completion_time << " | "
                << std::setw(colWidth) << turn_around_time  << " | "
                << std::setw(colWidth) << response_time  << std::endl;
    }
    // 테이블 테두리 행 출력
    std::cout << std::setfill('-') << std::setw(rowWidth) << "-" << std::endl;
    std::cout << std::setfill(' ');

    // 작업 평균 통계 정보 계산 및 출력
    std::cout << std::left << std::setw(nameWidth) << "AVG" << " | "
            << std::setw(colWidth) << sum_arrival_time/end_jobs_.size() << " | "
            << std::setw(colWidth) << sum_service_time/end_jobs_.size() << " | "
            << std::setw(colWidth) << sum_first_run_time/end_jobs_.size() << " | "
            << std::setw(colWidth) << sum_completion_time/end_jobs_.size() << " | "
            << std::setw(colWidth) << sum_turn_around_time/end_jobs_.size()  << " | "
            << std::setw(colWidth) << sum_response_time/end_jobs_.size() << std::endl;
    
    // 테이블 테두리 행 출력
    std::cout << std::setfill('-') << std::setw(rowWidth) << "-" << std::endl;
    std::cout << std::setfill(' ');
}

// 통계정보 출력함수 Job 구조체에 저장된 정보를 통해 계산하고 출력
void SchedulerTest::print_order() {
    int max = *(std::max_element(sched_log_.begin(), sched_log_.end()));
    
    if (max == -1) return;
    
    std::string job_string[max];
    for (int i = 0; i < max; i++){
      job_string[i] = "P" + std::to_string(i+1) + "      |";
      for (int job : sched_log_){
        if (i + 1 == job){
          job_string[i] += " [] ";
        } else{
          job_string[i] += "    ";
        }
      }
    }

    std::string order_string = "Process |";
    std::string row_string = "---------";
    for (int i= 0; i<sched_log_.size() - 1; i++){
      order_string += " " + std::to_string(i);
      if (i<10){
        order_string += " ";
      }
      order_string += " ";
      row_string += "----";
    }
    std::cout << row_string << std::endl
              << order_string << std::endl
              << row_string << std::endl;
    
    for (int i = 0; i < max; i++){
      std::cout << job_string[i] << std::endl;
    }
    std::cout << row_string << std::endl;
}

void SchedulerTest::check_answer(const std::string& scheduler_name, std::string suffix){
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    if (suffix == "order"){
      int* data = sched_log_.data();
      size_t data_size = sched_log_.size();
      SHA256_Update(&sha256, data, data_size*sizeof(int));
    } else {
      Job* data = end_jobs_.data();
      size_t data_size = end_jobs_.size();
      SHA256_Update(&sha256, data, data_size*sizeof(Job));
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    std::string filename = "./data/answer/"+ workload_name_ + "_" + std::to_string(switch_time_) + "_" + scheduler_name + "_" + suffix;

    unsigned char hashFromFile[SHA256_DIGEST_LENGTH];

    std::ifstream file(filename, std::ios::binary);
    file.read(reinterpret_cast<char*>(hashFromFile), SHA256_DIGEST_LENGTH);
    file.close();

    ASSERT_TRUE(memcmp(hash, hashFromFile, SHA256_DIGEST_LENGTH) == 0);
}
