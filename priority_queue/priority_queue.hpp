#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */

int flag=0;
int maxSize=0;
int totSize=0;

template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
    struct Node{
        T w;
        int h,lc,rc;
        bool operator <(const Node &b ){
            return Compare()(w,b.w);
        }
    };

    int curSize=0;
    int root=0;
    static Node *array;


    int merge(int A, int B){
        if(!A) return B;
        if(!B) return A;

        if(array[B] < array[A]) std::swap(A, B);

        if(array[B] < array[array[A].rc]) std::swap(array[A].rc, B);
        array[A].rc = merge(array[A].rc, B);

        if(array[array[A].lc].h < array[array[A].rc].h) {
            std::swap(array[A].lc, array[A].rc);
        }
        if(array[A].rc) array[A].h = array[array[A].rc].h + 1;
        else array[A].h = 0;

        return A;
    }

    void Doublespace(){
        Node *tmp =array;
        maxSize*=2;
        array= new Node [maxSize];
        for(int i=0;i<=totSize;++i) {
            array[i] = tmp[i];
        }
        delete [] tmp;
    }

    static int Node_init(T w) {
        array[++totSize].w = w;
        array[totSize].h = array[totSize].rc = 0;
        return totSize;
    }

    int insert(int A, T w){
        return merge(A, Node_init(w));
    }

    int del(int A){
        return merge(array[A].lc, array[A].rc);
    }

	priority_queue() {     //construct
	    array = new Node[100];
	    maxSize=100;
	}

	priority_queue(const priority_queue &other) {

    }  //todo

	~priority_queue() {
	    delete [] array;
	}

	priority_queue &operator=(const priority_queue &other) {
        curSize=other.curSize;
        root=other.root;
        return *this;
    }  //todo

	const T & top() const {
	    if(curSize==0)
	        throw container_is_empty();
        return array[root].w;
	}

	void push(const T &e) {
        int news=insert(root,e);
        root=news;
        curSize++;
	}

	void pop() {
        if(curSize==0)
            throw container_is_empty();
        root=del(root);
        curSize--;
    }

	size_t size() const {
        return size_t(curSize);
	}  //ok

	bool empty() const {
        return curSize==0;
	}  //ok

	void merge(priority_queue &other) {
        root=merge(root,other.root);
	}
};

}

#endif
