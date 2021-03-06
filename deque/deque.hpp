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
        bool toSplit(const int &k) {
            return k > 20 && k > int(3.12* std::sqrt(curSize));
        }
        bool toNew(const int &k){
            return k > 20 && k > int(2.58* std::sqrt(curSize));
        }

        bool toMerge(const int &k) {
            return k < int(0.6 * std::sqrt(curSize));
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

            Block() : head(new Node), tail(new Node), _curSize(0) {  //construct
                head->next = tail;
                tail->prev = head;
            }

            static Block *Copy(Block *b) {

                auto news = new Block;
                news->_curSize = b->_curSize;
                for (auto x = b->head->next; x != b->tail; x = x->next) {
                    auto tmp = new Node;

                    tmp->v = new T(*(x->v));

                    news->tail->prev->next = tmp;
                    tmp->prev = news->tail->prev;

                    tmp->next = news->tail;
                    news->tail->prev = tmp;

                }
                return news;
            }  //copy //attention the memory

            void Merge() {
/*
                auto curNode = next->head->next;

                while(curNode!=next->tail){
                    auto newNode =new Node;
                    newNode->v = new T(*(curNode->v));

                    newNode->prev=tail->prev;
                    tail->prev->next=newNode;

                    newNode->next=tail;
                    tail->prev=newNode;

                    _curSize++;

                    curNode=curNode->next;
                }

                Block *tmp = next;
                tmp->next->prev = this;
                next = tmp->next;
                delete tmp;
                */

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
            friend iterator deque::insert(iterator pos, const T &value);
            friend iterator deque::erase(iterator pos);

        private:
            /*
             * these three pointers point to the location
             * */
            deque *nowdeque;
            Block *nowBlock;
            Node *nowNode;

        public:
            explicit iterator(deque *a1 = nullptr, Block *a2 = nullptr, Node *a3 = nullptr):nowdeque(a1),nowBlock(a2),nowNode(a3){}
            iterator(const iterator &other) = default;
            explicit iterator(const const_iterator &other):nowdeque(other.nowdeque), nowBlock(other.nowBlock), nowNode(other.nowNode) {}

        public:
            int getPos() const {
                int pos = 0;
                for (auto x = nowNode; x->prev != nowBlock->head; x = x->prev)
                    pos++;
                for (auto x = nowBlock; x->prev != nowdeque->head; x =x->prev)
                    pos += x->prev->_curSize;

                return pos;
            }  //attention: this pos is 0-base

            iterator operator+(const int &n) const {
                if (n < 0)
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

                if (news.nowBlock->_curSize == 0 && cnt!=n)
                    throw invalid_iterator();

                news.nowNode = news.nowBlock->head->next;
                while (cnt < n) {
                    cnt++;
                    news.nowNode = news.nowNode->next;
                }
                return news;

            }

            iterator operator-(const int &n) const {
                if (n < 0)
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

                if (news.nowBlock->_curSize == 0 && cnt!=n)
                    throw invalid_iterator();

                news.nowNode = news.nowBlock->tail->prev;
                while (cnt < n) {
                    cnt++;
                    news.nowNode = news.nowNode->prev;
                }
                return news;
            }

            int operator-(const iterator &rhs) const {
                if (nowdeque != rhs.nowdeque)
                    throw invalid_iterator();
                return getPos() - rhs.getPos();
            }

            iterator &operator+=(const int &n) {
                iterator tmp = operator+(n);
                nowBlock = tmp.nowBlock;
                nowNode = tmp.nowNode;
                return *this;
            }

            iterator &operator-=(const int &n) {
                iterator tmp = operator-(n);
                nowBlock = tmp.nowBlock;
                nowNode = tmp.nowNode;
                return *this;
            }

            const iterator operator++(int) {
                if (*this == nowdeque->end())
                    throw invalid_iterator();

                auto x = iterator(*this);

                nowNode = nowNode->next;
                if (nowNode == nowBlock->tail) {
                    nowBlock = nowBlock->next;
                    nowNode = nowBlock->head->next;
                }
                return x;
            }

            iterator &operator++() {
                if (*this == nowdeque->end())
                    throw invalid_iterator();

                nowNode = nowNode->next;
                if (nowNode == nowBlock->tail) {
                    nowBlock = nowBlock->next;
                    nowNode = nowBlock->head->next;
                }

                return *this;
            }

            const iterator operator--(int) {
                if (*this == nowdeque->begin())
                    throw invalid_iterator();

                auto x = iterator(*this);

                nowNode = nowNode->prev;
                if (nowNode == nowBlock->head) {
                    nowBlock = nowBlock->prev;
                    nowNode = nowBlock->tail->prev;
                }
                return x;
            }

            iterator &operator--() {
                if (*this == nowdeque->begin())
                    throw invalid_iterator();

                nowNode = nowNode->prev;
                if (nowNode == nowBlock->head) {
                    nowBlock = nowBlock->prev;
                    nowNode = nowBlock->tail->prev;
                }
                return *this;
            }

            T &operator*() const {
                if (nowNode == nullptr || nowNode->v == nullptr)
                    throw invalid_iterator();
                return *(nowNode->v);
            }

            T *operator->() const noexcept {
                //if (nowNode == nullptr || nowNode->v == nullptr) throw invalid_iterator();
                return nowNode->v;
            }

            bool operator==(const iterator &rhs) const {
                return nowNode == rhs.nowNode;
            }

            bool operator==(const const_iterator &rhs) const {
                return nowNode == rhs.nowNode;
            }

            bool operator!=(const iterator &rhs) const {
                return nowNode != rhs.nowNode;
            }

            bool operator!=(const const_iterator &rhs) const {
                return nowNode != rhs.nowNode;
            }
        };

        class const_iterator {
        private:
            const deque *nowdeque;
            const Block *nowBlock;
            const Node *nowNode;

        public:
            explicit const_iterator(const deque *a1 = nullptr, const Block *a2 = nullptr, const Node *a3 = nullptr):nowdeque(a1),nowBlock(a2),nowNode(a3){}
            const_iterator(const const_iterator &other) = default;
            explicit const_iterator(const iterator &other):nowdeque(other.nowdeque), nowBlock(other.nowBlock), nowNode(other.nowNode) {}

        public:
            int getPos() const {
                int pos = 0;
                for (auto x = nowNode; x->prev!= nowBlock->head; x = x->prev)
                    pos++;
                for (auto x = nowBlock; x->prev != nowdeque->head; x = x->prev)
                    pos += x->prev->_curSize;
                return pos;
            }  //attention: this pos is 0-base

            const_iterator operator+(const int &n) const {
                if (n < 0)
                    return operator-(-n);

                const_iterator news(*this);
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

                if (news.nowBlock->_curSize == 0 && cnt!=n)
                    throw invalid_iterator();

                news.nowNode = news.nowBlock->head->next;
                while (cnt < n) {
                    cnt++;
                    news.nowNode = news.nowNode->next;
                }
                return news;

            }

            const_iterator operator-(const int &n) const {
                if (n < 0)
                    return operator+(-n);

                const_iterator news(*this);
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

                if (news.nowBlock->_curSize == 0 && cnt!=n)
                    throw invalid_iterator();

                news.nowNode = news.nowBlock->tail->prev;
                while (cnt < n) {
                    cnt++;
                    news.nowNode = news.nowNode->prev;
                }
                return news;
            }

            int operator-(const const_iterator &rhs) const {
                if (nowdeque != rhs.nowdeque)
                    throw invalid_iterator();
                return getPos() - rhs.getPos();
            }

            const_iterator &operator+=(const int &n) {
                const_iterator tmp = operator+(n);
                nowBlock = tmp.nowBlock;
                nowNode = tmp.nowNode;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                const_iterator tmp = operator-(n);
                nowBlock = tmp.nowBlock;
                nowNode = tmp.nowNode;
                return *this;
            }

            const const_iterator operator++(int) {
                if (*this == nowdeque->cend())
                    throw invalid_iterator();

                auto x = const_iterator(*this);

                nowNode = nowNode->next;
                if (nowNode == nowBlock->tail) {
                    nowBlock = nowBlock->next;
                    nowNode = nowBlock->head->next;
                }
                return x;
            }

            const_iterator &operator++() {
                if (*this == nowdeque->cend())
                    throw invalid_iterator();

                nowNode = nowNode->next;
                if (nowNode == nowBlock->tail) {
                    nowBlock = nowBlock->next;
                    nowNode = nowBlock->head->next;
                }

                return *this;
            }

            const const_iterator operator--(int) {
                if (*this == nowdeque->cbegin())
                    throw invalid_iterator();

                auto x = const_iterator(*this);

                nowNode = nowNode->prev;
                if (nowNode == nowBlock->head) {
                    nowBlock = nowBlock->prev;
                    nowNode = nowBlock->tail->prev;
                }
                return x;
            }

            const_iterator &operator--() {
                if (*this == nowdeque->cbegin())
                    throw invalid_iterator();

                nowNode = nowNode->prev;
                if (nowNode == nowBlock->head) {
                    nowBlock = nowBlock->prev;
                    nowNode = nowBlock->tail->prev;
                }
                return *this;
            }

            const T &operator*() const {
                if (nowNode == nullptr || nowNode->v == nullptr)
                    throw invalid_iterator();
                return *(nowNode->v);
            }

            const T *operator->() const noexcept {
                //if (nowNode == nullptr || nowNode->v == nullptr) throw invalid_iterator();
                return nowNode->v;
            }

            bool operator==(const iterator &rhs) const {
                return nowNode == rhs.nowNode;
            }

            bool operator==(const const_iterator &rhs) const {
                return nowNode == rhs.nowNode;
            }

            bool operator!=(const iterator &rhs) const {
                return nowNode != rhs.nowNode;
            }

            bool operator!=(const const_iterator &rhs) const {
                return nowNode != rhs.nowNode;
            }
        };

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
            curSize = other.curSize;
            for (auto x = other.head->next; x != other.tail; x = x->next) {

                auto news = Block::Copy(x);

                tail->prev->next = news;
                news->prev = tail->prev;

                tail->prev = news;
                news->next = tail;
            }
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
            if(pos<curSize/2) {
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
            }else{
                int Pos=curSize-pos-1;
                Block *curBlock =tail->prev;
                int curPos=0;

                while(curPos+curBlock->_curSize<=Pos){
                    curPos+=curBlock->_curSize;
                    curBlock=curBlock->prev;
                }

                Node *curNode = curBlock->tail->prev;
                while(curPos<Pos){
                    curPos++;
                    curNode=curNode->prev;
                }
                return *(curNode->v);
            }

        }

        const T &operator[](const size_t &pos) const {
            if (pos < 0 || pos >= curSize)
                throw index_out_of_bound();

            if(pos<curSize/2) {
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
            }else{
                int Pos=curSize-pos-1;
                Block *curBlock =tail->prev;
                int curPos=0;

                while(curPos+curBlock->_curSize<=Pos){
                    curPos+=curBlock->_curSize;
                    curBlock=curBlock->prev;
                }

                Node *curNode = curBlock->tail->prev;
                while(curPos<Pos){
                    curPos++;
                    curNode=curNode->prev;
                }
                return *(curNode->v);
            }
        }

        const T &front() const {
            if (curSize == 0)
                throw container_is_empty();
            return *(head->next->head->next->v);
        }

        const T &back() const {
            if (curSize == 0)
                throw container_is_empty();
            return *(tail->prev->tail->prev->v);
        }

        iterator begin() {
            return iterator(this, head->next, head->next->head->next);
        }

        const_iterator cbegin() const {
            return const_iterator(this, head->next, head->next->head->next);
        }

        //pointing to the next of the last element
        //attention: **the next!!!!**
        iterator end() {
            return iterator(this, tail, tail->head->next);
        }

        const_iterator cend() const {
            return const_iterator(this, tail, tail->head->next);
        }

        bool empty() const {
            return curSize == 0;
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
        iterator insert(iterator pos, const T &value) {
            if(pos.nowdeque!=this)
                throw invalid_iterator();
            if(pos==end()){
                push_back(value);
                return --end();
            }

            curSize++;
            auto tmp = pos.nowBlock;
            tmp->_curSize++;

            auto newNode =new Node;
            newNode->v= new T(value);
            auto priNode = pos.nowNode;

            newNode->prev =priNode->prev;
            newNode->prev->next=newNode;
            newNode->next=priNode;
            priNode->prev=newNode;

            if(toSplit(tmp->_curSize)){
                tmp->Split((tmp->_curSize)>>1);
            }
            for(auto x=tmp->head->next;x!=tmp->tail;x=x->next){
                if(x==newNode)
                    return iterator(this,tmp,newNode);
            }
            return iterator(this,tmp->next,newNode);
        }

        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator pos) {
            if (pos.nowdeque!=this)
                throw invalid_iterator();
            *pos;
            curSize--;

            auto priBlock=pos.nowBlock;
            priBlock->_curSize--;
            auto priNode=pos.nowNode;

            if(priBlock->_curSize==0){
                iterator news(this,priBlock->next,priBlock->next->head->next);

                priBlock->Del();

                priBlock->next->prev=priBlock->prev;
                priBlock->prev->next=priBlock->next;

                delete priBlock;

                return news;
            }      //specially the Block size==0;

            bool flag=priNode->next==priBlock->tail;
            Node* nextNode=flag?priBlock->next->head->next:priNode->next;    //get the nextNode

            priNode->next->prev=priNode->prev;
            priNode->prev->next=priNode->next;   //link the prev and next of the erase Node

            delete priNode;

            if(priBlock->prev!=head && toMerge(priBlock->_curSize + priBlock->prev->_curSize)){
                auto newBlock=priBlock->prev;
                newBlock->Merge();

                if(flag){
                    return iterator(this,newBlock->next,nextNode);
                } else{
                    return iterator(this,newBlock,nextNode);
                }
            }else if(priBlock->next!=tail && toMerge(priBlock->_curSize + priBlock->next->_curSize)){
                priBlock->Merge();
                return iterator(this,priBlock,nextNode);
            }else{
                if(flag){
                    return iterator(this,priBlock->next,nextNode);
                }else{
                    return iterator(this,priBlock,nextNode);
                }
            }
        }

        void push_back(const T &value) {
            curSize++;
            if (curSize == 1 || toNew(tail->prev->_curSize)) {
                auto tmp = new Node;
                tmp->v = new T(value);
                auto news = new Block;

                news->head->next = news->tail->prev = tmp;
                tmp->prev = news->head;
                tmp->next = news->tail;

                tail->prev->next = news;
                news->prev = tail->prev;

                news->next = tail;
                tail->prev = news;

                news->_curSize = 1;
            }else {
                tail->prev->_curSize++;
                auto tmp = new Node;
                tmp->v = new T(value);

                tail->prev->tail->prev->next = tmp;
                tmp->prev = tail->prev->tail->prev;
                tmp->next = tail->prev->tail;
                tail->prev->tail->prev = tmp;

            }
        }

        void pop_back() {
            if (curSize == 0)
                throw container_is_empty();

            curSize--;
            tail->prev->_curSize--;

            if (tail->prev->_curSize == 0) {
                auto tmp = tail->prev;

                delete tmp->head->next; //delete that cute element
                delete tmp->head, tmp->tail;

                tmp->prev->next = tmp->next;
                tmp->next->prev = tmp->prev;

                delete tmp;
            } else {
                auto tmpNode = tail->prev->tail->prev;

                tmpNode->prev->next = tmpNode->next;
                tmpNode->next->prev = tmpNode->prev;

                delete tmpNode;
            }
        }

        void push_front(const T &value) {
            curSize++;
            if (curSize == 1 || toNew(tail->prev->_curSize)) {
                auto tmp = new Node;
                tmp->v = new T(value);
                auto news = new Block;

                news->head->next = news->tail->prev = tmp;
                tmp->prev = news->head;
                tmp->next = news->tail;

                head->next->prev = news;
                news->next = head->next;

                news->prev = head;
                head->next = news;

                news->_curSize = 1;
            } else {

                head->next->_curSize++;
                auto tmp = new Node;
                tmp->v = new T(value);

                head->next->head->next->prev = tmp;
                tmp->next = head->next->head->next;
                tmp->prev = head->next->head;
                head->next->head->next = tmp;
            }

        }

        void pop_front() {
            if (curSize == 0)
                throw container_is_empty();

            curSize--;
            head->next->_curSize--;

            if (head->next->_curSize == 0) {
                auto tmp = head->next;

                delete tmp->head->next;
                delete tmp->head, tmp->tail;

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
