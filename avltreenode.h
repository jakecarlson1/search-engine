#ifndef AVLTREENODE_H
#define AVLTREENODE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class AVLTreeNode
{
public:
  AVLTreeNode();
  AVLTreeNode(string);
  AVLTreeNode(string, vector<pair<int, int>>);
  ~AVLTreeNode();

  void addDocumentNumber(int);
  void incrementCountOfWord();
  string getIndexedWord();
  int getLastDocumentID();
  int getCountOfLastID();
  vector<pair<int, int>>* getDocumentIDNumbers();

private:
  int height;
  string indexedWord;
  vector<pair<int, int>> documentIDNumbers;
  AVLTreeNode* left;
  AVLTreeNode* right;

  friend class AVLTreeIndex;

};

#endif // AVLTREENODE_H
