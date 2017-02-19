#ifndef AVLTREEINDEX_H
#define AVLTREEINDEX_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "indexinterface.h"
#include "avltreenode.h"

using namespace std;

class AVLTreeIndex: public IndexInterface
{
public:
  AVLTreeIndex();
  virtual ~AVLTreeIndex();

  virtual void loadPersistantIndex();
  virtual void addWordToIndex(string&, int&);
  virtual int* searchForWord(string);
  virtual void saveIndexToDisk();
  virtual void printMostFrequentWords();

  void saveTree(ofstream&);
  void pushWordToTree(int&, string&, vector<pair<int, int>>&);
  vector<pair<string, int>> getMostFrequentWords();

private:
  int numPagesInIndex;
  int numWordsIndexed;
  unordered_map<string, int> termFrequencies;
  AVLTreeNode* root;

  void insertWord(string&, int&, AVLTreeNode*&);
  inline int getHeight(AVLTreeNode*&);
  inline int max(int, int);
  inline void rotateWithLeftChild(AVLTreeNode*&);
  inline void rotateWithRightChild(AVLTreeNode*&);
  inline void doubleWithLeftChild(AVLTreeNode*&);
  inline void doubleWithRightChild(AVLTreeNode*&);
  AVLTreeNode* searchNodeForWord(string, AVLTreeNode*);
  void saveNode(ofstream&, AVLTreeNode*);
  void insertFullWord(int&, string&, vector<pair<int, int>>&, AVLTreeNode*&);
  void deleteNode(AVLTreeNode*&);

  struct compareByCount
  {
    bool operator()(pair<string, int>left, pair<string, int>right)
    const {
      return (left.second > right.second);
    }
  };

};

#endif // AVLTREEINDEX_H
