/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This AVLTreeNode class is an elment of the AVLTreeIndex that holds a key
 * word. It also hold other key information about the word
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added basic implementation                               4/12
  *     0.2   +added functions for getting document ID numbers          4/13
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "avltreenode.h"

/**
 * constructor initialized height, left, and right
 */
AVLTreeNode::AVLTreeNode():
  height(0), left(nullptr), right(nullptr)
{

}

/**
 * destructor
 */
AVLTreeNode::~AVLTreeNode()
{

}

/**
 * AVLTreeNode is constructed based on the incoming word
 */
AVLTreeNode::AVLTreeNode(string incomingWord):
  height(0), left(nullptr), right(nullptr)
{
  indexedWord = incomingWord;
  documentIDNumbers.reserve(32);
}

/**
 * AVLTreeNode is constructed based on the incomingWord and the document ID
 * numbers and counts
 */
AVLTreeNode::AVLTreeNode(string incomingWord, vector<pair<int, int>>
  incomingDocs): height(0), left(nullptr), right(nullptr)
{
  indexedWord = incomingWord;
  documentIDNumbers.reserve(incomingDocs.size());
  for(int i = 0; i < incomingDocs.size(); i++)
  {
    documentIDNumbers.push_back(make_pair(incomingDocs.at(i).first,
      incomingDocs.at(i).second));
  }
}

/**
 * addDocumentNumber saves the incomingDocNumber to this AVLTreeNode
 */
void AVLTreeNode::addDocumentNumber(int incomingDocNumber)
{
  documentIDNumbers.push_back(make_pair(incomingDocNumber, 1));
}

/**
 * incrementCountOfWord increments the count of the word in the last document
 * ID
 */
void AVLTreeNode::incrementCountOfWord()
{
  documentIDNumbers.back().second++;
}

/**
 * getIndexedWord returns the word that is indexed by this AVLTreeNode
 */
string AVLTreeNode::getIndexedWord()
{
  return indexedWord;
}

/**
 * getLastDocumentID returns the last document ID saved to this AVLTreeNode
 */
int AVLTreeNode::getLastDocumentID()
{
  return documentIDNumbers.back().first;
}

/**
 * getCountOfLastID returns the count of the last document ID saved to this
 * AVLTreeNode
 */
int AVLTreeNode::getCountOfLastID()
{
  return documentIDNumbers.back().second;
}

/**
 * getDocumentIDNumbers returns a reference to the vector of document ID
 * numbers and counts
 */
vector<pair<int, int>>* AVLTreeNode::getDocumentIDNumbers()
{
  return &documentIDNumbers;
}
