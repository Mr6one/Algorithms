#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Node{
    Node* left;
    Node* right;
    int data;
    int height;
    int Size;
    Node(int value){
        data=value;
        left=nullptr;
        right=nullptr;
        height=0;
        Size=1;
    }
};

class AVLTree{
public:
    AVLTree(){
        root=nullptr;
    }
    ~AVLTree(){
        Destroy(root);
    }

    void Add(int, int&);
    void Delete(int);
private:
    Node* root;
    void SizeSet(Node*);
    int Height(Node*);
    void HeightSet(Node*);
    Node* LeftRotate(Node*);
    Node* RightRotate(Node*);
    Node* Balance(Node*);
    Node* Add1(Node*, int, int&);
    Node* FindMin(Node*);
    Node* DeleteMin(Node*);
    Node* Delete1(Node*, int);
    void Destroy(Node*);
};

int AVLTree::Height(Node* node){
    if (node==nullptr){
        return 0;
    } else {
        return node->height;
    }
}

void AVLTree::HeightSet(Node* node){
    int h1=Height(node->left);
    int h2=Height(node->right);
    if (h1>h2){
        node->height=h1+1;
    } else {
        node->height=h2+1;
    }
}

void AVLTree::SizeSet(Node* node){
    int sizeL, sizeR;
    if (node->left==nullptr){
        sizeL=0;
    } else {
        sizeL=node->left->Size;
    }
    if (node->right==nullptr){
        sizeR=0;
    } else {
        sizeR=node->right->Size;
    }
    node->Size=sizeL+sizeR+1;
}

Node* AVLTree::LeftRotate(Node* node){
    Node* node2=node->right;
    node->right=node2->left;
    node2->left=node;
    SizeSet(node);
    SizeSet(node2);
    HeightSet(node);
    HeightSet(node2);
    return node2;
}

Node* AVLTree::RightRotate(Node* node){
    Node* node2=node->left;
    node->left=node2->right;
    node2->right=node;
    SizeSet(node);
    SizeSet(node2);
    HeightSet(node);
    HeightSet(node2);
    return node2;
}

Node* AVLTree::Balance(Node* node){
    HeightSet(node);
    if (Height(node->right)-Height(node->left)==2){
        Node* node2=node->right;
        if (Height(node2->right)-Height(node2->left)<0){
            node->right=RightRotate(node2);
        }
        return LeftRotate(node);
    }
    if (Height(node->right)-Height(node->left)==-2){
        Node* node2=node->left;
        if (Height(node2->right)-Height(node2->left)>0){
            node->left=LeftRotate(node2);
        }
        return RightRotate(node);
    }
    return node;
}

void AVLTree::Add(int value, int& position){
    root=Add1(root, value, position);
}

Node* AVLTree::Add1(Node* node, int value, int& position){
    if (node==nullptr){
        node=new Node(value);
        return node;
    }
    node->Size++;
    if (value<node->data){
        if (node->right==nullptr){
            position++;
        } else {
            position+=node->right->Size+1;
        }
        node->left=Add1(node->left, value, position);
    } else {
        node->right=Add1(node->right, value, position);
    }
    return Balance(node);
}

Node* AVLTree::FindMin(Node* node){
    if (node->left==nullptr){
        return node;
    } else {
        return FindMin(node->left);
    }
}

Node* AVLTree::DeleteMin(Node* node){
    if (node->left==nullptr){
        return node->right;
    }
    node->left=DeleteMin(node->left);
    node->Size--;
    return Balance(node);
}

void AVLTree::Delete(int key){
    root=Delete1(root, key);
}

Node* AVLTree::Delete1(Node* node, int position){
    if (node==nullptr){
        return nullptr;
    }
    int RightSize;
    if (node->right==nullptr){
        RightSize=0;
    } else {
        RightSize=node->right->Size;
    }
    if (position>RightSize){
        node->left=Delete1(node->left, abs(position-RightSize-1));
    } else if (position<RightSize){
        node->right=Delete1(node->right, position);
    } else {
        Node* Left=node->left;
        Node* Right=node->right;
        delete node;
        if (Right==nullptr){
            return Left;
        }
        Node* MinNode=FindMin(Right);
        MinNode->right=DeleteMin(Right);
        MinNode->left=Left;
        SizeSet(MinNode);
        return Balance(MinNode);
    }
    SizeSet(node);
    return Balance(node);
}

void AVLTree::Destroy(Node* root){
    if (root==nullptr){
        return;
    }
    Destroy(root->left);
    Destroy(root->right);
    delete root;
}

int main()
{
    int n;
    cin>>n;
    int order, key;
    AVLTree Tree;
    vector<int> a;
    for (int i=0; i<n; ++i){
        cin>>order;
        cin>>key;
        if (order==1){
            int position=0;
            Tree.Add(key, position);
            a.push_back(position);
        } else {
            Tree.Delete(key);
        }
    }
    for (int i=0; i<a.size(); ++i){
        cout<<a[i]<<endl;
    }
    return 0;
}
