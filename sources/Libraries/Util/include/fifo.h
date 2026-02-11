
#ifndef _FIFO_H_
#define _FIFO_H_

struct fifo_elem
    {
    fifo_elem() {prev = next = NULL;}
    ~fifo_elem() {prev = next = NULL;}

    fifo_elem* prev;
    fifo_elem* next;};

template <class T>
class fifo
    {
public:
    fifo() {_head = _tail = NULL;}
    ~fifo() {_head = _tail = NULL;}

    void push(T* t)
        { // 往尾部压入一个元素
        if (t == NULL) return;
        if (_head == NULL)
            { // fifo为空
            _head = t;
            _head->prev = NULL;
            _head->next = NULL;
            _tail = _head;}
        else{ // fifo非空
            _tail->next = t;
            t->prev = _tail;
            t->next = NULL;
            _tail = t;}}

    T* pop()
        { // 从头部取出一个元素
        T* tmp;
        if (_head == _tail)
            {
            if (_head == NULL) return NULL; // fifo为空
            else{ // fifo剩余一个元素
                tmp = _head;
                _head = _tail = NULL;
                return tmp;}}
        else{ // fifo中剩余多于一个元素，取出一个
            tmp = _head;
            _head = (T *)_head->next;
            _head->prev = NULL;
            return tmp;}}

    int size() const
        { // 取得fifo的长度
        int sz = 0;
        T* curr = _head;
        if (curr == NULL) return sz; // fifo为空
        while (curr != _tail)
            { // fifo非空
            ++ sz; curr = (T *)curr->next;}
        return (sz + 1);}

protected:
    T* _head;
    T* _tail;};

#endif // _FIFO_H_
