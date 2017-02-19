#ifndef HASHTABLEINDEX_H
#define HASHTABLEINDEX_H

#include <iostream>
#include <string>
#include <functional>

#include "indexinterface.h"
#include "avltreeindex.h"

using namespace std;

class HashTableIndex: public IndexInterface
{
public:
  HashTableIndex();
  virtual ~HashTableIndex();

  virtual void loadPersistantIndex();
  virtual void addWordToIndex(string&, int&);
  virtual int* searchForWord(string);
  virtual void saveIndexToDisk();
  virtual void printMostFrequentWords();

private:
  long numberOfBuckets;
  AVLTreeIndex** hashTableIndex;

  long hashString(string&);

  struct compareByCount
  {
    bool operator()(pair<string, int>left, pair<string, int>right)
    const {
      return (left.second > right.second);
    }
  };

};

#endif // HASHTABLEINDEX_H
