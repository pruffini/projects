/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to test updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * These tests are NOT a complete test of your priority queues!
 */

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "BinaryPQ.h"
#include "Eecs281PQ.h"
#include "UnorderedPQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"

using namespace std;


// Used to test a priority queue containing pointers to integers.
struct IntPtrComp {
    bool operator() (const int *a, const int  *b) const {
        return *a < *b;
    }
};


// TODO: Make sure that you're using this-compare() properly, and everywhere
// that you should.  Complete this function by adding a functor that compares
// two HiddenData structures, create a PQ of the specified type, and call
// this function from main().
void testHiddenData(const string &pqType) {
    struct HiddenData {
        int data;
    };
    struct HiddenDataComp {
        bool operator()(const HiddenData &a, const HiddenData &b) {
            return a.data < b.data;
            return false;
        }
    };

    cout << "Testing " << pqType << " with hidden data\n";
} // testHiddenData()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq) {
    vector<int> data{ 1, 5 };
    pq->push(&data[0]);
    pq->push(&data[1]);
    data[0] = 10;
    pq->updatePriorities();
    assert(*pq->top() == 10);
} // testUpdatePrioritiesHelper()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePriorities(const string &pqType) {
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on " << pqType << endl;
    if (pqType == "Unordered") {
        pq = new UnorderedPQ<int *, IntPtrComp>;
    }
    else if(pqType == "Sorted"){
        pq = new SortedPQ<int *, IntPtrComp>;
    }
    else if(pqType == "Binary"){
        pq = new BinaryPQ<int *, IntPtrComp>;
    }
    else if(pqType == "Pairing"){
        pq = new PairingPQ<int *, IntPtrComp>;
    }

    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType) {
    cout << "Testing priority queue: " << pqType << endl;

    pq->push(3);
    pq->push(4);
    assert(pq->size() == 2);
    assert(pq->top() == 4);

    pq->pop();
    assert(pq->size() == 1);
    assert(pq->top() == 3);
    assert(!pq->empty());

    pq->pop();
    assert(pq->size() == 0);
    assert(pq->empty());

    // TODO: Add more testing here!

    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()


// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().  Still not complete, should have more code, test
// addNode(), updateElt(), etc.
void testPairing(vector<int> & vec) {
    cout << "Testing Pairing Heap separately" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    // This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    *pq3 = *((PairingPQ<int> *)pq2);
    pq1->push(3);
    pq2->pop();
    assert(pq1->size() == 3);
    assert(!pq1->empty());
    assert(pq1->top() == 3);
    pq2->push(pq3->top());
    assert(pq2->top() == pq3->top());

    cout << "Basic tests done, calling destructors" << endl;

    delete pq1;
    delete pq2;
    delete pq3;

    cout << "testPairing() succeeded" << endl;
} // testPairing()

void testCustom(vector<int> & vec){
    
    //Can pass copy and fail assignment
    
    cout << "Custom Test" << endl;
    Eecs281PQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    Eecs281PQ<int> * pq2 = new PairingPQ<int>(*((PairingPQ<int> *)pq1)); //Copy
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    *pq3 = *((PairingPQ<int> *)pq2); //Assignment
    assert(pq1->top() == pq2->top());
    assert(pq2->top() == pq3->top());
    assert(pq1->top() == pq3->top());
    assert(pq1->size() == pq2->size());
    assert(pq2->size() == pq3->size());
    assert(pq1->size() == pq3->size());
    //P1: 45,44,6,3,1,0
    pq1->pop();
    //P1: 44,6,3,1,0
    pq2->pop();
    pq3->pop();
    assert(pq1->top() == pq2->top());
    assert(pq2->top() == pq3->top());
    assert(pq1->top() == pq3->top());
    assert(pq1->size() == pq3->size());
    assert(pq1->size() == pq2->size());
    assert(pq2->size() == pq3->size());
    //P1: 44,6,3,1,0
    pq1->pop();
    //P1: 6,3,1,0
    pq2->pop();
    pq3->pop();
    assert(pq1->top() == pq2->top());
    assert(pq2->top() == pq3->top());
    assert(pq1->size() == pq2->size());
    assert(pq2->size() == pq3->size());
    assert(pq1->top() == pq3->top());
    assert(pq1->size() == pq3->size());
    //P1: 6,3,1,0
    pq1->pop();
    //P1: 3,1,0
    pq2->pop();
    pq3->pop();
    assert(pq1->top() == pq2->top());
    assert(pq2->top() == pq3->top());
    assert(pq1->size() == pq2->size());
    assert(pq2->size() == pq3->size());
    assert(pq1->top() == pq3->top());
    assert(pq1->size() == pq3->size());
    //P1: 3,1,0
    pq1->push(78);
    //P1: 78,3,1,0
    pq2->push(78);
    pq3->push(78);
    assert(pq1->top() == pq2->top());
    assert(pq2->top() == pq3->top());
    assert(pq1->size() == pq2->size());
    assert(pq2->size() == pq3->size());
    assert(pq1->top() == pq3->top());
    assert(pq1->size() == pq3->size());
    //P1: 78,3,1,0
    pq1->push(2);
    //P1: 78,2,3,1,0
    pq2->push(2);
    pq3->push(2);
    assert(pq1->top() == pq2->top());
    assert(pq2->top() == pq3->top());
    assert(pq1->size() == pq2->size());
    assert(pq2->size() == pq3->size());
    assert(pq1->top() == pq3->top());
    assert(pq1->size() == pq3->size());
    //P1: 78,2,3,1,0
    pq1->pop();
    //P1: 3,2,1,0
    pq2->pop();
    pq3->pop();
    //P1: 3,2,1,0
    pq1->pop();
    //P1: 2,1,0
    pq2->pop();
    pq3->pop();
    //P1: 2,1,0
    pq1->pop();
    //P1: 1,0
    pq2->pop();
    pq3->pop();
    //P1: 1,0
    pq1->pop();
    //P1: 0
    pq2->pop();
    pq3->pop();
    //P1: 0
    pq1->pop();
    //P1: 
    pq2->pop();
    pq3->pop();
    assert(pq1->empty());
    assert(pq2->empty());
    assert(pq3->empty());
    
    Eecs281PQ<int> * pq4 = new PairingPQ<int>(*((PairingPQ<int> *)pq1));
    assert(pq4->size() == pq1->size());

    cout << "Custom Testing Complete" << endl;
    
    delete pq1;
    delete pq2;
    delete pq3;
    delete pq4;

}

/*void testUpdateElt(){
    cout << "Testing UpdateElt" << endl;
    
    PairingPQ<int> * philip = new PairingPQ<int>();
    philip->push(25);
    philip->push(15);
    philip->push(16);
    philip->push(17);
    philip->push(14);
    philip->push(28);
    philip->push(23);
    philip->push(32);
    
    philip->updateElt(philip->testUpdateElt(25), 26);
    
    
    delete philip;
}*/


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{ "Unordered", "Sorted", "Binary", "Pairing" };
    int choice;

    cout << "PQ tester" << endl << endl;
    for (size_t i = 0; i < types.size(); ++i)
        cout << "  " << i << ") " << types[i] << endl;
    cout << endl;
    cout << "Select one: ";
    cin >> choice;

    if (choice == 0) {
        pq = new UnorderedPQ<int>;
    } // if
    else if (choice == 1) {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2) {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3) {
        pq = new PairingPQ<int>;
    } // else if
    else {
        cout << "Unknown container!" << endl << endl;
        exit(1);
    } // else
   
    testPriorityQueue(pq, types[choice]);
    testUpdatePriorities(types[choice]);

    if (choice == 3) {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        
        vector<int> vec1;
        vec1.push_back(3);
        vec1.push_back(0);
        vec1.push_back(44);
        vec1.push_back(6);
        vec1.push_back(45);
        vec1.push_back(1);
        
        testPairing(vec);
        testCustom(vec1);
        
       // testUpdateElt();
        
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()
