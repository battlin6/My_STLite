/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <regex>
#include "utility.hpp"
#include "exceptions.hpp"
using namespace sjtu;

namespace sjtu {

template<class Key,class T,class Compare = std::less<Key>> class map {
public:
    /*
     * use RB-Tree to complete map
     * */
	typedef pair<const Key, T> value_type;

	template <class U>
	void swap(U &a,U &b){
	    U temp=a;
	    a=b;
	    b=temp;
	}  //define a

private:
    enum ColorType{Red,Black};
    struct Node{
        Node *prev,next;
        Node *fa,*child[2];
        ColorType color;
        value_type *value;
        bool ok;  //ok==0 is left child, ok==1 is right child

        Node():value(nullptr){}
        Node(Node *other, Node* fa,Node *prev,Node *next):
            value(new value_type(*(other->value))),fa(fa),
            prev(prev),next(next),ok(other->ok),color(other->color){
            child[0]=child[1]= nullptr;
            prev->next=next->prev=this;
            if(other->child[0]!= nullptr)
                child[0]= new Node(other->child[0],this,prev,this);
            //else child[0]= nullptr;
            if(other->child[1]!= nullptr)
                child[1]= new Node(other->child[1],this,this,next);
            //else child[1]= nullptr;
        }
        Node(const value_type &value,Node *prev,Node* next,Node *fa,Node *lc,Node *rc,bool ok)
            :value(new value_type(value)),prev(prev),next(next),fa(fa),ok(ok),color(Red){
            child[0]=lc,child[1]=rc;
            complish();
        }
        Node(const Key &key,Node *prev,Node *next,Node* fa,Node *lc,Node* rc,bool ok)
            :Node(value_type(key,T()),prev,next,fa,lc,rc,ok){}

        void complish(){
            next->prev = prev->next = this;
            if(fa!= nullptr) fa->child[ok]=this;
            if(child[0]!= nullptr){
                child[0]->fa=this;
                child[0]->ok=0;
            }
            if(child[1]!= nullptr){
                child[1]->fa=this;
                child[1]->ok=1;
            }
        }
        Node *brother(){
            return fa->child[!ok];
        }
        ~Node(){
            delete value;
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
		iterator() {
			// TODO
		}
		iterator(const iterator &other) {
			// TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {}
		bool operator==(const iterator &rhs) const {}
		bool operator==(const const_iterator &rhs) const {}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {}
		bool operator!=(const const_iterator &rhs) const {}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {}
	};
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
	};

private:
    Node *head,*tail, *root;
    size_t curSize;
    Compare compare_func;

    static void link(Node *a,Node *b){
        a->next=b;
        b->prev=a;
    } //link the prev and next;

    void Swap_Node(Node* a,Node* b){
        if(root==a)
            root=b;
        else if(root==b)
            root=a;

        swap(a->prev,b->prev);
        swap(a->next,b->next);
        swap(a->fa,b->fa);
        swap(a->child[0],b->child[0]);
        swap(a->child[1],b->child[1]);
        swap(a->color,b->color);
        swap(a->ok,b->ok);

        // problem?? if a is close to b
        if (a->prev == a) a->prev = b;
        if (a->next == a) a->next = b;
        if (a->father == a) a->father = b;
        if (a->child[0] == a) a->child[0] = b;
        if (a->child[1] == a) a->child[1] = b;

        if (b->prev == b) b->prev = a;
        if (b->next == b) b->next = a;
        if (b->father == b) b->father = a;
        if (b->child[0] == b) b->child[0] = a;
        if (b->child[1] == b) b->child[1] = a;


        a->complish(),b->complish();
    }//swap two Node in a map

    void Rotate(Node *x, bool ok){
        Node *y =x->child[!ok];
        if(root==x) root=y;

        x->child[!ok]=y->child[ok];
        y->fa=x->fa;
        y->ok=x->ok;
        x->fa=y;
        x->ok=ok;

        x->complish(),y->complish();
    }//rotate

    void up(Node *x){
        Node *fa=x->fa;
        if(fa== nullptr){
            x->color=Black;
            return;
        }

        if(fa->color==Black) return;

        Node *grandpa=fa->fa;
        Node *uncle=x->fa->brother();

        if(uncle== nullptr||uncle->color==Black){
            if(x->ok==fa->ok){
                fa->color=Black;
                grandpa->color=Red;
                Rotate(grandpa,!(x->ok));
            }else{
                x->fa=Black;
                grandpa->color=Red;
                Rotate(fa,!(x->ok));
                Rotate(grandpa,!(x->ok));
            }
        }else{
            fa->color=uncle->color =Black;
            grandpa->color=Red;
            up(grandpa);
        }
    }

    void down(Node *x, bool cas=false){
        if(x->color==Red && !cas) return;

        Node *child =x->child[x->child[0]== nullptr];   //get a non-nullptr

        if(child!= nullptr && child->color==Red && !cas){
            child->color=Black;
            return;
        }

        if(root==x){
            x->color=Black;
            return;
        }

        Node *fa=x->fa;
        Node *brother=x->brother();
        Node **cousin=brother->child;  //two side array

        if(fa->color==Black && brother->color ==Black &&
        (cousin[0] == nullptr || cousin[0]->color == Black) &&
        (cousin[1] == nullptr || cousin[1]->color == Black)){
            brother->color= Red;
            down(fa,true);
            return;
        }

        if(brother->color== Red){
            fa->color=Red;
            brother->color=Black;
            Rotate(fa,x->ok);
            brother=x->brother();
            cousin=brother->child;
        }

        if(fa->color==Red && brother->color==Black &&
        (cousin[0] == nullptr || cousin[0]->color == Black) &&
        (cousin[1] == nullptr || cousin[1]->color == Black)){
            fa->color=Black;
            brother->color=Red;
            return;
        }

        if(cousin[!(x->ok)]== nullptr || cousin[!(x->ok)]->color==Black){
            cousin[x->ok]->color=Black;
            brother->color=Red;
            Rotate(brother ,!(x->ok));
            brother=x->brother();
            cousin=brother->child;
        }

        swap(fa->color,brother->color);
        cousin[!(x->ok)]->color=Black;
        rotate(fa,x->ok);
    }

    pair<Node *,bool> insert(const Key &key,const T &value){
        if(root== nullptr){
            Node *newNode = new Node (value,head,tail, nullptr, nullptr, nullptr,0);
            root = newNode;
            curSize++;
            up(newNode);
            return {newNode,true};
        }

        Node *curNode=root;
        bool cas;
        while(true){
            cas = compare_func(curNode->value->first,key);  //todo
            if(!cas && )
        }
    }

public:


	/**
	 * TODO two constructors
	 */
	map() {}
	map(const map &other) {}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {}
	/**
	 * TODO Destructors
	 */
	~map() {}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {}
	const T & at(const Key &key) const {}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {}
	const_iterator cend() const {}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {}
	/**
	 * clears the contents
	 */
	void clear() {}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {}
	const_iterator find(const Key &key) const {}
};

}

#endif
