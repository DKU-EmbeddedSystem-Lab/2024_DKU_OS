#include "bst.h"

#ifndef BST_IMPI_H
#define BST_IMPI_H

/**
 * @brief BST without lock
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class BST : public DefaultBST {
	private:
		// 멤버 변수 추가 선언 가능

	public:
		// 멤버 함수 추가 선언 가능
		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;
};

/**
 * @brief BST with coarse-grained lock
 * BST 전체를 critical section으로 가정하여, 하나의 lock으로 이를 관리한다.
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class CoarseBST : public DefaultBST {
	private:
		// 멤버 변수 추가 선언 가능
		// pthread_mutex_t mutex_lock;
	public:
		// 멤버 함수 추가 선언 가능
		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;
};

/// @brief FineBST는 FineNode를 정의하여 사용하길 권장한다.
struct FineNode : public Node {
    // 멤버 변수 추가 가능
};

/**
 * @brief BST with fine-grained lock
 * BST 내부의 critical section을 Node 단위로 lock으로 관리한다.
 * Node를 상속받은 FineNode를 정의하여 사용하길 권장한다.
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class FineBST : public DefaultBST {
	private:
		// 멤버 변수 추가 선언 가능

	public:
		// 멤버 함수 추가 선언 가능
		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;
};
#endif