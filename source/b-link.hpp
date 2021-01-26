// Copyright
#ifndef SOURCE_B_LINK_HPP_
#define SOURCE_B_LINK_HPP_

#include <utility>
#include<iostream>
#include <queue>

using namespace std;


namespace EDA {
namespace Concurrent {

template <std::size_t B, typename Type>
class BLinkTree {
	class Node {
	public:
		bool isLeaf;						//IF IS LEAF, IT HAS TO BE CONNECTED WITH THE OTHER LEAFTS
		Type* key;
		int size = 0;
		Node** ptr;
		Node() {
			key = new int[B];
			ptr = new Node * [B + 1];

			for (int i = 0; i < B + 1; i++) {
				ptr[i] = NULL;
			}
		};

		void printValue() {
			
			if (key == nullptr) {
				return;
			}

			for (int i = 0; i < size; i++) {
				if (key[i] == NULL) {
					break;
				}
				cout << key[i] << " ";
			}
			cout << endl;
		}
	};

	Node* findParent(Node* current, Node* child) {					//WE WILL WALK TO THROW THE TREE TO FIND THE PARENT OF CHILD
		Node* parent;

		if (current->isLeaf || (current->ptr[0])->isLeaf) {			
			return NULL;
		}

		for (int i = 0; i < current->size + 1; i++) {
			if (current->ptr[i] == child) {
				parent = current;
				return parent;
			}
			else {
				parent = findParent(current->ptr[i], child);
				if (parent != NULL) {
					return parent;
				}
			}
		}
		return parent;
	}
	void fitRoot() {
		Node* current = root;
		current = current->ptr[1];
		if (!current) {
			return;
		}

		while (!(current->isLeaf)) {
			current = current->ptr[0];
		}
		cout << "LEAF AL FONDO: " << current->key[0] << endl;
		cout << "ROOT: " << root->key[0] << endl;

		if (current->key[0] < root->key[0]) {
			Type temporal = (current->key[0]);
			(root->key[0]) = current->key[0];
			current->key[0] = temporal;

		}
	}
	void insertInternal(int value, Node* current, Node* child) {
		if (current->size < B) {	//IF THERE IS NOT OVERFLOW. WE WILL DO THE SAME PROCCESS OF UP ABOVE
			int leftIndex = 0;
			while (value > current->key[leftIndex] && leftIndex < current->size) {
				leftIndex++;
			}

			for (int rightIndex = current->size; rightIndex > leftIndex; rightIndex--) {
				current->key[rightIndex] = current->key[rightIndex - 1];
			}

			for (int rightIndex = current->size + 1; rightIndex > leftIndex + 1; rightIndex--) {
				current->ptr[rightIndex] = current->ptr[rightIndex - 1];
			}

			current->key[leftIndex] = value;
			current->size++;
			current->ptr[leftIndex + 1] = child;
		}
		else {	//IF THERE IS GONNA BE OVERFLOW
			Node* newInternal = new Node;
			Type temporalKey[B + 1];
			Node* temporalPtr[B + 2];

			for (int i = 0; i < B; i++) {
				temporalKey[i] = current->key[i];
			}

			for (int i = 0; i < B + 1; i++) {
				temporalPtr[i] = current->ptr[i];
			}

			int i = 0, j;

			while (value > temporalKey[i] && i < B) {
				i++;
			}

			for (int j = B + 1; j > i; j--) {
				temporalKey[j] = temporalKey[j - 1];
			}

			temporalKey[i] = value;

			for (int j = B + 2; j > i + 1; j--) {
				temporalPtr[j] = temporalPtr[j - 1];
			}

			temporalPtr[i + 1] = child;
			newInternal->isLeaf = false;

			current->size = (B + 1) / 2;

			newInternal->size = B - ((B + 1) / 2);

			for (int i = 0, j = current->size + 1; i < newInternal->size; i++, j++) {
				newInternal->key[i] = temporalKey[j];
			}

			for (int i = 0, j = current->size + 1; i < newInternal->size + 1; i++, j++) {
				newInternal->ptr[i] = temporalPtr[j];
			}

			if (current == root) {
				Node* newRoot = new Node;

				newRoot->key[0] = current->key[current->size];

				newRoot->ptr[0] = current;
				newRoot->ptr[1] = newInternal;
				newRoot->isLeaf = false;
				newRoot->size = 1;
				root = newRoot;
			}
			else {
				insertInternal(current->key[current->size], findParent(root, current), newInternal);
			}
		}
	}

 public:
  typedef Type data_type;

  BLinkTree() {
	  root = NULL;
  }

  void destroy(Node* node) {							//DESTROYING NODES
	  if (node) {
		  for (int i = 0; i < node->size; i++) {
			  destroy(node->ptr[i]);
		  }
		  delete node;
	  }
  }

  ~BLinkTree() {
	  destroy(root);
  }

  std::size_t size() const {}

  Node* getRoot() {
	  return root;
  }

  bool empty() const {
	  if (root == NULL) {
		  return true;
	  }
	  return false;
  }

  bool search(const data_type& value) const {
	  if (empty()) {								//IF THE TREE IS EMPTY WE WILL RETURN FALSE
		  return false;
	  }
	  else {
		  Node* current = root;
		  
		  while (current->isLeaf == false) {				//WHILE CURRENT IS NOT A LEAF WE WILL KEEP MOVING DOWN, WE WILL FIND THE NODE WHERE THE VALUE WE WANT TO FIND IS
			  for (int i = 0; i < current->size; i++) {		//WE WILL MOVE THROW THE PTRs WETHER THE VALUE WE WANT TO FIND IS LESS OR MORE THAN THE CURRENT NODE
				  for (int o = 0; o < current->size; o++) {				//ONCE WE ARE ON THE NODE WHERE OUR VALUE IS SUPPOSED TO BE. WE WILL LOOK FOR IT INSIDE THE KEYS OF THE NODE
					  if (current->key[o] == value) {
						  return true;
					  }
				  }

				  if (value < current->key[i]) {
					  current = current->ptr[i];
					  break;
				  }

				  if (i == current->size - 1) {
					  current = current->ptr[i + 1];
					  break;
				  }
			  }
		  }

		  for (int i = 0; i < current->size; i++) {				//ONCE WE ARE ON THE NODE WHERE OUR VALUE IS SUPPOSED TO BE. WE WILL LOOK FOR IT INSIDE THE KEYS OF THE NODE
			  if (current->key[i] == value) {
				  return true;
			  }
		  }
	  }
	  return false;												//IF WE DON'T FIND THE VALUE IN THAT NODE WE WILL RETURN FALSE BECAUSE THE VALUE IS NOT THERE
  }

  

  void insert(const data_type& value) {
	  if (root == NULL) {				//WE CHECK IF THE VALUE THAT WE WANT TO INSERT IS THE FIRST ONE THAT THE TREE IS RECEIVING
		  root = new Node;
		  root->key[0] = value,
		  root->isLeaf = true;
		  root->size = 1;
	  }
	  else {
		  Node* current = root;		//CURRENT WILL BE ROOT
		  Node* parent;				
		  
		  while (current->isLeaf == false) {		//WE WILL GO ALL THE WAY DOWN UNTIL FIND A LEAF
			  parent = current;						//PARENT WILL BE CURRENT IF CURRENT IT'S NOT A LEAF
			  for (int i = 0; i < current->size; i++) { //WE CHECK THE CURRENT NODE
				  if (value < current->key[i]) {		//IF WE WANT TO INSERT A NODE LESS THAN THE CURRENT KEY WE WILL GO TO THE LEFT NODE
					  current = current->ptr[i];		
					  break;
				  }
				  if (i == current->size - 1) {			//IF WE ARRIVE TO THE FINAL PTR WE WILL GO TO THE RIGHTEST PTR OF THE CURRENT NODE
					  current = current->ptr[i + 1];
					  break;
				  }
			  }
		  }

		  if (current->size < B) {				//IF WE DO NOT HAVE OVERFLOW
			  int leftIndex = 0;
			  while (value > current->key[leftIndex] && leftIndex < current->size) {		//WHILE THE VALUE IT'S GREATER THAN THE ACTUAL KEY AND LESS THAN THE CURRENT SIZE, WE WILL ADD 1 TO IT
				  leftIndex++;
			  }
			  for (int rightIndex = current->size; rightIndex > leftIndex; rightIndex--) { //WHILE THE VALUE IS LESS THAN THE KEYS FROM THE RIGHT
				  current->key[rightIndex] = current->key[rightIndex - 1];	//WE BEGIN SWAPPING
			  }
			  current->key[leftIndex] = value;	//WE FINISH THE SWAP
			  current->size++;	//WE FIX THE SIZE

			  current->ptr[current->size] = current->ptr[current->size - 1];	//WE FIX THE PTRs
			  current->ptr[current->size - 1] = NULL;
		  }
		  else {								//IF WE HAVE OVERFLOW
			  Node* newLeaf = new Node;

			  int temporalNode[B + 1];

			  for (int i = 0; i < B; i++) {			//WE COPY THE WHOLE CURRENT NODE IN OUR TEMPORAL
				  temporalNode[i] = current->key[i];
			  }

			  int leftIndex = 0, rightIndex;

			  while (value > temporalNode[leftIndex] && leftIndex < B) {	//WE GET THE LEFT INDEX
				  leftIndex++;
			  }

			  for (rightIndex = B + 1; rightIndex > leftIndex; rightIndex--) {
				  temporalNode[rightIndex] = temporalNode[rightIndex - 1];	//WE BEGIN TO OPEN SPACE FOR THE NEW KEY
			  }

			  temporalNode[leftIndex] = value;	//WE INSERT THE VALUE ON THE SPACE
			  newLeaf->isLeaf = true;

			  current->size = (B + 1) / 2;		//WE BEGIN THE SPLITTING INDICATING THE NEW SIZES
			  newLeaf->size = B + 1 - ((B + 1) / 2);

			  current->ptr[current->size] = newLeaf;			//WE FIX THE PTRs
																				
			  newLeaf->ptr[newLeaf->size] = current->ptr[B];

			  current->ptr[B] = NULL;

			  for (leftIndex = 0; leftIndex < current->size; leftIndex++) { //WE COPPY OUR KEYS FROM OUR TEMPORAL NODE TO CURRENT FROM LEFT TO RIGHT
				  current->key[leftIndex] = temporalNode[leftIndex];
			  }
			  
			  for (leftIndex = 0, rightIndex = current->size; leftIndex < newLeaf->size; leftIndex++, rightIndex++) { //WE COPPY OUR KEYS FROM OUR TEMPORAL NODE TO NEWLEAF FROM RIGHT TO LEFT
				  newLeaf->key[leftIndex] = temporalNode[rightIndex];
			  }

			  if (current == root) {				//IF CURRENT IS ROOT WE WILL HAVE TO CREATE A NEW ONE AND SET ALL THE VALUES
				  Node* newRoot = new Node;

				  newRoot->key[0] = newLeaf->key[0];
				  newRoot->ptr[0] = current;
				  newRoot->ptr[1] = newLeaf;
				  newRoot->isLeaf = false;
				  newRoot->size = 1;
				  root = newRoot;
			  }
			  else {							//IF NOT WE ARE GONNA HAVE TO DO THE INTERAL INSERT
				  insertInternal(newLeaf->key[0], parent, newLeaf);
			  }
		  }
	  }
	  fitRoot();
  }

  void remove(const data_type& value) {}

  void printLeafs() {
	  Node* current = root;

	  while (!(current->isLeaf)) {
		  current = current->ptr[0];
	  }
	  while (current) {
		  current->printValue();
		  current = current->ptr[B-1];
		  if (!(current)) {
			  break;
		  }
	  }
  }

  void amplitud() {
	  cout << "RECORRIDO POR AMPLITUD:" << endl;
	  cout << "RAIZ: ";
	  root->printValue();
	  cout << "HIJO IZQUIERDO: ";
	  root->ptr[0]->printValue();
	  cout << "HIJO DERECHO: ";
	  root->ptr[1]->printValue();

	  cout << "HOJAS: "; 
	  printLeafs();
  }

 private:
  data_type* data_;
  Node* root;

};

}  // namespace Concurrent
}  // namespace EDA

#endif  // SOURCE_B_LINK_HPP_
