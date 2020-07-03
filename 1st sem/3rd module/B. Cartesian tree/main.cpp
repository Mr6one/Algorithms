#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

using namespace std;

struct BNode{
    int data;
    BNode* left;
    BNode* right;
    BNode (int a){
        data=a;
        left=nullptr;
        right=nullptr;
    }
};

struct CNode{
    int data;
    CNode* left;
    CNode* right;
    int priority;
    CNode (int a, int prior){
        data=a;
        priority=prior;
        left=nullptr;
        right=nullptr;
    }
};

class BinaryTree{
public:
    BinaryTree(){
        root=nullptr;
    }
    ~BinaryTree(){
        Delete(root);
    }

    void AddNode(int);
    int MaxHeight();
private:
    BNode* root;
    void BuildTree(vector<int>&);
    void Delete(BNode*);
    int SearchMaxHeight(BNode*);
};

class CartesianTree {
public:
    CartesianTree(){
        root=nullptr;
    }
    ~CartesianTree(){
        Delete(root);
    }

    void AddNode(int, int);
    int MaxHeight();
private:
    CNode* root;
    CNode* Merge(CNode*, CNode*);
    void Split(CNode*, CNode*&, CNode*&, int);
    void BuildCartesianTree(vector<int>&, vector<int>&);
    CNode* AddNode1(CNode*, int, int);
    int SearchMaxHeight(CNode*);
    void Delete(CNode*);
};

CNode* CartesianTree::Merge(CNode* Lroot, CNode* Rroot){
    if (Lroot==nullptr || Rroot==nullptr){
        if (Lroot==nullptr){
            return Rroot;
        } else {
            return Lroot;
        }
    }
    if (Lroot->priority>Rroot->priority){
        Lroot->right=Merge(Lroot->right,Rroot);
        return Lroot;
    } else {
        Rroot->left=Merge(Lroot,Rroot->left);
        return Rroot;
    }
}

void CartesianTree::Split(CNode* node, CNode*& Left, CNode*& Right, int value){
    if (node==nullptr){
        Left=nullptr;
        Right=nullptr;
    } else if (value<node->data){
        Split (node->left, Left, node->left, value);
        Right=node;
    } else {
        Split (node->right, node->right, Right, value);
        Left=node;
    }
}

void CartesianTree::AddNode(int value, int priority){
    root=AddNode1(root, value, priority);
}

CNode* CartesianTree::AddNode1(CNode* node, int value, int priority){
        CNode* Left; CNode* Right;
        Split (node, Left, Right, value);
        CNode* NewNode=new CNode(value, priority);
        return Merge(Merge(Left, NewNode),Right);
}

int CartesianTree::MaxHeight(){
    CNode* node=root;
    return SearchMaxHeight(node);
}

int CartesianTree::SearchMaxHeight(CNode* node){
    if (node==nullptr){
        return 0;
    }
    return max(SearchMaxHeight(node->left),SearchMaxHeight(node->right))+1;
}

void CartesianTree::Delete(CNode* root){
    if (root==nullptr){
        return;
    }
    Delete(root->left);
    Delete(root->right);
    delete root;
}



void BinaryTree::AddNode(int value){
    if (root==nullptr){
        root=new BNode(value);
        return;
    }
    BNode* node=root;
    while ((node->left!=nullptr || value>=node->data) && (node->right!=nullptr || value<node->data)){
        if (value<node->data){
            node=node->left;
        } else {
            node=node->right;
        }
    }
    BNode* NewNode=new BNode(value);
    if (value<node->data){
        node->left=NewNode;
    } else {
        node->right=NewNode;
    }
}

int BinaryTree::MaxHeight(){
    BNode* node=root;
    return SearchMaxHeight(node);
}

int BinaryTree::SearchMaxHeight(BNode* node){
    if (node==nullptr){
        return 0;
    }
    return max(SearchMaxHeight(node->left),SearchMaxHeight(node->right))+1;
}

void BinaryTree::Delete(BNode* root){
    if (root==nullptr){
        return;
    }
    Delete(root->left);
    Delete(root->right);
    delete root;
}

int main()
{
    int n;
    cin>>n;
    BinaryTree Btree;
    CartesianTree Ctree;
    int x,y;
    for (int i=0; i<n; ++i){
        cin>>x>>y;
        Btree.AddNode(x);
        Ctree.AddNode(x,y);
    }
    cout<<abs(Ctree.MaxHeight()-Btree.MaxHeight());
    return 0;
}
