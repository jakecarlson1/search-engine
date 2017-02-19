/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This QueryEnigne class allows for searching over the index. This object
 * receives a query containing search words and boolean expressions and returns
 * the most relevant pages in the index
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added basic implementation for issuing searches          4/12
  *     0.2   +added implemantation for searching multiple words in     4/16
  *            search query while not searching boolean expressions
  *     0.3   +added and, or, and not merge implementations             4/24
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "queryengine.h"

/**
 * consturctor initializes values
 */
QueryEngine::QueryEngine():
  searchReturn(nullptr), numberOfReturns(0), queryWords(nullptr),
  numberOfQueryWords(1), masterReturn(nullptr)
{

}

/**
 * destructor
 */
QueryEngine::~QueryEngine()
{

}

/**
 * issueSearch receives a query from the UserInterface and processes it,
 * searching the index for the search words entered. A SearchReturnList is
 * returned that has the most relevant documents to the search in it
 */
SearchReturnList* QueryEngine::issueSearch(IndexInterface* indexAccess,
  string incomingSearch)
{
  if(masterReturn != nullptr)
  {
    delete masterReturn;
    masterReturn = nullptr;
  }

  countQueryWords(incomingSearch);
  generateQueryWords(incomingSearch);

  searchRelevantWords(indexAccess);
  applyBooleanExpressions();

  masterReturn->sortReturn();
  masterReturn->trimReturnList();

  //resets the everything needed for processing the query
  returnLists.clear();
  numberOfReturns = 0;
  numberOfQueryWords = 1;

  return masterReturn;
}



/**
 * countQueryWords counts the number of words in the searchQuery
 */
void QueryEngine::countQueryWords(string& searchQuery)
{
  numberOfQueryWords = 1;
  for(int i = 0; i < searchQuery.size(); i++)
  {
    if(searchQuery[i] == ' ')
    {
      numberOfQueryWords++;
    }
  }
}

/**
 * generateQueryWords stems and saves the words in searchQuery that are not
 * boolean expressions
 */
void QueryEngine::generateQueryWords(string& searchQuery)
{
  StemAndStopWords wordStemmer;
  queryWords = new string[numberOfQueryWords];
  stringstream queryReader(searchQuery);
  for(int i = 0; i < numberOfQueryWords; i++)
  {
    string tempWord;
    queryReader >> tempWord;
    if(!isBooleanExpression(tempWord))
    {
      wordStemmer.stemWord(tempWord);
    }
    queryWords[i] = tempWord;
  }
}

/**
 * searchRelevantWords searches indexAccess for the search words and pushes
 * the SearchReturnLists to the returnLists vector
 */
void QueryEngine::searchRelevantWords(IndexInterface*& indexAccess)
{
  for(int i = 0; i < numberOfQueryWords; i++)
  {
    if(!isBooleanExpression(queryWords[i]))
    {
      returnLists.push_back(new SearchReturnList(queryWords[i],
        indexAccess->getPageCount(),
        indexAccess->searchForWord(queryWords[i])));
      numberOfReturns++;
    }
  }
}

/**
 * isBooleanExpression returns 1 if checkWord is a boolean expression and 0 if
 * it is not
 */
bool QueryEngine::isBooleanExpression(string& checkWord)
{
  if(checkWord.compare("AND") == 0 || checkWord.compare("OR") == 0 ||
    checkWord.compare("NOT") == 0)
  {
    return 1;
  }
  return 0;
}

/**
 * applyBooleanExpressions first assumes the search will be following an AND
 * relationship. The list of query words is then looped through. The
 * currentBool is applied to all of the SearchReturnLists sequentially until a
 * new boolean expression is found
 */
void QueryEngine::applyBooleanExpressions()
{
  string currentBool("AND");
  int returnListIterator = 0;
  //for-loop loops through all of the words in queryWords
  for(int i = 0; i < numberOfQueryWords; i++)
  {
    //if the word is a boolean expression, currentBool is updated
    if(isBooleanExpression(queryWords[i]))
    {
      currentBool = queryWords[i];
    }
    //on the first pass, masterReturn is initialized
    else if(returnListIterator == 0)
    {
      masterReturn = new SearchReturnList(returnLists.at(returnListIterator)->
        documentIDNumbers);

      returnListIterator++;
    }
    //if it is not the first pass, masterReturn and the current word are merged
    //following the property of currentBool into joinedList
    else
    {
      vector<pair<int, int>> joinedList;
      if(currentBool.compare("AND") == 0)
      {
        andMerge(returnLists.at(returnListIterator)->documentIDNumbers,
          masterReturn->documentIDNumbers, joinedList);
      }
      else if(currentBool.compare("OR") == 0)
      {
        orMerge(returnLists.at(returnListIterator)->documentIDNumbers,
          masterReturn->documentIDNumbers, joinedList);
      }
      else if(currentBool.compare("NOT") == 0)
      {
        notMerge(masterReturn->documentIDNumbers,
          returnLists.at(returnListIterator)->documentIDNumbers, joinedList);
      }
      returnListIterator++;
      //masterReturn is updated
      delete masterReturn;
      masterReturn = new SearchReturnList(joinedList);
    }
  }
}

/**
 * andMerge merges lhs and rhs into target. Elements are only coppied if it
 * appears in both lhs and rhs. The tfidf value of both rhs and lhs are summed
 * for ranking
 */
void QueryEngine::andMerge(vector<pair<int, int>> const& lhs,
  vector<pair<int, int>> const& rhs, vector<pair<int, int>>& target)
{
  int lhsIter = 0;
  int rhsIter = 0;
  int lhsEnd = lhs.size();
  int rhsEnd = rhs.size();

  while(lhsIter != lhsEnd && rhsIter != rhsEnd)
  {
    if(lhs.at(lhsIter).first < rhs.at(rhsIter).first)
    {
      lhsIter++;
    }
    else if(lhs.at(lhsIter).first > rhs.at(rhsIter).first)
    {
      rhsIter++;
    }
    else if(lhs.at(lhsIter).first == rhs.at(rhsIter).first)
    {
      target.push_back(make_pair(lhs.at(lhsIter).first,
        (lhs.at(lhsIter).second + rhs.at(rhsIter).second)));
      lhsIter++;
      rhsIter++;
    }
  }
}

/**
 * orMerge merges lhs and rhs into target. All elements are coppied in order.
 * If a document appears in both lhs and rhs, the tfidf values are added
 */
void QueryEngine::orMerge(vector<pair<int, int>> const& lhs,
  vector<pair<int, int>> const& rhs, vector<pair<int, int>>& target)
{
  int lhsIter = 0;
  int rhsIter = 0;
  int lhsEnd = lhs.size();
  int rhsEnd = rhs.size();

  while(lhsIter != lhsEnd && rhsIter != rhsEnd)
  {
    if(lhs.at(lhsIter).first < rhs.at(rhsIter).first)
    {
      target.push_back(make_pair(lhs.at(lhsIter).first,
        lhs.at(lhsIter).second));
      lhsIter++;
    }
    else if(lhs.at(lhsIter).first > rhs.at(rhsIter).first)
    {
      target.push_back(make_pair(rhs.at(rhsIter).first,
        rhs.at(rhsIter).second));
      rhsIter++;
    }
    else if(lhs.at(lhsIter).first == rhs.at(rhsIter).first)
    {
      target.push_back(make_pair(lhs.at(lhsIter).first,
        (lhs.at(lhsIter).second + rhs.at(rhsIter).second)));
      lhsIter++;
      rhsIter++;
    }
  }
}

/**
 * notMerge places elements of lhs into target if they do not occur in rhs
 */
void QueryEngine::notMerge(vector<pair<int, int>> const& lhs,
  vector<pair<int, int>> const& rhs, vector<pair<int, int>>& target)
{
  int lhsIter = 0;
  int rhsIter = 0;
  int lhsEnd = lhs.size();
  int rhsEnd = rhs.size();

  while(lhsIter != lhsEnd && rhsIter != rhsEnd)
  {
    if(lhs.at(lhsIter).first < rhs.at(rhsIter).first)
    {
      target.push_back(make_pair(lhs.at(lhsIter).first,
        lhs.at(lhsIter).second));
      lhsIter++;
    }
    else if(lhs.at(lhsIter).first > rhs.at(rhsIter).first)
    {
      rhsIter++;
    }
    else
    {
      rhsIter++;
      lhsIter++;
    }
  }
}
