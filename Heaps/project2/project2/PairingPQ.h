#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent;
            // TODO: Add one extra pointer (parent or previous) as desired.
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
            this->compare = comp;
            root = NULL;
            size_heap = 0;
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
            this->compare = comp;
            root = NULL;
            while(start != end){
                push(*start);
                ++start;
            }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare } {
            this->compare = other.compare;
            root = NULL;
            if(other.root != NULL){
                other.root->parent = NULL;
                other.root->sibling = NULL;
                storage.push_back(other.root);
            }
            while(!storage.empty()){
                if(storage.front()->sibling != NULL){
                    storage.push_back(storage.front()->sibling);
                }
                if(storage.front()->child != NULL){
                    storage.push_back(storage.front()->child);
                }
                if(storage.front() != NULL){
                    push(storage.front()->getElt());
                }
                storage.pop_front();
            }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        PairingPQ temp = rhs;
        swap_nodes(temp);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        DeleteAll();
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        if(empty()){
            return;
        }
        if(root->sibling != NULL){
            storage.push_back(root->sibling);
            root->sibling = NULL;
        }
        if(root->child != NULL){
            storage.push_back(root->child);
            root->child = NULL;
        }
        root->parent = NULL;
        while(!storage.empty()){
            if(storage.front()->sibling != NULL){
                storage.push_back(storage.front()->sibling);
            }
            if(storage.front()->child != NULL){
                storage.push_back(storage.front()->child);
            }
            storage.front()->child = NULL;
            storage.front()->sibling = NULL;
            storage.front()->parent = NULL;
            root = meldRoots(root, storage.front());
            storage.pop_front();
        }
    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This has been provided for you,
    //              in that you should implement push functionality in the
    //              addNode function.
    // Runtime: Amortized O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        ++size_heap;
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        if(root->child == NULL){
            root->sibling = NULL;
            root->child = NULL;
            root->parent = NULL;
            delete root;
            root = NULL;
        }
        else if(size_heap == 2){
            Node* holder = root;
            root = root->child;
            holder->child = NULL;
            holder->sibling = NULL;
            
            delete holder;
        }
        else{
            Node* hold = root->child;
            root->child = NULL;
            root->sibling = NULL;
            hold->parent = NULL;
            delete root;
            root = NULL;
            storage.push_back(hold);
            hold = hold->sibling;
            while(hold != NULL){
                hold->parent = NULL;
                storage.push_back(hold);
                hold = hold->sibling;
            }

            while(storage.size() != 1){
                storage[0]->sibling = NULL;
                storage[1]->sibling = NULL;
                //Have queue - meld them then push them onto the back - in sets of two
                Node* merged_tree = meldRoots(storage[0], storage[1]);
                storage.pop_front();
                storage.pop_front();
                storage.push_back(merged_tree);
            }
            
            root = storage.front();
            
            storage.pop_front();
        }
        --size_heap;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return root->getElt();
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return size_heap;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return (root == NULL);
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        if(!(this->compare(node->getElt(), new_value))){
            return;
        }
        if(node == root){
            return;
        }
        node->elt = new_value;
        storage.push_front(root);
        if(node->parent->child == node){
            if(node->sibling != NULL){
                node->parent->child = node->sibling;
            }
        }
        else{
            while(storage.front()->sibling != node){
                if(storage.front()->sibling != NULL){
                    storage.push_back(storage.front()->sibling);
                }
                if(storage.front()->child != NULL){
                    storage.push_back(storage.front()->child);
                }
                storage.pop_front();
            }
            if(node->sibling != NULL){
                storage.front()->sibling = node->sibling;
            }
            else{
                storage.front()->sibling = NULL;
            }
        }
        node->sibling = NULL;
        node->parent = NULL;
        root = meldRoots(root, node);
        while(!storage.empty()){
            storage.pop_front();
        }
    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: Amortized O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    Node* addNode(const TYPE & val) {
        Node *new_node = new Node(val);
        if(root == NULL){
            root = new_node;
        }
        else{
            root = meldRoots(root, new_node);
        }
        return new_node;
    } // addNode()
    
    
    /*//Returns a pointer to a specific value
    virtual Node* testUpdateElt(TYPE value){
        std::deque<Node*> hold;
        hold.push_back(root);
        while(hold.front()->getElt() != value){
            if(hold.front()->sibling != NULL){
                hold.push_back(hold.front()->sibling);
            }
            if(hold.front()->child != NULL){
                hold.push_back(hold.front()->child);
            }
            hold.pop_front();
        }
        Node* save = hold.front();
        return save;
    }*/

private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
    //Meld(two node pointers), Copy Recursive helper(takes in node pointer and makes it in your class, checks if end)
    //Queue for member variable should be used for meld
    //Function that recursivly adds on every element starting from the root into the queue.
    //Queue uses first in and first out to update priorities
    Node* root;
    size_t size_heap = 0;
    std::deque<Node*> storage;
    
    virtual void swap_nodes(PairingPQ &other){
        std::swap(this->compare, other.compare);
        std::swap(root, other.root);
        std::swap(size_heap, other.size_heap);
    }
    
    virtual void clear(Node* node){
        if(node != NULL){
            clear(node->child);
            clear(node->sibling);
            node->parent = NULL;
            node->sibling = NULL;
            node->child = NULL;
            delete node;
            node = NULL;
        }
    }
    
    //Deletes all of the PQ
    //Should pass in the root
    virtual void DeleteAll(){
        clear(root);
        size_heap = 0;
        
       /* if(root == NULL){
            delete root;
            return;
        }
        else{
            storage.push_back(root);
            while(!storage.empty()){
                Node* hold = storage.front();
                storage.pop_front();
                if(hold->sibling != NULL){
                    storage.push_back(hold->sibling);
                }
                if(hold->child != NULL){
                    storage.push_back(hold->child);
                }
                hold->sibling = NULL;
                hold->sibling = NULL;
                hold->parent = NULL;
                delete hold;
            }
        }
        size_heap = 0;*/
    }
    
    virtual Node* meldRoots(Node* node_original, Node* new_value){
        //Attaching the original root/first value as the child of the new one
        //Returns true if the node_original is more of a priority
        if(new_value == NULL){
            return node_original;
        }
        if(!(this->compare(node_original->getElt(), new_value->getElt()))){
            if(node_original->child != NULL){
                new_value->sibling = node_original->child;
                node_original->child->parent = node_original;
            }
            node_original->child = new_value;
            new_value->parent = node_original;
            node_original->parent = NULL;
            node_original->sibling = NULL;
            return node_original;
        }
        else{
            if(new_value->child != NULL){
                node_original->sibling = new_value->child;
                new_value->child->parent = new_value;
            }
            new_value->child = node_original;
            node_original->parent = new_value;
            new_value->sibling = NULL;
            new_value->parent = NULL;
            return new_value;
        }
    }
    

};


#endif // PAIRINGPQ_H
