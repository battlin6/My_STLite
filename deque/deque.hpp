#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

template<class T>
class deque{

private:
    size_t curSize;   //size

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

        Block() : head(new Node), tail(new Node), prev(nullptr),next(next),_curSize(0) {  //construct
            head->next = tail;
            tail->prev = head;
        }

        static Block* Copy(Block *b) {

            auto news = new Block;

            for (auto x = b->head->next; x != b->tail; x = x->next) {
                auto tmp = new Node;

                tmp->v = new T(*(tmp->v));

                news->tail->prev->next = tmp;
                tmp->prev = news->tail->prev;

                tmp->next = news->tail;
                news->tail->prev = tmp;

            }
            news->_curSize= b->_curSize;

            return news;
        }  //copy //attention the memory

        void Merge() {

            Node *x2 = tail, *x3 = next->head;

            x2->prev->next = x3->next;
            x3->next->prev = x2->prev;
            delete x2,x3;

            tail = next->tail;

            _curSize += next->_curSize;

            Block *tmp = next;
            tmp->next->prev = this;
            next = tmp->next;
            delete tmp;

        }  //merge this and next

        void Split(const int &k ) {
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

            news->_curSize = _curSize-k;
            _curSize = k;     //size
        }  //split into 1~k and k+1~curSize

        void Del() {
            Node *x = head->next;
            while (x != tail) {
                Node *tmp = x->next;
                delete x;
                x = tmp;
            }
            _curSize = 0;
        }

        ~Block() {
            Node *x = head->next;
            while (x != tail) {
                Node *tmp = x->next;
                delete x;
                x = tmp;
            }
            _curSize = 0;
            delete head,tail;
        }
    };



public:

	class const_iterator;

	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */


	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			//TODO
		}
		iterator operator-(const int &n) const {
			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			//TODO
		}
		iterator& operator+=(const int &n) {
			//TODO
		}
		iterator& operator-=(const int &n) {
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {}
		/**
		 * TODO *it
		 */
		T& operator*() const {}
		/**
		 * TODO it->field
		 */
		T* operator->() const noexcept {}
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
	}; //todo

	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
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


	/**
	 * TODO Constructors
	 */
	deque() {}
	deque(const deque &other) {}
	/**
	 * TODO Deconstructor
	 */
	~deque() {}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {}
	const T & at(const size_t &pos) const {}
	T & operator[](const size_t &pos) {}
	const T & operator[](const size_t &pos) const {}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {}
	const_iterator cend() const {}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {}
	/**
	 * returns the number of elements
	 */
	size_t size() const {}
	/**
	 * clears the contents
	 */
	void clear() {}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {}
};

}

#endif
