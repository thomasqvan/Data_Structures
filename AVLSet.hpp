// AVLSet.hpp
#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include<algorithm>

template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    explicit AVLSet(bool shouldBalance = true);

    ~AVLSet() noexcept override;

    AVLSet(const AVLSet& s);

    AVLSet(AVLSet&& s) noexcept;

    AVLSet& operator=(const AVLSet& s);

    AVLSet& operator=(AVLSet&& s) noexcept;

    bool isImplemented() const noexcept override;

    void add(const ElementType& element) override;
    
    bool contains(const ElementType& element) const override;

    unsigned int size() const noexcept override;

    int height() const noexcept;
    
    void preorder(VisitFunction visit) const;
    void inorder(VisitFunction visit) const;
    void postorder(VisitFunction visit) const;

private:
    
    struct Node {
    
        ElementType value;
        Node* left;
        Node* right;
    };
    
    Node* root;
    unsigned int sz;
    bool isBalancing;

    void destroyAll(Node* root) noexcept;
    Node* copyAll(Node* node_);

    int height_(Node* node_) const;
    bool contains_(Node* node_, const ElementType& element) const;
    
    void insert(Node*& node_, const ElementType& element);
    void checkBalance(Node*& node_, const ElementType& element);
    void llRotation(Node*& node_);
    void rrRotation(Node*& node_);

    void preorder_(Node* node_, VisitFunction visit) const;
    void inorder_(Node* node_, VisitFunction visit) const;
    void postorder_(Node* node_, VisitFunction visit) const;
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
    :root{ nullptr }, sz{ 0 }, isBalancing{ shouldBalance }
{
}


template <typename ElementType>
void AVLSet<ElementType>::destroyAll(Node* node_) noexcept {

    if(node_ != nullptr) {
        
        destroyAll(node_->left);
        destroyAll(node_->right);
        delete node_;
    }
    
    sz = 0;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    destroyAll(root);
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::copyAll(Node* node_)
{
    Node* newRoot = nullptr;
    
    if(node_ == nullptr) {
        
        return newRoot;
    }
    else { 
        
        try{
        
            newRoot = new Node{ node_->value, nullptr, nullptr };
            newRoot->left = copyAll(node_->left);
            newRoot->right = copyAll(node_->right);
        
            return newRoot;
        }
        catch(...) {
    
            destroyAll(newRoot);
            throw;
        }
    }
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
    : root{ nullptr }, sz{ s.sz }, isBalancing{ s.isBalancing }
{
    root = copyAll(s.root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
    : root{ nullptr }, sz{ s.sz }, isBalancing{ s.isBalancing }
{
    std::swap(root, s.root);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s) {
        
        destroyAll(root);
        root = copyAll(s.root);
        
        sz = s.sz;
        isBalancing = s.isBalancing;
    }

    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if(this != &s) {
        
        std::swap(root, s.root);
        std::swap(sz, s.sz);
        std::swap(isBalancing, s.isBalancing);
    }

    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::rrRotation(Node*& node_)
{
    Node* temp = node_->right;
    node_->right = temp->left;
    temp->left = node_;
    
    node_ = temp;
}


template <typename ElementType>
void AVLSet<ElementType>::llRotation(Node*& node_)
{
    Node* temp = node_->left;
    node_->left = temp->right;
    temp->right = node_;
    
    node_ = temp;
}


template <typename ElementType> 
void AVLSet<ElementType>::checkBalance(Node*& node_, 
    const ElementType& element)
{
    if(node_ != nullptr) {

        int balance = height_(node_->left) - height_(node_->right);
        
        if(balance < -1) {
            
            if(element > node_->right->value) {
           
                rrRotation(node_);
            }
            else if(element < node_->right->value) {
                
                llRotation(node_->right);
                rrRotation(node_);
            }
        }
        else if(balance > 1) {
            
            if(element < node_->left->value) {
                
                llRotation(node_);
            }
            else if(element > node_->left->value) {
                
                rrRotation(node_->left);
                llRotation(node_);
            }
        }
    }
}


template <typename ElementType>
void AVLSet<ElementType>::insert(Node*& node_, const ElementType& element)
{
    if(node_ == nullptr) {
        
        try {
            
            node_ = new Node{element, nullptr, nullptr};
            sz++;
        }
        catch(...) {
            
            destroyAll(root);
            throw;
        }
    }
    else if(element < node_->value) {
        
        insert(node_->left, element);  
    }
    else if(element > node_->value) {
        
        insert(node_->right, element);
    }
    else if(element == node_->value) {
        
        return;
    }

    if(isBalancing == true) {
        
        checkBalance(node_, element);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    insert(root, element);
}


template <typename ElementType>
bool AVLSet<ElementType>::contains_(Node* node_, 
    const ElementType& element) const
{
    if(node_ != nullptr) {
        
        if(node_->value == element) {
            
            return true;
        }
        else if(element < node_->value) {
            
            return contains_(node_->left, element);
        }
        else {
            
            return contains_(node_->right, element);
        }
    }
    else {
        
        return false;
    }
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return contains_(root, element);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
int AVLSet<ElementType>::height_(Node* node_) const
{    
    if(node_ == nullptr) {
    
        return -1;
    }
    
    int left_subtree = height_(node_->left);
    int right_subtree = height_(node_->right);
    
    return std::max(left_subtree, right_subtree) + 1;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const noexcept
{
    return height_(root);
}


template <typename ElementType>
void AVLSet<ElementType>::preorder_(Node* node_, VisitFunction visit) const
{
    if(node_ == nullptr) {

        return;
    }

    Node* left_subtree = node_->left;
    Node* right_subtree = node_->right;

    visit(node_->value);
    preorder_(left_subtree, visit);
    preorder_(right_subtree, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
   preorder_(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder_(Node* node_, VisitFunction visit) const
{
    if(node_ == nullptr) {
        
        return;
    }

    Node* left_subtree = node_->left;
    Node* right_subtree = node_->right;

    inorder_(left_subtree, visit);
    visit(node_->value);
    inorder_(right_subtree, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorder_(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder_(Node* node_, VisitFunction visit) const
{
    if(node_ == nullptr) {
        
        return;
    }

    Node* left_subtree = node_->left;
    Node* right_subtree = node_->right;

    postorder_(left_subtree, visit);
    postorder_(right_subtree, visit);
    visit(node_->value);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorder_(root, visit);
}

#endif // AVLSET_HPP
