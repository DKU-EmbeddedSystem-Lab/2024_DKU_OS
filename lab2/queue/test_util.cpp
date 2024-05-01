
#include <iostream>
#include <unistd.h>
#include <random>
#include <chrono>
#include <iostream>
#include <algorithm>
#include "gtest/gtest.h"
#include "test_util.h"


/// @brief TEST_P 실행 전, 실행 되는 함수
void QueueTest::SetUp() {
  load_workload();
}

/// @brief TEST_P 실행 후, 실행 되는 함수
void QueueTest::TearDown() {
  run_workload();
  check_answer();
}

/**
 * @brief 워크로드를 불러오는 함수
 * Workload의 type과 num을 바탕으로, request을 생성한다.
 * Request의 key와 value는 1 ~ 100,000 사이의 랜덤한 값을 가진다.
 * 생성된 request들은 workload_에 저장된다.
 */
void QueueTest::load_workload() {
  // random 함수 초기화
  std::random_device rd;
  std::mt19937 gen(rd());
  // random 함수는 uniform random이고, 최소값은 1, 최대값은 100000
  std::uniform_int_distribution<> dis(1, 100000);

  // workload을 저장할 배열 메모리 할당 
  workload_ = (Req*)malloc(sizeof(Req)*workload_num_);
  
  // 첫번째 request는 enqueue로 고정
  workload_[0] = {1, 1, ENQUEUE};

  // 나머지 request 생성
  for (int i = 1; i < workload_num_; ++i) {
    // request key : random한 int 값 생성 후 저장
    workload_[i].key = dis(gen);
    // request value : random한 int 값 생성 후 저장
    workload_[i].value= dis(gen);

    // Workload Type에 따라, Request Type 생성 
    switch (workload_type_) {
      // ENQUEUE_ONLY인 경우
      case ENQ_THEN_DEQ:
        workload_[i].req_type = ENQUEUE;
        break;
      
      // ENQUEUE_DEQUEUE인 경우
      case ENQUEUE_DEQUEUE:
        // INSERT 50%, LOOKUP 50%
        workload_[i].req_type = static_cast<REQ_TYPE>(dis(gen) % 2);
        break;

      default:
        // 예외처리
        exit(0);
        break;
    }
  }
}

/**
 * @brief 워크로드를 실행하는 함수
 * Workload를 스레드 수 만큼 분할하고, 각 스레드에 할당한다.
 * 스레드와 스레드 인자를 생성하고, 실행하고, 조인한다.
 */
void QueueTest::run_workload() {
  // 스레드 구조체 포인터 배열 생성, 할당 및 초기화
  pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * thread_num_);
  memset(threads, 0x0, sizeof(pthread_t) * thread_num_);

  // 스레드 argument 포인터 배열 생성, 할당
  ThreadArg *th_args = (ThreadArg*)malloc(sizeof(ThreadArg) * thread_num_);

  // 스레드 argument 값 입력
  for (int i = 0; i < thread_num_; i++){
    // TestCase의 tree 포인터 저장
    th_args[i].queue = queue_;
    // TestCase의 thread 별 workload 개수
    th_args[i].workload_num = workload_num_ / thread_num_;
    // TestCase의 thread 별 workload 포인터 저장
    th_args[i].workload = workload_ + i * (workload_num_ / thread_num_);
    // TestCase의 thread 별 dequeue한 것을 저장하는 vec벡터
    th_args[i].vec = &th_vec_arr_[i];
  }

  // Thread 수행시작 시간 측정
  auto start = std::chrono::steady_clock::now();

  // 모든 Thread 생성 및 실행
  for (int i =0; i < thread_num_; i++){
    pthread_create(&threads[i], NULL, thread_job, (void*)(th_args + i));
  }

  // 모든 Thread 종료 및 대기
  for (int i = 0; i < thread_num_; i++){
    pthread_join(threads[i], NULL);
  }
  
  // Thread 수행종료 시간 측정
  auto end = std::chrono::steady_clock::now();
  // Thread 수행시간 계산
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  // Thread 수행시간 출력
  std::cout << "Execution time: " << duration_ms.count() << " ms" << std::endl;

  // 스레드 구조체 배열 메모리 해제
  free(threads);
  // 스레드 인자 배열 메모리 해제
  free(th_args);
}

/**
 * @brief 테스트 케이스의 수행결과를 확인하는 함수 
 * std::queue를 통해 single thread로 workload를 수행한다
 * std::queue가 dequeue한 data를 ans_vec_에 저장한 뒤 정렬한다.
 * 각 스레드의 vector를 합병하여 정렬하고, ans_vec_과 내부 요소가 모두 동일한지 확인한다.
 */
void QueueTest::check_answer() {
  // std::map으로 해당 workload를 싱글 스레드로 수행
  for (int i = 0; i < workload_num_; i++){
    switch (workload_[i].req_type) {
      // Enqueue인 경우
      case ENQUEUE:
        ans_queue_.push({workload_[i].key, workload_[i].value});
        break;
      case DEQUEUE:
      if (!ans_queue_.empty()){
        ans_vec_.push_back(ans_queue_.front());
        ans_queue_.pop();
      }
        break;
      default:
        break;
    }
  }
  
  while (!ans_queue_.empty()){
    ans_vec_.push_back(ans_queue_.front());
    ans_queue_.pop();
  }

  // ans_vec_ 정렬 (key 오름차순, value 오름차순)
  std::sort(ans_vec_.begin(), ans_vec_.end(), 
    [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
      // key가 동일하다면
      if (a.first == b.first) {
          // value를 비교
          return a.second < b.second;
      }
      // 키 비교
      return a.first < b.first;
  });

  // thread들의 벡터를 합병하여 저장할 벡터
  std::vector<std::pair<int, int>> merge_vec;

  // thread들의 벡터를 merge_vec에 합병
  for (int i = 0; i < thread_num_; i++){
    merge_vec.insert(merge_vec.end(), th_vec_arr_[i].begin(), th_vec_arr_[i].end());
  }

  // merge_vec 정렬 (key 오름차순, value 오름차순)
  std::sort(merge_vec.begin(), merge_vec.end(), 
    [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
      // key가 동일하다면
      if (a.first == b.first) {
          // value를 비교
          return a.second < b.second;
      }
      // key 비교
      return a.first < b.first;
  });

  ASSERT_EQ(ans_vec_.size(), merge_vec.size());

  //  answer_vec과 merge_vec의 모든 요소가 동일한지 확인 
  for (int i = 0; i < ans_vec_.size(); i++){
    ASSERT_EQ(ans_vec_[i].first, merge_vec[i].first);
    ASSERT_EQ(ans_vec_[i].second, merge_vec[i].second);
  }

  return;
}

/**
 * @brief 각 스레드가 자신에게 할당된 workload를 수행
 * workload의 request를 수행하고, dequeue한 경우 그 데이터를 vector에 저장한다.
 * 할당된 workload를 모두 수행하고, queue가 비어있지 않다면 빌 때까지 dequeue하고 이를 vector에 저장한다.
 * 
 * @param arg : 스레드 인자
 * @return void* 
 */
void* thread_job(void *arg){
  // void pointer로 받은 인자를 다시 ThreadArg pointer로 변환
  ThreadArg *th_arg = (ThreadArg *)arg;

  // 스레드 별 workload의 request를 차례대로 수행
  for (int i=0; i < th_arg->workload_num; i++){
    // workload에 존재하는 request type에 따라
    switch (th_arg->workload[i].req_type) {
      // 삽입
      case ENQUEUE:
        // request의 key-value pair를 enqueue한다.
        th_arg->queue->enqueue(th_arg->workload[i].key, th_arg->workload[i].value);
        break;
      // 탐색
      case DEQUEUE:{
        // queue를 접근하여 dequeue한다.
        std::pair<int, int> temp = th_arg->queue->dequeue();

        // 만약 key가 -1이 아니라면
        if (temp.first != -1){
          // 스레드의 벡터에 dequeue한 데이터를 삽입한다.
          th_arg->vec->push_back(temp);
        }
        break;
      }
      // 예외처리
      default:
        exit(0);
        break;
    }
  }

  // 워크로드를 수행하고 queue가 비어있지 않다면, 빌 때까지 dequeue한다.
  while (!th_arg->queue->empty()){
    std::pair<int, int> temp = th_arg->queue->dequeue();

    if (temp.first != -1){
      th_arg->vec->push_back(temp);
    }
  }
  // 종료
  return nullptr;
}