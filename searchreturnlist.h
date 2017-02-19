#ifndef SEARCHRETURNLIST_H
#define SEARCHRETURNLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class SearchReturnList
{
public:
  SearchReturnList();
  SearchReturnList(string, int, int*);
  SearchReturnList(vector<pair<int, int>>);
  ~SearchReturnList();

  void printDocs();
  void trimReturnList();
  void sortReturn();
  vector<pair<int, int>>& getReturnList();

private:
  string searchWord;
  int numberOfReturns;
  int corpusDocumentCount;
  vector<pair<int, int>> documentIDNumbers;

  void tfidfConvert();

  struct compareByCount
  {
    bool operator()(pair<int, int>left, pair<int, int>right)
    const {
      return (left.second > right.second);
    }
  };

  friend class QueryEngine;
};

#endif // SEARCHRETURNLIST_H
