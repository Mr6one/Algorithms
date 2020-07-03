#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct Node{
    int data;
    Node* left;
    Node* right;
    Node(int value){
        data=value;
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
    void PreOrder(vector<int>&);
private:
    Node* root;
    void Delete(Node*);
};

void BinaryTree::AddNode(int value){
    if (root==nullptr){
        root=new Node(value);
        return;
    }
    Node* node=root;
    while ((node->left!=nullptr || value>=node->data) && (node->right!=nullptr || value<node->data)){
        if (value<node->data){
            node=node->left;
        } else {
            node=node->right;
        }
    }
    Node* NewNode=new Node(value);
    if (value<node->data){
        node->left=NewNode;
    } else {
        node->right=NewNode;
    }
}

void BinaryTree::PreOrder(vector<int>& NodesValue){
    Node* node=root;
    stack<Node*> way;
    way.push(node);
    while (!way.empty()){
        while (node!=nullptr){
            if (node->right!=nullptr){
                way.push(node->right);
            }
            NodesValue.push_back(node->data);
            node=node->left;
        }
        node=way.top();
        way.pop();
    }
}

void BinaryTree::Delete(Node* root){
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
    BinaryTree tree;
    int key;
    for (int i=0; i<n; i++){
        cin>>key;
        tree.AddNode(key);
    }
    vector<int> NodesValue;
    tree.PreOrder(NodesValue);
    for (int i=0; i<NodesValue.size(); ++i){
        cout<<NodesValue[i]<<' ';
    }
    return 0;
}
