#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP


/*
 * attention
 * in Block::Del(), head and tail have been deleted
 * but in deque::clear(), I did't delete head and tail
 * must delete them outside
 * in this way,I can delete head->head,head->tail.........etc.
 */

#include "exceptions.hpp"

#include <cstddef>
#include <cmath>

namespace sjtu {

    template<class T>
    class deque {

    private:
        /*
         * Judge whether to split or merge
         * */
        bool toSplit(const int &k){
            return k>=20&&k>=int(2.5*std::sqrt(double(curSize)));
        }
        bool toMerge(const int &k){
            return k<=int(0.5*std::sqrt(double(curSize)));
        }

    private:
        /*
         *a Node contains one element
         * */
        struct Node {
            Node *prev, *next;
            T *v;

            Node() : prev(nullptr), next(nullptr), v(nullptr) {}

            ~Node() {
                delete v;
            }
        };

        /*
         * a Block contains sqrt(n) Node
         * */
        struct Block {
            Block *prev, *next;
            Node *head, *tail;
            size_t _curSize;

            Block() : head(new Node), tail(new Node), prev(nullptr), next(nullptr), _curSize(0) {  //construct
                head->next = tail;
                tail->prev = head;
            }

            static Block *Copy(Block *b) {

                auto news = new Block;

                for (auto x = b->head->next; x != b->tail; x = x->next) {
                    auto tmp = new Node;

                    tmp->v = new T(*(x->v));

                    news->tail->prev->next = tmp;
                    tmp->prev = news->tail->prev;

                    tmp->next = news->tail;
                    news->tail->prev = tmp;

                }
                news->_curSize = b->_curSize;

                return news;
            }  //copy //attention the memory

            void Merge() {

                Node *x2 = tail, *x3 = next->head;

                x2->prev->next = x3->next;
                x3->next->prev = x2->prev;
                delete x2, x3;

                tail = next->tail;

                _curSize += next->_curSize;

                Block *tmp = next;
                tmp->next->prev = this;
                next = tmp->next;
                delete tmp;

            }  //merge this and next

            void Split(const int &k) {
                auto news = new Block;

                Node *pos1 = head;
                for (size_t i = 0; i < k; i++)
                    pos1 = pos1->next;
                Node *pos2 = pos1->next;


                news->next = next;
                news->prev = this;
                next->prev = news;
                next = news;

                //head1 is still head1

                news->tail->prev = tail->prev;
                tail->prev->next = news->tail;  //tail2

                news->head->next = pos2;
                pos2->prev = news->head; //head2

                tail->prev = pos1;
                pos1->next = tail;   //tail1

                news->_curSize = _curSize - k;
                _curSize = k;     //size
            }  //split into 0~k-1 and k~curSize-1

            void Del() {
                Node *x = head->next;
                while (x != tail) {
                    Node *tmp = x->next;
                    delete x;
                    x = tmp;
                }
                _curSize = 0;
                delete head, tail;
            }

            /*~Block() {
                Node *x = head->next;
                while (x != tail) {
                    Node *tmp = x->next;
                    delete x;
                    x = tmp;
                }
                _curSize = 0;
                delete head, tail;
            }
             */ //Don't need!!!!!!!  cause delete again and running error
        };

    public:

        class const_iterator;

        class iterator {
        private:
            /*
             * these three pointers point to the location
             * */
            deque* nowdeque;
            Block* nowBlock;
            Node* nowNode;
        public:
            explicit iterator(deque* a1 = nullptr,Block* a2 = nullptr,Node* a3 = nullptr){
                nowdeque=a1;
                nowBlock=a2;
                nowNode=a3;
            }
            iterator(const iterator &other)= default;

        public:
            iterator operator+(const int &n) const {
                if(n<0)
                    return operator-(-n);

                iterator news(*this);
                int cnt = 0;

                while (cnt < n && news.nowNode->next != news.nowBlock->tail) {
                    cnt++;
                    news.nowNode = news.nowNode->next;
                }
                if (cnt == n) return news;

                cnt++;
                news.nowBlock = news.nowBlock->next;

                while (cnt + news.nowBlock->_curSize <= n && news.nowBlock->_curSize != 0) {
                    cnt += news.nowBlock->_curSize;
                    news.nowBlock = news.nowBlock->next;
                }

                if (news.nowBlock->_curSize == 0)
                    throw invalid_iterator();

                news.nowNode = news.nowBlock->head->next;
                while (cnt < n) {
                    cnt++;
                    news.nowNode = news.nowNode->next;
                }
                return news;

            }

            iterator operator-(const int &n) const {
                if(n<0)
                    return operator+(-n);

                iterator news(*this);
                int cnt = 0;

                while (cnt < n && news.nowNode->prev != news.nowBlock->head) {
                    cnt++;
                    news.nowNode = news.nowNode->prev;
                }
                if (cnt == n) return news;

                cnt++;
                news.nowBlock = news.nowBlock->prev;

                while (cnt + news.nowBlock->_curSize <= n && news.nowBlock->_curSize != 0) {
                    cnt += news.nowBlock->_curSize;
                    news.nowBlock = news.nowBlock->prev;
                }

                if (news.nowBlock->_curSize == 0)
                    throw invalid_iterator();

                news.nowNode = news.nowBlock->tail->prev;
                while (cnt < n) {
                    cnt++;
                    news.nowNode = news.nowNode->prev;
                }
                return news;
            }

            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
            }

            iterator &operator+=(const int &n) {
                //TODO
            }

            iterator &operator-=(const int &n) {
                //TODO
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {}

            /**
             * TODO ++iter
             */
            iterator &operator++() {}

            /**
             * TODO iter--
             */
            iterator operator--(int) {}

            /**
             * TODO --iter
             */
            iterator &operator--() {}

            /**
             * TODO *it
             */
            T &operator*() const {}

            /**
             * TODO it->field
             */
            T *operator->() const noexcept {}

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const {}

            bool operator==(const const_iterator &rhs) const {}

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {}

            bool operator!=(const const_iterator &rhs) const {}
        };

        class const_iterator {
        private:
            deque* nowdeque;
            Block* nowBlock;
            Node* nowNode;
        public:
            const_iterator() {
                // TODO
            }

            const_iterator(const const_iterator &other) {
                // TODO
            }

            const_iterator(const iterator &other) {
                // TODO
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
        };  //todo

    private:
        size_t curSize;
        Block *head, *tail;

    public:
        deque() : head(new Block), tail(new Block), curSize(0) {
            head->next = tail;
            tail->prev = head;
        }

        deque(const deque &other) : head(new Block), tail(new Block), curSize(other.curSize) {
            head->next = tail;
            tail->prev = head;

            for (auto x = other.head->next; x != other.tail; x = x->next) {

                auto news = Block::Copy(x);

                tail->prev->next = news;
                news->prev = tail->prev;

                tail->prev = news;
                news->next = tail;
            }
        }  //attention the memory

        ~deque() {
            clear();  //here I continue to do the work after clear
            delete head->head, head->tail, tail->head, tail->tail;
            delete head, tail;
        }

        deque &operator=(const deque &other) {
            if (this == &other) return *this;

            clear();  //though clear, but in my settings, head and tail remained

            for (auto x = other.head->next; x != other.tail; x = x->next) {

                auto news = Block::Copy(x);

                tail->prev->next = news;
                news->prev = tail->prev;

                tail->prev = news;
                news->next = tail;
            }
            curSize = other.curSize;
            return *this;
        }

        T &at(const size_t &pos) {
            return operator[](pos);
        }
        const T &at(const size_t &pos) const {
            return operator[](pos);
        }
        T &operator[](const size_t &pos) {
            if (pos < 0 || pos >= curSize)
                throw index_out_of_bound();

            Block *curBlock = head->next;
            int curPos = 0;

            while (curPos + curBlock->_curSize <= pos) {
                curPos += curBlock->_curSize;
                curBlock = curBlock->next;
            }

            Node *curNode = curBlock->head->next;
            while (curPos < pos) {
                curPos++;
                curNode = curNode->next;
            }
            return *(curNode->v);
        }
        const T &operator[](const size_t &pos) const {
            if (pos < 0 || pos >= curSize)
                throw index_out_of_bound();

            Block *curBlock = head->next;
            int curPos = 0;

            while (curPos + curBlock->_curSize <= pos) {
                curPos += curBlock->_curSize;
                curBlock = curBlock->next;
            }

            Node *curNode = curBlock->head->next;
            while (curPos < pos) {
                curPos++;
                curNode = curNode->next;
            }
            return *(curNode->v);
        }

        const T &front() const {
            if(curSize==0)
                throw container_is_empty();
            return *(head->next->head->next->v);
        }
        const T &back() const {
            if(curSize==0)
                throw container_is_empty();
            return *(tail->prev->tail->prev->v);
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {} //todo

        const_iterator cbegin() const {}  //todo

        /**
         * returns an iterator to the end.
         */
        iterator end() {} //todo

        const_iterator cend() const {}  //todo

        bool empty() const {
            return curSize==0;
        }
        size_t size() const {
            return curSize;
        }

        void clear() {
            Block *x = head->next;
            while (x != tail) {
                auto *tmp = x->next;
                x->Del();  //in Del(), head and tail have been deleted
                delete x;
                x = tmp;
            }
            head->next = tail;
            tail->prev = head; //attention: here I didn't delete the head and tail
            curSize = 0;
        }

        /**
         * inserts elements at the specified locat on in the container.
         * inserts value before pos
         * returns an iterator pointing to the inserted value
         *     throw if the iterator is invalid or it point to a wrong place.
         */
        iterator insert(iterator pos, const T &value) {}  //todo

        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator pos) {} //todo


        void push_back(const T &value) {
            curSize++;
            if(curSize!=1&&!toSplit(tail->prev->_curSize)){
                tail->prev->_curSize++;
                auto tmp=new Node;
                tmp->v = new T(value);

                tail->prev->tail->prev->next = tmp;
                tmp->prev = tail->prev->tail->prev;
                tmp->next = tail->prev->tail;
                tail->prev->tail->prev = tmp;
            }else{  //a new Block
                auto tmp=new Node;
                tmp->v = new T(value);
                auto news=new Block;

                news->head->next = news->tail->prev = tmp;
                tmp->prev = news->head;
                tmp->next = news->tail;

                tail->prev->next = news;
                news->prev = tail->prev;

                news->next = tail;
                tail->prev = news;

                news->_curSize=1;
            }
        }
        void pop_back() {
            if(curSize==0)
                throw container_is_empty();

            curSize--;
            tail->prev->_curSize--;

            if(tail->prev->_curSize==0){
                auto tmp=tail->prev;

                delete tmp->head->next; //delete that cute element
                delete tmp->head,tmp->tail;

                tmp->prev->next = tmp->next;
                tmp->next->prev = tmp->prev;

                delete tmp;
            }else{
                auto tmpNode=tail->prev->tail->prev;

                tmpNode->prev->next = tmpNode->next;
                tmpNode->next->prev = tmpNode->prev;

                delete tmpNode;
            }
        }
        void push_front(const T &value) {
            curSize++;
            if(curSize!=1&&!toSplit(tail->prev->_curSize)){
                head->next->_curSize++;
                auto tmp=new Node;
                tmp->v=new T(value);

                head->next->head->next->prev = tmp;
                tmp->next = head->next->head->next;
                tmp->prev = head->next->head;
                head->next->head->next  = tmp;
            }else{
                auto tmp=new Node;
                tmp->v=new T(value);
                auto news=new Block;

                news->head->next = news->tail->prev = tmp;
                tmp->prev = news->head;
                tmp->next = news->tail;

                head->next->prev = news;
                news->next = head->next;

                news->prev = head;
                head->next = news;

                news->_curSize = 1;
            }

        }
        void pop_front() {
            if(curSize==0)
                throw container_is_empty();

            curSize--;
            head->next->_curSize--;

            if (head->next->_curSize == 0) {
                auto tmp = head->next;

                delete tmp->head->next;
                delete tmp->head,tmp->tail;

                tmp->prev->next = tmp->next;
                tmp->next->prev = tmp->prev;

                delete tmp;
            } else {
                auto tmpNode = head->next->head->next;

                tmpNode->prev->next = tmpNode->next;
                tmpNode->next->prev = tmpNode->prev;

                delete tmpNode;
            }
        }
    };

}

#endif
