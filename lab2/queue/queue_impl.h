#include "queue.h"

#ifndef QUE_IMPI_H
#define QUE_IMPI_H

/**
 * @brief Queue without lock
 * DefaultQueue의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class Queue : public DefaultQueue {
	private:
		// 멤버 변수 추가 선언 가능
	public:
		// 멤버 함수 추가 선언 가능
        void enqueue (int key, int value) override;
        std::pair<int, int> dequeue () override;
		bool empty() override;
};


/**
 * @brief Queue with coarse-grained lock
 * Queue 전체를 critical section으로 가정하여, 하나의 lock으로 이를 관리한다.
 * DefaultQueue의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class CoarseQueue : public DefaultQueue {
	private:
		// 멤버 변수 추가 선언 가능
		// pthread_mutex_t mutex_lock;
	public:
		// 멤버 함수 추가 선언 가능
        void enqueue (int key, int value) override;
        std::pair<int, int> dequeue () override;
		bool empty() override;
};


/**
 * @brief Queue with fine-grained lock
 * Queue 내부의 critical section을 개별적으로 lock으로 관리한다.
 * DefaultQueue의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class FineQueue : public DefaultQueue {
	private:
		// 멤버 변수 추가 선언 가능
	public:
		// 멤버 함수 추가 선언 가능
        void enqueue (int key, int value) override;
        std::pair<int, int> dequeue () override;
		bool empty() override;
};
#endif