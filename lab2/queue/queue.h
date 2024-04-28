#include <pthread.h>
#include <utility>

#ifndef QUE_H
#define QUE_H

/// @brief Queue의 부모 클래스
class DefaultQueue {
    public:
        /**
         * @brief 큐 back에 key-value pair를 삽입한다.
         * 
         * @details 큐가 가득 찼다면, 공간을 새로 할당하여 삽입한다.
         * 
         * @param key
         * @param value 
         */
        virtual void enqueue(int key, int value) = 0;

        /**
         * @brief 큐 front에 존재하는 key-value pair를 반환하고, 이 큐에서 삭제한다.
         * 
         * @details 큐 front가 비었다면, {-1,-1}를 반환한다.
         * 
         * @return std::pair<int, int> 
         */
        virtual std::pair<int, int> dequeue() = 0;
        
        /**
         * @brief 큐가 비었다면 true, 아니라면 false를 반환한다.
         * 
         * @return true 
         * @return false 
         */
        virtual bool empty() = 0;
};
#endif // QUE_H
