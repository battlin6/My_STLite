#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include <regex>
#include "utility.hpp"
#include "exceptions.hpp"
//using namespace sjtu;   //attention don't add it! cause ambiguous!

namespace sjtu {
    template<class Key,class T,class Compare = std::less<Key>>
              class map {
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
	}  //define a swap function

private:
    enum ColorType{Red,Black};
    struct Node{
        Node *prev,*next;
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
	    friend void map::erase(iterator);
	    friend const_iterator;
	private:
		map *nowMap;
		Node *nowNode;
	public:
		iterator():nowMap(nullptr),nowNode(nullptr){}
		iterator(map *a1, Node *a2):nowMap(a1),nowNode(a2){};
		iterator(const iterator &other)= default;
		iterator &operator=(const iterator &other)= default;
		operator const_iterator(){
		    return const_iterator(*this);
		}

		iterator operator++(int) {
		    iterator now(*this);
		    operator++();
		    return now;
		}

		iterator & operator++() {
		    if(nowNode== nullptr||nowNode==nowMap->tail) throw invalid_iterator();
		    nowNode=nowNode->next;
		    return *this;
		}

		iterator operator--(int) {
            iterator now(*this);
            operator--();
            return now;
		}

		iterator & operator--() {
            if(nowNode== nullptr||nowNode==nowMap->head->next) throw invalid_iterator();
            nowNode=nowNode->prev;
            return *this;
		}

		value_type & operator*() const {
		    return *operator->();
		}

		bool operator==(const iterator &rhs) const {
		    return nowNode==rhs.nowNode;
		}
		bool operator==(const const_iterator &rhs) const {
            return nowNode==rhs.nowNode;
		}

		bool operator!=(const iterator &rhs) const {
            return nowNode!=rhs.nowNode;
		}
		bool operator!=(const const_iterator &rhs) const {
            return nowNode!=rhs.nowNode;
		}
		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
		    //if(nowNode== nullptr||nowNode==nowMap->tail) throw invalid_iterator();
		    return nowNode->value;
		}
	};
	class const_iterator {
	    friend iterator;
		private:
			const map *nowMap;
            const Node *nowNode;
		public:
			const_iterator():nowMap(nullptr),nowNode(nullptr){}
            const_iterator(const map* a1,const Node *a2):nowMap(a1),nowNode(a2){}
			const_iterator(const const_iterator &other) = default;
			explicit const_iterator(const iterator &other):nowMap(other.nowMap),nowNode(other.nowNode){}
            const_iterator &operator=(const const_iterator &other) = default;

        const_iterator operator++(int) {
            const_iterator now(*this);
            operator++();
            return now;
        }

        const_iterator & operator++() {
            if(nowNode== nullptr||nowNode==nowMap->tail) throw invalid_iterator();
            nowNode=nowNode->next;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator now(*this);
            operator--();
            return now;
        }

        const_iterator & operator--() {
            if(nowNode== nullptr||nowNode==nowMap->head->next) throw invalid_iterator();
            nowNode=nowNode->prev;
            return *this;
        }

        value_type & operator*() const {
            return *operator->();
        }

        bool operator==(const iterator &rhs) const {
            return nowNode==rhs.nowNode;
        }
        bool operator==(const const_iterator &rhs) const {
            return nowNode==rhs.nowNode;
        }

        bool operator!=(const iterator &rhs) const {
            return nowNode!=rhs.nowNode;
        }
        bool operator!=(const const_iterator &rhs) const {
            return nowNode!=rhs.nowNode;
        }

        value_type* operator->() const noexcept {
            //if(nowNode== nullptr||nowNode==nowMap->tail) throw invalid_iterator();
            return nowNode->value;
        }
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
        if (a->fa == a) a->fa = b;
        if (a->child[0] == a) a->child[0] = b;
        if (a->child[1] == a) a->child[1] = b;

        if (b->prev == b) b->prev = a;
        if (b->next == b) b->next = a;
        if (b->fa == b) b->fa = a;
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
                x->color=Black;
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
        Rotate(fa,x->ok);
    }

    template <class O>
    pair<Node *,bool> Insert(const Key &key,const O &value){
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
            cas = compare_func(curNode->value->first,key);
            if(!cas && !compare_func(key,curNode->value->first)) return {curNode,false};
            if(curNode->child[cas]== nullptr) break;
            curNode=curNode->child[cas];
        }

        Node *newNode = new Node(value,cas?curNode:curNode->prev,cas?curNode->next:curNode,curNode, nullptr, nullptr,cas);
        curSize++;
        up(newNode);
        return {newNode,true};
    }

    pair<Node*,bool> Insert(const value_type &value){
        return Insert(value.first,value);
    }

    Node* Insert(const Key &key){
        return Insert(key,key).first;
    }

    void Erase(Node *x){
        curSize--;
        if(x->child[0]!= nullptr && x->child[1]!= nullptr){
            Swap_Node(x,x->next);
        }   //x->next must be a leave
        down(x);
        link(x->prev,x->next);

        Node *child=x->child[x->child[0]== nullptr];
        if(x==root) root=child;
        else x->fa->child[x->ok]=child;

        if(child!= nullptr){
            child->fa=x->fa;
            child->ok=x->ok;
        }

        delete x;
    }

public:
	map():head(new Node),tail(new Node),root(nullptr),curSize(0){
        head->next=tail;
        tail->prev=head;
        }
	map(const map &other):map(){
        if(other.curSize==0) return;
        curSize=other.curSize;
        root= new Node(other.root, nullptr,head,tail);
    }

	map & operator=(const map &other) {
        if(this==&other) return *this;
        clear();
        curSize=other.curSize;
        if(other.curSize==0) return *this;
        root= new Node (other.root, nullptr,head,tail);
        return *this;
    }

	~map() {
        clear();
        delete head;
        delete tail;
    }

	T & at(const Key &key) {
	    iterator now= find(key);
	    if(now==end()) throw index_out_of_bound();
	    return now->second;
	}
	const T & at(const Key &key) const {
        const_iterator now= find(key);
        if(now==cend()) throw index_out_of_bound();
        return now->second;
    }
	/**
	 * access specified element
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
	    return Insert(key)->value->second;
	}

	const T & operator[](const Key &key) const {
	    return at(key);
	}

	iterator begin() {
	    return iterator(this,head->next);
	}
	const_iterator cbegin() const {
	    return const_iterator(this,head->next);
	}
	iterator end() {
	    return iterator(this,tail);
	}
	const_iterator cend() const {
	    return const_iterator(this,tail);
	}

	bool empty() const {
	    return curSize==0;
	}

	size_t size() const {
	    return curSize;
	}

	void clear() {
	    curSize=0;
	    Node * curNode =head->next;
	    while(curNode!=tail){
	        Node *tmp=curNode;
	        curNode=curNode->next;
	        delete tmp;
	    }
	    head->next=tail;
	    tail->prev=head;
	    root= nullptr;
	}

	pair<iterator, bool> insert(const value_type &value) {
	    auto result=Insert(value);
	    return {iterator(this,result.first),result.second};
	}

	void erase(iterator pos) {
	    if(pos.nowMap!=this||pos==end()) throw invalid_iterator();
	    Erase(pos.nowNode);
	}

	size_t count(const Key &key) const {
	    return find(key)==cend()?0:1;
	}

	iterator find(const Key &key) {
	    Node* curNode=root;
	    while(curNode!= nullptr){
	        if(compare_func(key,curNode->value->first))
	            curNode=curNode->child[0];
	        else if(compare_func(curNode->value->first,key))
	            curNode=curNode->child[1];
	        else
	            return iterator(this,curNode);
	    }
	    return end();
	}
	const_iterator find(const Key &key) const {
        Node* curNode=root;
        while(curNode!= nullptr){
            if(compare_func(key,curNode->value->first))
                curNode=curNode->child[0];
            else if(compare_func(curNode->value->first,key))
                curNode=curNode->child[1];
            else
                return const_iterator(this,curNode);
        }
        return cend();
	}
};

}

#endif
