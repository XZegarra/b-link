// Copyright 2021 Roger Peralta Aranibar
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

#include "b-link.hpp"

std::size_t NUMBER_THREADS = 8;

const std::size_t NUMBER_OPERATIONS = 2;

int MIN_VALUE = 1;
int MAX_VALUE = 15000;

std::random_device rd;

class TreeSearcher {
 public:
  TreeSearcher(unsigned int id, EDA::Concurrent::BLinkTree<3, int> *b_link)
      : id_(id), b_link_(b_link) {}

  void operator()() {
    std::uniform_int_distribution<int> distribution(MIN_VALUE, MAX_VALUE);
    for (std::size_t i = 0; i < NUMBER_OPERATIONS; ++i) {
      std::cout << distribution(rd) << "\n";
      b_link_->search(distribution(rd));
    }
  }

 private:
  unsigned int id_;
  EDA::Concurrent::BLinkTree<3, int> *b_link_;
};

void sequential_insert(EDA::Concurrent::BLinkTree<3, int> *b_link) {
  std::uniform_int_distribution<int> distribution(MIN_VALUE, MAX_VALUE);
  for (std::size_t i = 0; i < NUMBER_OPERATIONS; ++i) {
    std::cout << distribution(rd) << "\n";
     b_link->insert(distribution(rd));
  }
}

void run_test() {
  EDA::Concurrent::BLinkTree<3, int> b_link;

  sequential_insert(&b_link);

  std::thread *threads = new std::thread[NUMBER_THREADS];

  for (std::size_t i = 0; i < NUMBER_THREADS; ++i) {
    threads[i] = std::thread(TreeSearcher(i, &b_link));
  }
  for (std::size_t i = 0; i < NUMBER_THREADS; ++i) {
    threads[i].join();
  }

}

void run_test2() {
	EDA::Concurrent::BLinkTree<3, int> b_link;

	b_link.insert(1);
	b_link.insert(4);
	b_link.insert(7);
	b_link.insert(10);
	b_link.insert(17);
	b_link.insert(21);
	b_link.insert(31);
	b_link.insert(25);
	b_link.insert(19);
	b_link.insert(20);
	b_link.insert(28);
	b_link.insert(42);


	//b_link.insert(6);
	//b_link.insert(16);
	//b_link.insert(26);
	//b_link.insert(36);
	//b_link.insert(46);

	//b_link.amplitud();

	cout << "\t"; b_link.getRoot()->printValue();
	cout << "\t\t";b_link.getRoot()->ptr[1]->printValue();
	cout << "\t";b_link.getRoot()->ptr[1]->ptr[0]->printValue();
	cout << "\t";b_link.getRoot()->ptr[1]->ptr[1]->printValue();
	cout << "\t";b_link.getRoot()->ptr[0]->ptr[2]->printValue();
	cout << "\t";b_link.getRoot()->ptr[0]->ptr[3]->printValue();
	cout << b_link.search(20);
	//cout << "\t\t\t"; b_link.getRoot()->printValue();
	//cout << "\t\t";b_link.getRoot()->ptr[0]->printValue();
	//cout << "\t";b_link.getRoot()->ptr[1]->ptr[0]->printValue();
	//cout << "\t";b_link.getRoot()->ptr[0]->ptr[1]->printValue();
	//cout << "\t";b_link.getRoot()->ptr[0]->ptr[2]->printValue();


	//b_link.printLeafs();
}

int main() {
  run_test();
  return 0;
}
