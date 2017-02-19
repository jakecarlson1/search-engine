#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "indexinterface.h"
#include "searchreturnlist.h"
#include "stemandstopwords.h"

using namespace std;

class QueryEngine
{
public:
  QueryEngine();
  ~QueryEngine();
  SearchReturnList* issueSearch(IndexInterface*, string);

private:
  int* searchReturn;
  int numberOfReturns;
  int numberOfQueryWords;
  string* queryWords;
  vector<SearchReturnList*> returnLists;
  SearchReturnList* masterReturn;

  void countQueryWords(string&);
  void generateQueryWords(string&);
  void searchRelevantWords(IndexInterface*&);
  bool isBooleanExpression(string&);
  void applyBooleanExpressions();

  void andMerge(vector<pair<int, int>> const&, vector<pair<int, int>> const&,
    vector<pair<int, int>>&);
  void orMerge(vector<pair<int, int>> const&, vector<pair<int, int>> const&,
    vector<pair<int, int>>&);
  void notMerge(vector<pair<int, int>> const&, vector<pair<int, int>> const&,
    vector<pair<int, int>>&);

};

#endif // QUERYENGINE_H
