#include <pthread.h>
#ifndef BST_H
#define BST_H

/// @brief Key, Value, Update Count 구조체
struct KVC {
    int key;
    int value;
    int upd_cnt;
};

/// @brief BST 노드 구조체
struct Node{
    // 키
    int key;
    // 밸류
    int value;
    // 업데이트 횟수
    int upd_cnt;
    // left child 노드
    Node* left;
    // right child 노드
    Node* right;
};

/// @brief BST 부모 클래스.
class DefaultBST {
    protected:
        // traverse시, 인자로 받은 KVC 배열 포인터 저장
        KVC* traverse_arr_;
        // traverse시, KVC 배열에 data를 삽입할 위치 계산 용도
        int arr_index_;
        
    public:
        /**
         * @brief 삽입 함수 : 인자로 받은 key와 value를 저장한다
         * 이미 key가 존재하는 경우, 기존 value에 새로운 value를 더하고 update count를 1 증가시킨다.
         * 
         * @param key 
         * @param value 
         */
        virtual void insert(int key, int value) = 0;

        /**
         * @brief 탐색 함수 : 인자로 받은 key의 value를 반환한다.
         * BST에 key가 존재하지 않을 경우, 0을 반환한다.
         * 
         * @param key 
         * @return int 
         */
        virtual int lookup(int key) = 0;

        /**
         * @brief 삭제 함수 : 인자로 받은 key가 저장된 노드를 삭제한다.
         * 
         * @param key 
         */
        virtual void remove(int key) = 0;

        /**
         * @brief 순회 함수 : 중위 순회하여, KVC 배열에 BST에 존재하는 모든 데이터를 저장한다.
         * KVC 배열에 key, value, upd_cnt가 key의 오름차순으로 저장한다.
         * KVC 배열은 함수 호출 전 이미 메모리에 할당된다.
         * KVC 배열의 길이는 그 길이는 std::map으로 single-thread로 해당 워크로드를 수행한 것과 동일하다.
         * KVC 배열에 데이터 삽입 시, arr_index_를 증감 연산하며 활용하길 권장한다.
         * @param arr 
         */
        virtual void traversal(KVC* arr) = 0;
};
#endif // BST_H
