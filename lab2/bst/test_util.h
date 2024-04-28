#include <chrono>
#include <iomanip>
#include <map>
#include "bst.h"
#include "gtest/gtest.h"

/**
 * @brief Workload Type
 * INSERT_ONLY : workload num 만큼 insert한 후, single-thread로 중위 순회한다.
 * INSERT_LOOKUP : workload num 만큼 insert(50%). lookup(50%)한 후, single-thread로 중위 순회한다.
 * INSERT_LOOKUP_DELETE : workload num 만큼 insert(60%), lookup(20%), delete(20%)한다. 
 *                        Single-thread인 경우는 중위 순회를 수행하나, multi-thread인 경우 중위 순회를 수행하지 않는다.
 */
enum WL_TYPE{INSERT_ONLY, INSERT_LOOKUP, INSERT_LOOKUP_DELETE};

// Request Type
enum REQ_TYPE{INSERT, LOOKUP, DELETE};

// Request 구조체
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
  // BST 포인터
  DefaultBST* tree_;
};

/// @brief 부모 테스트 클래스
class BstTest : public ::testing::TestWithParam<std::tuple<WL_TYPE, int, int>> {
 protected:
  // 테스트 워크로드
  Req* workload_;
  // 테스트할 BST 포인터
  DefaultBST* tree_;
  // 정답용 std::map <키, 값, 업데이트 횟수>
  std::map<int, std::pair<int, int>> map_;
  // workload type
  WL_TYPE workload_type_ = std::get<0>(GetParam());
  // workload 총 횟수
  int workload_num_ = std::get<1>(GetParam());
  // 스레드 개수
  int thread_num_ = std::get<2>(GetParam());

 public:
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
   * std::map를 통해 single thread로 workload를 수행한다
   * BST와 std::map을 (중위)순회한 결과가 모두 동일한지 확인한다.
   */
  void check_answer();
};

/// @brief Test BST with Single Thread
class BstSingleThreadTest : public BstTest {};

/// @brief Test BST with Multi Thread
class BstMultiThreadTest : public BstTest {};

/**
 * @brief 각 스레드가 자신에게 할당된 workload를 수행
 * 
 * @param arg : 스레드 인자
 * @return void* 
 */
void* thread_job(void *arg);

