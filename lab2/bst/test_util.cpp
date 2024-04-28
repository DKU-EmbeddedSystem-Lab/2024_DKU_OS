
#include <iostream>
#include <unistd.h>
#include <random>
#include <chrono>
#include "gtest/gtest.h"
#include "test_util.h"

/// @brief TEST_P 실행 전, 실행 됨
void BstTest::SetUp() {
  load_workload();
}

/// @brief TEST_P 실행 후, 실행 됨
void BstTest::TearDown() {
  run_workload();
  check_answer();
}

/**
 * @brief 워크로드를 불러오는 함수
 * Workload의 type과 num을 바탕으로, request을 생성한다.
 * Request의 key와 value는 1 ~ 100,000 사이의 랜덤한 값을 가진다.
 * 생성된 request들은 workload_에 저장된다.
 */
void BstTest::load_workload() {
  // random 함수 초기화
  std::random_device rd;
  std::mt19937 gen(rd());
  // random 함수는 uniform random이고, 최소값은 1, 최대값은 100000
  std::uniform_int_distribution<> dis(1, 100000);

  // workload을 저장할 배열 메모리 할당 
  workload_ = (Req*)malloc(sizeof(Req)*workload_num_);
  
  // 첫번째 request는 insert로 고정
  workload_[0] = {1, 1, INSERT};

  // 나머지 request 생성
  for (int i = 1; i < workload_num_; ++i) {
    // request key : random한 int 값 생성 후 저장
    workload_[i].key = dis(gen);
    // request value : random한 int 값 생성 후 저장
    workload_[i].value= dis(gen);

    // Workload Type에 따라, Request Type 생성 
    switch (workload_type_) {
      // INSERT_ONLY인 경우
      case INSERT_ONLY:
        workload_[i].req_type = INSERT;
        break;
      
      // INSERT_LOOKUP인 경우
      case INSERT_LOOKUP:
        // INSERT 50%, LOOKUP 50%
        workload_[i].req_type = static_cast<REQ_TYPE>(dis(gen) % 2);
        break;

      // INSERT_LOOKUP_DELETE인 경우
      case INSERT_LOOKUP_DELETE:
      {
        int num = dis(gen) % 5;
        
        // INSERT 60%, LOOKUP 20%, DELETE 20%
        if (num < 3) {
          workload_[i].req_type = static_cast<REQ_TYPE>(INSERT);
        } else if (num == 3){
          workload_[i].req_type = static_cast<REQ_TYPE>(LOOKUP);
        } else {
          workload_[i].req_type = static_cast<REQ_TYPE>(DELETE);
        }

        break;
      }
        
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
void BstTest::run_workload() {
  // 스레드 구조체 포인터 배열 생성, 할당 및 초기화
  pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * thread_num_);
  memset(threads, 0x0, sizeof(pthread_t) * thread_num_);

  // 스레드 argument 포인터 배열 생성, 할당
  ThreadArg *th_args = (ThreadArg*)malloc(sizeof(ThreadArg) * thread_num_);

  // 스레드 argument 값 입력
  for (int i = 0; i < thread_num_; i++){
    // TestCase의 tree 포인터 저장
    th_args[i].tree_ = tree_;
    // TestCase의 thread 별 workload 개수
    th_args[i].workload_num = workload_num_ / thread_num_;
    // TestCase의 thread 별 workload 포인터 저장
    th_args[i].workload = workload_ + i * (workload_num_ / thread_num_);
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
 * std::map를 통해 single thread로 workload를 수행한다
 * BST와 std::map을 (중위)순회한 결과가 모두 동일한지 확인한다.
 */
void BstTest::check_answer() {
  // INSERT_LOOKUP_DELETE이고 Multi-threading인 경우, 중위순회하지 않음
  if ((workload_type_ == INSERT_LOOKUP_DELETE) && (thread_num_ != 1)){
    return;
  }

  // std::map으로 해당 workload를 싱글 스레드로 수행
  for (int i = 0; i < workload_num_; i++){
    switch (workload_[i].req_type) {
      // Insert인 경우
      case INSERT:
      {
        // map에 해당 키가 존재하는 지 확인
        auto itr = map_.find(workload_[i].key);
        // 해당 키가 존재하지 않는다면,
        if (itr == map_.end()) {
            // 새롭게 해당 키, 값, 없데이트 횟수 삽입
            map_.insert({workload_[i].key, {workload_[i].value, 0}});
        // 해당 키가 이미 존재한다면,
        } else {
            // 새롭게 값 업데이트
            itr->second.first += workload_[i].value;
            // 업데이트 횟수 ++
            itr->second.second ++;
        }
        break;
      }
      case LOOKUP:
        // 정답 처리용 이므로, Lookup은 수행하지 않음
        break;
      case DELETE:
        // 키를 삭제함
        map_.erase(workload_[i].key);
        break;
      default:
        break;
    }
  }

  // map에 저장된 요소만큼, 배열을 할당함
  KVC* arr = new KVC[map_.size()];
  // tree를 traverse하여, tree에 존재하는 모든 key,value,upd_cnt를 arr에 오름차순으로 저장함
  tree_->traversal(arr);
  
  // map 순회용 iterator 생성
  auto iter = map_.begin();

  // 순회하면서 arr와 iter의 값을 비교 -> 다른 경우 TEST FAIL
  for (int i = 0; i < map_.size(); i++){
    // 키 값 비교
    ASSERT_EQ(iter->first, arr[i].key);
    // 밸류 값 비교
    ASSERT_EQ(iter->second.first, arr[i].value);
    // 업데이트 횟수 값 비교
    ASSERT_EQ(iter->second.second, arr[i].upd_cnt);
    // 다음 key 순회
    iter++;
  }
  // arr 메모리 해제
  delete[] arr;
}

/**
 * @brief 각 스레드가 자신에게 할당된 workload를 수행
 * 
 * @param arg : 스레드 인자
 * @return void* 
 */
void* thread_job(void *arg){
  // void pointer로 받은 인자를 다시 ThreadArg pointer로 변환
  ThreadArg *th_arg = (ThreadArg *)arg;

  // 스레드 별 workload 수행
  for (int i=0; i < th_arg->workload_num; i++){
    // workload에 존재하는 request type에 따라
    switch (th_arg->workload[i].req_type) {
      // 삽입
      case INSERT:
        th_arg->tree_->insert(th_arg->workload[i].key, th_arg->workload[i].value);
        break;
      // 탐색
      case LOOKUP:
        th_arg->tree_->lookup(th_arg->workload[i].key);
        break;
      // 삭제
      case DELETE:
        th_arg->tree_->remove(th_arg->workload[i].key);
        break;
      default:
        // 예외처리
        exit(0);
        break;
    }
  }
  // 종료
  return nullptr;
}