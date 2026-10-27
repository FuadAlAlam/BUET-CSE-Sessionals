#include <iostream>

using namespace std;

struct Node
{
    int val;
    Node *left;
    Node *right;
};

Node *insertNode(Node *node, int val)
{
    // Write your codes here
    // This function takes a pointer to the root of the BST and 
    // the value to be inserted
    // Returns the pointer to the root of the modified BST.

    if(node == nullptr){
        Node *v = new Node;
        v->val = val;
        v->left = nullptr;
        v->right = nullptr;
        return v;
    }

    if(val < node->val) node->left = insertNode(node->left, val);
    else if(val > node->val) node->right = insertNode(node->right, val);

    return node;
}

Node *deleteNode(Node *node, int val)
{
    // Write your codes here
    // This function takes a pointer to the root of the BST and 
    // the value to be deleted.
    // Returns the pointer to the root of the modified BST.

    if(node == nullptr) return nullptr;

    if(val < node->val) node->left = deleteNode(node->left, val);
    else if(val > node->val) node->right = deleteNode(node->right, val);
    else{
        if(node->left == nullptr){
            Node *v = node->right;
            delete node;
            return v;
        }
        else if(node->right == nullptr){
            Node *v = node->left;
            delete node;
            return v;
        }
        else{
            Node *v = node->right;
            while(v->left != nullptr){
                v = v->left;
            }
            node->val = v->val;
            node->right = deleteNode(node->right, v->val);
        }
    }

    return node;
}

Node* find(Node *node, int val)
{
    // Write your codes here
    // This function takes a pointer to the root of the BST and 
    // the value to be searched.
    // Returns the pointer to the node that contains val.
    // Returns NULL if not found.

    if(node == nullptr) return nullptr;

    if(val == node->val) return node;
    if(val < node->val) return find(node->left, val);
    if(val > node->val) return find(node->right, val);

    return nullptr;
}

void inOrder(Node *node)
{
    // Write your codes here
    // This function takes a pointer to the root of the BST
    // Prints the in-order traversal of the given BST

    if(node == nullptr) return;

    inOrder(node->left);
    cout<<node->val<<" ";
    inOrder(node->right);
}

void preOrder(Node *node)
{
    // Write your codes here
    // This function takes a pointer to the root of the BST
    // Prints the pre-order traversal of the given BST

    if(node == nullptr) return;

    cout<<node->val<<" ";
    preOrder(node->left);
    preOrder(node->right);
}

void postOrder(Node *node)
{
    // Write your codes here
    // This function takes a pointer to the root of the BST
    // Prints the post-order traversal of the given BST

    if(node == nullptr) return;

    postOrder(node->left);
    postOrder(node->right);
    cout<<node->val<<" ";
}

void printTree(Node *root){
    // Write your codes here
    // This function takes a pointer to the root of the BST
    // Prints parenthesized representation of the given BST

    if(root == nullptr){
        cout<<"_";
        return;
    }

    cout<<root->val;

    if(root->left != nullptr || root->right != nullptr){
        cout<<" (";
        printTree(root->left);
        cout<<", ";
        printTree(root->right);
        cout<<")";
    }
}