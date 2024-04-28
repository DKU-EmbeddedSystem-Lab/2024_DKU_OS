#include <chrono>
#include "queue.h"
#include "gtest/gtest.h"
#include <queue>

/**
 * @brief Workload Type
 * ENQ_THEN_DEQ : workload num 만큼 enqueue 한 뒤, 큐가 빌 때까지 dequeue한다.
 * ENQUEUE_DEQUEUE : workload num 만큼 en/dequeue 한 뒤, 큐가 빌 때까지 dequeue한다.
 */
enum WL_TYPE{ENQ_THEN_DEQ, ENQUEUE_DEQUEUE};

// Request Type
enum REQ_TYPE{ENQUEUE, DEQUEUE};

/// @brief Request 구조체
struct Req {
  // 키
  int key;
  // 밸류
  int value;
  // Request Type
  REQ_TYPE req_type;
};

/// @brief 스레드 Job 인자 구조체
struct ThreadArg {
  // Workload (= Request 배열)
  Req* workload;
  // Workload 수 
  int workload_num;
  // Queue 포인터
  DefaultQueue* queue;
  // Vector 포인터 (dequeue한 데이터를 저장하는 벡터의 포인터)
  std::vector<std::pair<int,int>>* vec;
};

/// @brief 부모 테스트 클래스
class QueueTest : public ::testing::TestWithParam<std::tuple<WL_TYPE, int, int>> {
 protected:
  // 테스트 워크로드
  Req* workload_;
  // 테스트할 Queue 포인터
  DefaultQueue* queue_;
  // 각 스레드용 dequeue한 데이터를 저장하는 벡터
  std::vector<std::pair<int, int>>* th_vec_arr_;
  // 정답용 std::map <키, 값>
  std::queue<std::pair<int, int>> ans_queue_;
  // 정답용 std::vector <키, 값>
  std::vector<std::pair<int,int>> ans_vec_;
  // workload type
  WL_TYPE workload_type_ = std::get<0>(GetParam());
  // workload 총 횟수
  int workload_num_ = std::get<1>(GetParam());
  // workload thread 수
  int thread_num_ = std::get<2>(GetParam());

 public:
  // 생성자
  QueueTest (){
    // 스레드 수 만큼 벡터를 동적 할당한다.
    th_vec_arr_ = new std::vector<std::pair<int, int>>[thread_num_];
  }

  /// @brief TEST_P 실행 전, 실행 되는 함수
  void SetUp() override;

  /// @brief TEST_P 실행 후, 실행 되는 함수
  void TearDown() override;

  /**
   * @brief 워크로드를 불러오는 함수
   * Workload의 type과 num을 바탕으로, request을 생성한다.
   * Request의 key와 value는 1 ~ 100,000 사이의 랜덤한 값을 가진다.
   * 생성된 request들은 workload_에 저장된다.
   */
  void load_workload();

  /**
   * @brief 워크로드를 실행하는 함수
   * Workload를 스레드 수 만큼 분할하고, 각 스레드에 할당한다.
   * 스레드와 스레드 인자를 생성하고, 실행하고, 조인한다.
   */
  void run_workload();

  /**
   * @brief 테스트 케이스의 수행결과를 확인하는 함수 
   * std::queue를 통해 single thread로 workload를 수행한다
   * std::queue가 dequeue한 data를 ans_vec_에 저장한 뒤 정렬한다.
   * 각 스레드의 vector를 합병하여 정렬하고, ans_vec_과 내부 요소가 모두 동일한지 확인한다.
   */
  void check_answer();
  
  // 소멸자
  ~QueueTest (){
    // 스레드 별 할당한 벡터 해제
    delete[] th_vec_arr_;
  }
};

/// @brief Test Queue with Single Thread
class QueueSingleThreadTest : public QueueTest {};

/// @brief Test Queue with Multi Thread
class QueueMultiThreadTest : public QueueTest {};

/**
 * @brief 각 스레드가 자신에게 할당된 workload를 수행
 * workload의 request를 수행하고, dequeue한 경우 그 데이터를 vector에 저장한다.
 * 할당된 workload를 모두 수행하고, queue가 비어있지 않다면 빌 때까지 dequeue하고 이를 vector에 저장한다.
 * 
 * @param arg : 스레드 인자
 * @return void* 
 */
void* thread_job(void *arg);
