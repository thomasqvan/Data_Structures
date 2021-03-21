// HashSet.hpp
//
// ICS 46 Winter 2021
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"

#include "StringHashing.hpp"


template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;

private:
    HashFunction hashFunction;
    
    struct Node {

        ElementType value;
        Node* next;
    };

    Node** hashTable;
    unsigned int sz, capacity;

    double loadFactor() const;
    void destroyAll() noexcept;
};

namespace {
    
    unsigned int max_bucketCount = 0;
}

namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{ hashFunction },
      hashTable{ nullptr }, sz{ 0 }, capacity{ DEFAULT_CAPACITY }
{
    hashTable = new Node*[DEFAULT_CAPACITY];
    this->hashFunction = hashFunction;

    for(unsigned int i = 0; i < capacity; ++i) {
        
        hashTable[i] = nullptr;
    }
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    if(hashTable != nullptr) {
    
        for(unsigned int i = 0; i < capacity; ++i) {

            Node* head = hashTable[i];
            Node* current = head;

            while(current != nullptr) {
            
                head = current;
                current = current->next;
                delete head;
            }
        }
    }
    
    delete[] hashTable;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction},
      hashTable{ nullptr }, sz{ s.sz }, capacity{ s.capacity }
{
    Node** newhashTable = new Node*[capacity];
    for(unsigned int i = 0; i < s.capacity; ++i) {
        
        Node* originalHash = s.hashTable[i];
        Node* newHash = nullptr;

        try {
            
            while(originalHash != nullptr) {
                
                newHash = new Node{originalHash->value, newHash};
                originalHash = originalHash->next;
            }
            
            newhashTable[i] = newHash;
        }
        catch(...) {
            
            destroyAll();
            throw;
        }
    }
    
    hashTable = newhashTable;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{ s.hashFunction },
      hashTable{ nullptr }, sz{ 0 }, capacity{ DEFAULT_CAPACITY }
{
    std::swap(hashTable, s.hashTable);
    std::swap(sz, s.sz);
    std::swap(capacity, s.capacity);
}


template <typename ElementType>
void HashSet<ElementType>::destroyAll() noexcept
{
    for(unsigned int i = 0; i < capacity; ++i) {

        Node* head = hashTable[i];
        Node* current = head;

        while(current != nullptr) {
            
            head = current;
            current = current->next;
            delete head;
            sz--;
        }

        hashTable[i] = nullptr;
        max_bucketCount = 0;
    }
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if(this != &s) {
        
        destroyAll();

        Node** newHashTable = new Node*[s.capacity];
        for(unsigned int i = 0; i < s.capacity; ++i) {
        
            Node* originalHash = s.hashTable[i];
            Node* newHash = nullptr;

            try {
            
                while(originalHash != nullptr) {
                
                    newHash = new Node{originalHash->value, newHash};
                    originalHash = originalHash->next;
                }
            
                newHashTable[i] = newHash;
            }
            catch(...) {
            
                destroyAll();
                throw;
            }
        }
        
        sz = s.sz;
        capacity = s.capacity;
        
        delete[] hashTable;
        hashTable = newHashTable;
    }

    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if(this != &s) {
        
        std::swap(hashTable, s.hashTable);
        std::swap(sz, s.sz);
        std::swap(capacity, s.capacity);
    }

    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
double HashSet<ElementType>::loadFactor() const {

    return static_cast<double>(sz) / capacity;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    
    if(loadFactor() > 0.8) {
        
        unsigned int newCapacity = capacity * 2 + 1;
        Node** newHashTable = new Node*[newCapacity]();

        for(unsigned int i = 0; i < capacity; ++i) {
            
            Node* head = hashTable[i];
            while(head != nullptr) {
                
                Node* current = head;
                head = head->next;

                unsigned int index = hashFunction(element) % newCapacity;

                Node*& newHash = newHashTable[index];

                current->next = newHash;
                newHash = current;
            }
        }
        
        capacity = newCapacity;
        
        delete[] hashTable;
        hashTable = newHashTable;
    }

    unsigned int index = hashFunction(element) % capacity;
    
    if(contains(element) == false) {
        
        Node* head = hashTable[index];
        Node* current = head;

        if(head == nullptr) {
            
            current = new Node;
            current->value = element;
            current->next = nullptr;

            hashTable[index] = current;
        }
        else {
            
            current = new Node;
            current->value = element;

            current->next = head;
            hashTable[index] = current;
        }

        sz++;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    
    unsigned int index = hashFunction(element) % capacity;

    Node* find = hashTable[index];

    while(find != nullptr) {

        if(find->value == element) {
            
            return true;
        }

        find = find->next;
    }
    
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    if(index >= 0 && index < capacity) {
    
        unsigned int temp = 0;
        Node* current = hashTable[index];

        while(current != nullptr) {
        
            temp++;
            current = current->next;
        }

        return temp;
    }
    else {
        
        return 0;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, 
    unsigned int index) const
{
    if(index >= 0 || index < capacity) {
        
        Node* current = hashTable[index];
        
        while(current != nullptr) {
        
            if(current->value == element) {
                
                return true;
            }
            else {
                
                current = current->next;
            }
        }        
    }
    
    return false;
}

#endif // HASHSET_HPP
