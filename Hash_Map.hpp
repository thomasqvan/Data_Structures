// Hash_Map.hpp
#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include <functional>
#include "Set.hpp"
#include "StringHashing.hpp"

template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    static constexpr unsigned int DEFAULT_CAPACITY = 10;
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    explicit HashSet(HashFunction hashFunction);
    ~HashSet() noexcept override;

    HashSet(const HashSet& s);
    HashSet(HashSet&& s) noexcept;

    HashSet& operator=(const HashSet& s);
    HashSet& operator=(HashSet&& s) noexcept;

    bool isImplemented() const noexcept override;
    void add(const ElementType& element) override;
    bool contains(const ElementType& element) const override;
    unsigned int size() const noexcept override;
    unsigned int elementsAtIndex(unsigned int index) const;
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

#endif // HASH_MAP_HPP
