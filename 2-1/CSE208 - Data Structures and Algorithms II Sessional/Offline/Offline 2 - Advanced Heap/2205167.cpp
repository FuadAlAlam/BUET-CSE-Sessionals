#include<bits/stdc++.h>

using namespace std;

const int INF = 1e9;

class Node{
public:
	int key, degree;
	Node *parent;
	Node *child;
	Node *sibling;

	Node(int k = -INF){
		key = k;
		degree = 0;
		parent = child = sibling = nullptr;
	}
};

class BinomialHeap{
	Node *head;
	unordered_map<int, Node*> mp;

	Node *mergeTree(Node *u, Node *v){
		if(u->key > v->key) swap(u, v);

		v->parent = u;
		v->sibling = u->child;
		u->child = v;
		u->degree++;

		return u;
	}

	Node *mergeHeap(Node *u, Node *v){
		if(!u) return v;
		if(!v) return u;

		Node *new_head = nullptr;
		Node *tail = nullptr;
		
		while(u && v){
			Node *temp = nullptr;

			if(u->degree <= v->degree){
				temp = u;
				u = u->sibling;
			}
			else{
				temp = v;
				v = v->sibling;
			}

			if(!new_head){
				new_head = tail = temp;
			}
			else{
				tail->sibling = temp;
				tail = temp;
			}
		}

		if(u){
			if(!new_head) new_head = u;
			else tail->sibling = u;
		}
		if(v){
			if(!new_head) new_head = v;
			else tail->sibling = v;
		}

		return new_head;
	}

	Node *unionHeap(Node *u, Node *v){
		Node *new_head = mergeHeap(u, v);
		if(!new_head) return nullptr;

		Node *prev = nullptr;
		Node *current = new_head;
		Node *next = new_head->sibling;

		while(next){
			if(current->degree != next->degree){
				prev = current;
				current = next;
			}
			else if(next->sibling && next->sibling->degree == current->degree){
				prev = current;
				current = next;
			}
			else{
				if(current->key <= next->key){
					current->sibling = next->sibling;
					current = mergeTree(current, next);
				}
				else{
					if(prev) prev->sibling = next;
					else new_head = next;
					next = mergeTree(next, current);
					current = next;
				}
			}

			next = current->sibling;
		}

		return new_head;
	}

	void printHelper(Node *u, int order){
		if(!u) return;

		queue<pair<Node*, int>> q;
		q.push({u, 0});
		unordered_map<int, vector<int>> levels;

		while(!q.empty()){
			Node *v = q.front().first;
			int level = q.front().second;
			q.pop();

			levels[level].push_back(v->key);

			Node *temp = v->child;
			while(temp){
				q.push({temp, level+1});
				temp = temp->sibling;
			}
		}

		cout<<"Binomial Tree, B"<<order<<"\n";

		int i = 0;
		while(levels[i].size() > 0){
			cout<<"Level "<<i<<": ";
			for(int val: levels[i]){
				cout<<val<<" ";
			}
			cout<<"\n";

			i++;
		}
	}

	void clearHelper(Node *u){
		if(!u) return;

		clearHelper(u->child);
		clearHelper(u->sibling);
		delete u;
	}

public:

	BinomialHeap(){
		head = nullptr;
	}

	~BinomialHeap(){
		clearHelper(head);
	}

	int findMin(){
		if(!head) return -INF;
		int min_val = head->key;

		Node *temp = head->sibling;
		while(temp){
			if(temp->key < min_val) min_val = temp->key;
			temp = temp->sibling;
		}

		return min_val;
	}

	int extractMin(){
		if(!head) return -INF;
		int min_val = head->key;

		Node *minNode = head;
		Node *prevMin = nullptr;
		Node *current = head;
		Node *prev = nullptr;

		while(current){
			if(current->key < min_val){
				min_val = current->key;
				minNode = current;
				prevMin = prev;
			}
			prev = current;
			current = current->sibling;
		}

		if(prevMin) prevMin->sibling = minNode->sibling;
		else head = minNode->sibling;

		Node *u = minNode->child;
		Node *v = nullptr;

		while(u){
			Node *temp = u->sibling;
			u->sibling = v;
			v = u;
			v->parent = nullptr;
			u = temp;
		}

		mp.erase(min_val);
		delete minNode;
		head = unionHeap(head, v);

		return min_val;
	}

	void insert(int key){
		Node *v = new Node(key);

		head = unionHeap(head, v);
		mp[key] = v;
 	}

 	void updateKey(int key, int new_key){
 		Node *v = mp[key];

 		v->key = new_key;
 		mp.erase(key);
 		mp[new_key] = v;

 		Node *temp = v;
 		Node *par = temp->parent;
 		while(par && temp->key < par->key){
 			swap(temp->key, par->key);
 			mp[temp->key] = temp;
 			mp[par->key] = par;
 			temp = par;
 			par = temp->parent;
 		}
 	}

 	void removeKey(int key){
 		updateKey(key, -INF);
 		extractMin();
 	}

 	void printHeap(){
 		cout<<"Printing Binomial Heap…\n";

 		Node *temp = head;
 		while(temp){
 			printHelper(temp, temp->degree);
 			temp = temp->sibling;
 		}
 	}
};

int main(){
	ifstream in("input.txt");
	freopen("output.txt", "w", stdout);

	BinomialHeap heap;
	string s;

	while(in>>s){
		if(s=="F"){
			int val = heap.findMin();
			if(val != -INF) cout<<"Find Min returned: "<<val<<"\n";
		}
		else if(s=="E"){
			int val = heap.extractMin();
			if(val != -INF) cout<<"Extract Min returned: "<<val<<"\n";
		}
		else if(s=="I"){
			int val; in>>val;
			heap.insert(val);
		}
		else if(s=="D"){
			int val, new_val; in>>val>>new_val;
			heap.updateKey(val, new_val);
		}
		else if(s=="R"){
			int val; in>>val;
			heap.removeKey(val);
		}
		else if(s=="P"){
			heap.printHeap();
		}
		else{
			cout<<"Invalid Command\n";
		}
	}

	in.close();

	return 0;
}