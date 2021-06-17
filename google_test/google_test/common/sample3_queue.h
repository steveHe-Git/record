#ifndef SAMPLE3_QUEUE_H_
#define SAMPLE3_QUEUE_H_

#include <stddef.h>

template<typename T>
class Queue;

template<typename T>
class QueueNode
{
    friend class Queue<T>;
private:
    /* data */
    T element;
    QueueNode* next;

    explicit QueueNode(T an_element):element(an_element),next(nullptr) {}
    const QueueNode& operator=(const QueueNode&);
    QueueNode(const QueueNode&);

public:
    QueueNode(/* args */){};
    ~QueueNode(){};

    const T& Element() const{
        return element;
    }

    QueueNode* Next() {
        return next;
    }

    const QueueNode* Next() const{
        return next;
    }
};

template <typename T>
class Queue
{
private:
    /* data */
    QueueNode<T>* head;
    QueueNode<T>* tail;
    size_t size;

    Queue(const Queue&);
    const Queue& operator=(const Queue&);
public:
    Queue(/* args */):head(nullptr), tail(nullptr), size(0){

    };

    ~Queue(){
        Clear();
    };

    void Clear(){
        if (size > 0) {
            QueueNode<T>* node = head;
            QueueNode<T>* next = head->Next();

            for (;;) {
                delete node;
                node = next;
                if (node == nullptr) break;
                next = node->Next();
            }
        }

        head = tail = nullptr;
        size = 0;
    }

    T* Dequeue() {
        if (size <= 0) {
            return nullptr;
        }

        const QueueNode<T>* const old_node = head;
        //这个位置使用head = old_node->Next();会报错，因为old_node为常对象需要调用常函数Next(),返回的是一个const的类型， 不能吧const赋给一个非常类型
        head = old_node->next;
        size--;
        if (size == 0) {
            tail = nullptr;
        }
        T* tmp_value = new T(old_node->Element());
        delete old_node;
        return tmp_value;
    }

    void Enqueue(const T& element){
        QueueNode<T>* new_node = new QueueNode<T>(element);
        if (size == 0) {
            head = tail = new_node;
            new_node->next = nullptr;
            size = 1;
        } else {
            tail->next = new_node;
            tail = new_node;
            new_node->next = nullptr;
            size++;
        }
    }
    
    QueueNode<T>* Head() {
        return head;
    }

    const QueueNode<T>* Head() const {
        return head;
    }

    QueueNode<T>* Tail(){
        return tail;
    }

    const QueueNode<T>* Tail() const{
        return tail;
    }

    const size_t Size() const{
        return size;
    }

    template<typename F>
    Queue<T>* Map(F function) const{
        Queue<T>* new_queue = new Queue<T>();
        for (QueueNode<T>* node = head; node != nullptr; node = node->Next()) {
            new_queue->Enqueue(function(node->Element()));
        }
        return new_queue;
    }
};



#endif