#include<bits/stdc++.h>

using namespace std;

struct Node{
    int id, stock;
    string name, color;
    Node *left, *right, *parent;

    Node(int id = -1, string name = "", int count = 0){
        this->id = id;
        this->name = name;
        this->stock = count;
        color = "RED";
        left = right = parent = nullptr;
    }
};

class RedBlackTree{
    Node *root;
    Node *NIL;
    int size;

    void leftRotate(Node *x){
        Node *y = x->right;
        x->right = y->left;
        if(y->left != NIL){
            y->left->parent = x;
        }
        y->parent = x->parent;
        if(x->parent == nullptr){
            root = y;
        }
        else if(x == x->parent->left){
            x->parent->left = y;
        }
        else{
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *y){
        Node *x = y->left;
        y->left = x->right;
        if(x->right != NIL){
            x->right->parent = y;
        }
        x->parent = y->parent;
        if(y->parent == nullptr){
            root = x;
        }
        else if(y == y->parent->right){
            y->parent->right = x;
        }
        else{
            y->parent->left = x;
        }
        x->right = y;
        y->parent = x;
    }

    void insertFix(Node *z){
        while(z->parent != nullptr && z->parent->color == "RED"){
            if(z->parent == z->parent->parent->left){
                Node *y = z->parent->parent->right;
                if(y->color == "RED"){
                    z->parent->color = y->color = "BLACK";
                    z->parent->parent->color = "RED";
                    z = z->parent->parent;
                }
                else{
                    if(z == z->parent->right){
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = "BLACK";
                    z->parent->parent->color = "RED";
                    rightRotate(z->parent->parent);
                }
            }
            else{
                Node *y = z->parent->parent->left;
                if(y->color == "RED"){
                    z->parent->color = y->color = "BLACK";
                    z->parent->parent->color = "RED";
                    z = z->parent->parent;
                }
                else{
                    if(z == z->parent->left){
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = "BLACK";
                    z->parent->parent->color = "RED";
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = "BLACK";
    }

    Node *minVal(Node *x){
        while(x->left != NIL){
            x = x->left;
        }
        return x;
    }

    void transplant(Node *u, Node *v){
        if(u->parent == nullptr){
            root = v;
        }
        else if(u == u->parent->left){
            u->parent->left = v;
        }
        else{
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteFix(Node *x){
        while(x != root && x->color == "BLACK"){
            if(x == x->parent->left){
                Node *w = x->parent->right;
                if(w->color == "RED"){
                    w->color = "BLACK";
                    x->parent->color = "RED";
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if(w->left->color == "BLACK" && w->right->color == "BLACK"){
                    w->color = "RED";
                    x = x->parent;
                }
                else{
                    if(w->right->color == "BLACK"){
                        w->left->color = "BLACK";
                        w->color = "RED";
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = "BLACK";
                    w->right->color = "BLACK";
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else{
                Node *w = x->parent->left;
                if(w->color == "RED"){
                    w->color = "BLACK";
                    x->parent->color = "RED";
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if(w->left->color == "BLACK" && w->right->color == "BLACK"){
                    w->color = "RED";
                    x = x->parent;
                }
                else{
                    if(w->left->color == "BLACK"){
                        w->right->color = "BLACK";
                        w->color = "RED";
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = "BLACK";
                    w->left->color = "BLACK";
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = "BLACK";
    }

    void treeHelper(Node *v, ostream &out){
        if(v == NIL) return;

        if(v->color == "RED"){
            out<<v->id<<"_"<<v->name;
            cout<<"\033[31m"<<v->id<<"_"<<v->name<<"\033[0m";
        }
        else{
            out<<v->id<<"_"<<v->name;
            cout<<"\033[30m"<<v->id<<"_"<<v->name<<"\033[0m";
        }

        if(v->left != NIL || v->right != NIL){
            out<<"(";
            cout<<"(";
            treeHelper(v->left, out);
            out<<",";
            cout<<",";
            treeHelper(v->right, out);
            out<<")";
            cout<<")";
        }
    }

    void listHelper(Node *v, ostream &out){
        if(v == NIL) return;

        listHelper(v->left, out);
        if(v->color == "RED"){
            out<<v->id<<" => "<<v->name<<" ("<<v->stock<<")\n";
            cout<<"\033[31m"<<v->id<<" => "<<v->name<<" ("<<v->stock<<")"<<"\033[0m"<<"\n";
        }
        else{
            out<<v->id<<" => "<<v->name<<" ("<<v->stock<<")\n";
            cout<<"\033[30m"<<v->id<<" => "<<v->name<<" ("<<v->stock<<")"<<"\033[0m"<<"\n";
        }
        listHelper(v->right, out);
    }

    void clearHelper(Node *v) {
        if(v == NIL) return;
        clearHelper(v->left);
        clearHelper(v->right);
        delete v;
    }

public:
    RedBlackTree(){
        NIL = new Node();
        NIL->color = "BLACK";
        root = NIL;
        size = 0;
    }

    ~RedBlackTree(){
        clearHelper(root);
        delete NIL;
    }

    void insert(int id, const string &name, int stock){
        Node *z = new Node(id, name, stock);
        Node *y = nullptr;
        Node *x = root;

        while(x != NIL){
            y = x;
            if(z->id == x->id){
                x->name = name;
                x->stock = stock;
                delete z;
                return;
            }
            else if(z->id < x->id){
                x = x->left;
            }
            else{
                x = x->right;
            }
        }

        z->parent = y;
        if(y == nullptr){
            root = z;
        }
        else if(z->id < y->id){
            y->left = z;
        }
        else{
            y->right = z;
        }

        z->left = z->right = NIL;
        z->color = "RED";
        insertFix(z);
        size++;
    }

    Node *find(int id){
        Node *v = root;
        while(v != NIL && v->id != id){
            if(id < v->id){
                v = v->left;
            }
            else{
                v = v->right;
            }
        }
        if(v == NIL) return nullptr;
        return v;
    }

    void deleteNode(Node *z){
        Node* y = z;
        Node* x;
        string original_color = y->color;

        if(z->left == NIL){
            x = z->right;
            transplant(z, z->right);
        }
        else if(z->right == NIL){
            x = z->left;
            transplant(z, z->left);
        }
        else{
            y = minVal(z->right);
            original_color = y->color;
            x = y->right;
            if(y->parent == z){
                x->parent = y;
            }
            else{
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if(original_color == "BLACK"){
            deleteFix(x);
        }
        size--;
    }

    void printTree(ostream &out){
        treeHelper(root, out);

        out<<"\n";
        cout<<"\n";
    }

    void printList(ostream &out){
        listHelper(root, out);
    }

    void clearTree(){
        clearHelper(root);
        root = NIL;
        size = 0;
    }

    int getSize(){
        return size;
    }

    bool isEmpty(){
        return size == 0;
    }
};

class Inventory{
    RedBlackTree rbt;

public:
    void AddItem(int id, const string &name, int stock, ostream &out){
        Node *v = rbt.find(id);
        string proper_name = name;
        
        if(!proper_name.empty() && proper_name.front() == '"' && proper_name.back() == '"'){
            proper_name = proper_name.substr(1, proper_name.length()-2);
        }

        if(v == nullptr){
            rbt.insert(id, proper_name, stock);
        } else {
            v->name = proper_name;
            v->stock = stock;
        }
        rbt.printTree(out);
    }

    void BuyItem(int id, int quantity, ostream &out){
        Node *v = rbt.find(id);

        if(v == nullptr){
            out<<"Not available\n";
            cout<<"Not available\n";
        }
        else{
            v->stock -= quantity;
            if(v->stock <= 0){
                rbt.deleteNode(v);
            }
            rbt.printTree(out);
        }
    }

    void CheckItem(int id, ostream &out){
        Node *v = rbt.find(id);

        if(v == nullptr){
            out<<"Not available\n";
            cout<<"Not available\n";
        }
        else{
            out<<"Stock left: "<<v->stock<<"\n";
            cout<<"Stock left: "<<v->stock<<"\n";
        }
    }

    void ClearInventory(ostream &out){
        if(rbt.isEmpty()){
            out<<"unsuccessful\n";
            cout<<"unsuccessful\n";
        }
        else{
            rbt.clearTree();
            out<<"successful\n";
            cout<<"successful\n";
        }
    }

    void InventorySize(ostream &out){
        out<<rbt.getSize()<<"\n";
        cout<<rbt.getSize()<<"\n";
    }

    void Empty(ostream &out){
        if(rbt.isEmpty()){
            out<<"yes\n";
            cout<<"yes\n";
        }
        else{
            out<<"no\n";
            cout<<"no\n";
        }
    }

    void ListInventory(ostream &out){
        rbt.printList(out);
    }
};

int main(){
    ifstream in("input.txt");
    if(!in){
        cerr<<"Error: Could not open input.txt!\n";
        return 1;
    }

    ofstream out("output.txt");
    if(!out){
        cerr<<"Error: Could not open output.txt!\n";
        return 1;
    }

    Inventory inventory;
    string s;

    int id, stock, quantity;
    string name;

    while(in>>s){
        if(s == "AI"){
            if(!(in>>id>>name>>stock)){
                out<<"ERROR: Invalid arguments for AI command\n";
                cerr<<"ERROR: Invalid arguments for AI command\n";
                continue;
            }
            inventory.AddItem(id, name, stock, out);
        }
        else if(s == "BI"){
            if(!(in>>id>>quantity)){
                out<<"ERROR: Invalid arguments for BI command\n";
                cerr<<"ERROR: Invalid arguments for BI command\n";
                continue;
            }
            inventory.BuyItem(id, quantity, out);
        }
        else if(s == "CI"){
            if(!(in>>id)){
                out<<"ERROR: Invalid arguments for CI command\n";
                cerr<<"ERROR: Invalid arguments for CI command\n";
                continue;
            }
            inventory.CheckItem(id, out);
        }
        else if(s == "Clr"){
            inventory.ClearInventory(out);
        }
        else if(s == "S"){
            inventory.InventorySize(out);
        }
        else if(s == "Em"){
            inventory.Empty(out);
        }
        else if(s == "Itr"){
            inventory.ListInventory(out);
        }
        else{
            out<<"ERROR: Invalid command: "<<s<<"\n";
            cerr<<"ERROR: Invalid command: "<<s<<"\n";
            continue;
        }
    }

    in.close();
    out.close();

    return 0;
}