#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {


int flag=0;
int maxSize=0;
int totSize=0;

template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
    class Node{
    public:
        T w;  //value
        int h;  //distance
        Node *lc,*rc;   // leftson and rightson
        bool operator >(const Node &b ){
            return Compare()(b.w,w);
        }
        explicit Node(const T &x): lc(nullptr),rc(nullptr),w(x),h(0){} //init Node
        Node():lc(nullptr),rc(nullptr),h(0){} //init Node

        static Node* merge(Node *A, Node *B){  //just merge
            if(A== nullptr) return B;
            if(B== nullptr) return A;

            if(*B > *A) std::swap(A, B);  //ensure A>B

            // if(*B < *(A->rc)) std::swap(A->rc, B);  //I suddenly realize it is not necessary
            // BTW, it may cause bugs when A->rc refer to nullptr

            A->rc = merge(A->rc, B);

            if(A->lc!= nullptr && A->rc!= nullptr && A->lc->h < A->rc->h) {
                std::swap(A->lc, A->rc);
            }
            if(A->lc == nullptr && A->rc!= nullptr) //attention the stuation when nullptr->RE!!!
                std::swap(A->lc,A->rc);

            if(A->rc!= nullptr) A->h = (A->rc->h) + 1;
            else A->h = 0;

            return A;
        }

        static Node* CopyNode(const Node *x){   //copy a Node(tree) //mainly to construct a new tree
            if(x== nullptr) return nullptr;
            Node* news = new Node(x->w);
            news->h=x->h;
            news->lc=CopyNode(x->lc);
            news->rc=CopyNode(x->rc);
            return news;
        }

        static void DelNode(const Node*x){
            if(x== nullptr) return;
            DelNode(x->lc);
            DelNode(x->rc);
            delete x;
        }

    };

    Node *root;
    size_t curSize;

	priority_queue() {     //construct
	    root= nullptr;
	    curSize=0;
	}

	priority_queue(const priority_queue &other) {
        curSize=other.curSize;
        root=Node::CopyNode(other.root);
    }

	~priority_queue() {
	    Node::DelNode(root);
	}

	priority_queue &operator=(const priority_queue &other) {   //pay attention to it  //Don't forget the format in the book
	    if(this==&other){
	        return *this;
	    }
	    curSize=other.curSize;
	    Node::DelNode(root);
	    root=Node::CopyNode(other.root);
        return *this;
    }

	const T & top() const {
	    if(empty())
	        throw container_is_empty();
        return root->w;
	}

	void push(const T &e) {
        root=Node::merge(root,new Node(e));
        curSize++;
	}

	void pop() {
        if(empty())
            throw container_is_empty();
        Node *tmp=root;
        root=Node::merge(root->lc,root->rc);
        delete tmp;  //attention! just delete, do not DelNode
        curSize--;
    }

	size_t size() const {
        return curSize;
	}

	bool empty() const {
        return curSize==0;
	}

	void merge(priority_queue &other) {
        root=merge(root,other.root);
	}
};

}

#endif
