/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This SearchReturnList class acts as a container for the list of document IDs
 * a word occurs in. This object calculates a words tfidf value for each
 * document and sorts the document IDs based on this number. This object also
 * allows for removing extra document IDs
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added basic implementation for issuing searches          4/12
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "searchreturnlist.h"

/**
 * constructor
 */
SearchReturnList::SearchReturnList()
{

}

/**
 * constructor initializes documentIDNumebrs to the vector incomingDocs
 */
SearchReturnList::SearchReturnList(vector<pair<int, int>> incomingDocs):
  documentIDNumbers(incomingDocs)
{

}

/**
 * This constructor recieves a integer pointer and generares the
 * documentIDNumbers vector. The term frequencies are then converted to tfidf
 * values
 */
SearchReturnList::SearchReturnList(string incomingWord, int pageCount,
  int* docIDs):
  numberOfReturns(0), corpusDocumentCount(pageCount)
{
  searchWord = incomingWord;
  if(docIDs == nullptr)
  {
    cout << incomingWord << " could not be found in the index" << endl;
  }
  else
  {
    numberOfReturns = docIDs[0];

    for(int i = 1; i < numberOfReturns * 2; i++)
    {
      int currentDocID = docIDs[i];
      i++;
      int currentFreq = docIDs[i];
      documentIDNumbers.push_back(make_pair(currentDocID, currentFreq));
    }
    tfidfConvert();
  }
}

/**
 * destructor
 */
SearchReturnList::~SearchReturnList()
{

}

/**
 * getReturnLists returns documentIDNumbers
 */
vector<pair<int, int>>& SearchReturnList::getReturnList()
{
  return documentIDNumbers;
}

/**
 * tfidfConvert calculates the inverse document frequency value for the word
 * and then converts all of the term frequencies in documentIDNumbers to tfidf
 * values
 */
void SearchReturnList::tfidfConvert()
{
  //inverse document frequency is calculated
  double idf = corpusDocumentCount / documentIDNumbers.size();

  for(int i = 0; i < documentIDNumbers.size(); i++)
  {
    documentIDNumbers.at(i).second = (documentIDNumbers.at(i).second * idf);
  }
}

/**
 * sortReturn sorts the documentIDNumbers by tfidf value
 */
void SearchReturnList::sortReturn()
{
  sort(documentIDNumbers.begin(), documentIDNumbers.end(), compareByCount());
}

/**
 * printDocs prints the documentID numbers and tfidf values
 */
void SearchReturnList::printDocs()
{
  for(int i = 0; i < documentIDNumbers.size(); i++)
  {
    cout << "Document ID: " << documentIDNumbers.at(i).first << " tfidf.: "
      << documentIDNumbers.at(i).second << endl;
  }
}

/**
 * trimReturnList trims this SearchReturnList to 15 elements
 */
void SearchReturnList::trimReturnList()
{
  if(documentIDNumbers.size() > 15)
  {
    documentIDNumbers.resize(15);
  }
}
