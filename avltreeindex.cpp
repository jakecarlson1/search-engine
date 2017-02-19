/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This AVLTreeIndex class is maintains the AVL tree index data structure
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added basic implementation                               4/12
  *     0.2   +added implementation for searching                       4/13
  *     0.3   +added saving to and loading from index                   4/14
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "avltreeindex.h"

/**
 * this constructor initiallizes root to nullptr
 */
AVLTreeIndex::AVLTreeIndex():
  root(nullptr)
{

}

/**
 * destructor deallocates the memory reserved for this AVL Tree
 */
AVLTreeIndex::~AVLTreeIndex()
{
  if(root != nullptr)
  {
    deleteNode(root);
    root = nullptr;
  }
}

/**
 * addWordToIndex is the public member function for saving a word and its
 * associated document ID number to the index
 */
void AVLTreeIndex::addWordToIndex(string& incomingWord, int& incomingDocID)
{
  insertWord(incomingWord, incomingDocID, root);
}

/**
 * insertWord is the provate member function for saving a word and its
 * associated document ID numebr to the index. It recursively looks through the
 * index until it either finds where the word exists in the index or finds the
 * place to put the word. The tree is rebalanced if needed
 */
void AVLTreeIndex::insertWord(string& incomingWord, int& incomingDocID,
  AVLTreeNode*& curr)
{
  //word is saved if nullptr is found before the word is
  if(curr == nullptr)
  {
    curr = new AVLTreeNode(incomingWord);
    curr->addDocumentNumber(incomingDocID);
    incrementWordCount();
  }
  else
  {
    int compareValue = incomingWord.compare(curr->indexedWord);
    //if the incomingWord goes to the left of curr
    if(compareValue < 0)
    {
      //insertWord is called on the left child
      insertWord(incomingWord, incomingDocID, curr->left);
      //sees if the tree has been made unbalanced
      if(getHeight(curr->left) - getHeight(curr->right) == 2)
      {
        if(incomingWord.compare(curr->left->indexedWord) < 0)
        {
          rotateWithLeftChild(curr);  //case 1
        }
        else
        {
          doubleWithLeftChild(curr);  //case 3
        }
      }
    }
    //if the incomingWord goes to the right of curr
    else if(compareValue > 0)
    {
      //insertWord is called on the right child
      insertWord(incomingWord, incomingDocID, curr->right);
      //sees if the tree has been made unbalanced
      if(getHeight(curr->right) - getHeight(curr->left) == 2)
      {
        if(incomingWord.compare(curr->right->indexedWord) > 0)
        {
          rotateWithRightChild(curr); //case 4
        }
        else
        {
          doubleWithRightChild(curr); //case 2
        }
      }
    }
    //if the word is found the document numbers are adjusted
    else if(compareValue == 0)
    {
      //if the document ID is already recorded by the word, its count is
      //incremented
      if(incomingDocID == curr->getLastDocumentID())
      {
        curr->incrementCountOfWord();
      }
      //if it is not found the new document ID is added
      else
      {
        curr->addDocumentNumber(incomingDocID);
      }
      //update the 50 most frequent words
      if(termFrequencies.count(incomingWord) > 0)
      {
        termFrequencies.at(incomingWord)++;
      }
      else
      {
        termFrequencies.insert(make_pair(incomingWord, 1));
      }
    }
  }
  //the height of the node is updated
  curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
}

/**
 * getHeight returns the height of the leaf. If the leaf does not exist, -1 is
 * returned as the height
 */
inline int AVLTreeIndex::getHeight(AVLTreeNode*& leaf)
{
  if(leaf == nullptr)
  {
    return -1;
  }
  else
  {
    return leaf->height;
  }
}

/**
 * max quickly determines the maximum of two values
 */
inline int AVLTreeIndex::max(int left, int right)
{
  return (left > right)? left : right;
}

/**
 * rotateWithLeftChild rotates the alpha node with its left child
 */
inline void AVLTreeIndex::rotateWithLeftChild(AVLTreeNode*& alpha)
{
  AVLTreeNode* temp = alpha->left;
  alpha->left = temp->right;
  temp->right = alpha;

  alpha->height = max(getHeight(alpha->left), getHeight(alpha->right)) + 1;
  temp->height = max(getHeight(temp->left), alpha->height) + 1;

  alpha = temp;
}

/**
 * rotateWithRightChild rotates the alpha node with its right child
 */
inline void AVLTreeIndex::rotateWithRightChild(AVLTreeNode*& alpha)
{
  AVLTreeNode* temp = alpha->right;
  alpha->right = temp->left;
  temp->left = alpha;

  alpha->height = max(getHeight(alpha->left), getHeight(alpha->right)) + 1;
  temp->height = max(getHeight(temp->right), alpha->height) + 1;

  alpha = temp;
}

/**
 * doubleWithLeftChild restructures the tree if it is in a case 3 unbalance
 */
inline void AVLTreeIndex::doubleWithLeftChild(AVLTreeNode*& alpha)
{
  rotateWithRightChild(alpha->left);
  rotateWithLeftChild(alpha);
}

/**
 * doubleWithRightChild restructures the tree if it is in a case 2 unbalance
 */
inline void AVLTreeIndex::doubleWithRightChild(AVLTreeNode*& alpha)
{
  rotateWithLeftChild(alpha->right);
  rotateWithRightChild(alpha);
}

/**
 * searchForWord recieves a word and then searches the index for it. If the
 * word is found then an integer array is formed that has the document ID
 * numbers and counts in them and the address is returned
 */
int* AVLTreeIndex::searchForWord(string searchWord)
{
  //the word is searched for
  AVLTreeNode* searchWordNode = searchNodeForWord(searchWord, root);
  if(searchWordNode == nullptr)
  {
    return nullptr;
  }
  else
  {
    //vecotr pointer that holds the documentID numbers of the search word
    vector<pair<int, int>>* searchWordDocNumbers =
      searchWordNode->getDocumentIDNumbers();
    //the number od doc ID numbers is found and the returnArray is formed
    int numberOfDocs = searchWordDocNumbers->size();
    int* returnArray = new int[(2 * numberOfDocs) + 1];
    int j = 0;
    returnArray[j] = numberOfDocs;
    j++;
    for(int i = 0; i < numberOfDocs; i++)
    {
      returnArray[j] = searchWordDocNumbers->at(i).first;
      j++;
      returnArray[j] = searchWordDocNumbers->at(i).second;
      j++;
    }
    return returnArray;
  }
}

/**
 * searchNodeForWord recursively searches the index for the search word
 */
AVLTreeNode* AVLTreeIndex::searchNodeForWord(string searchWord,
  AVLTreeNode* curr)
{
  //if the word isn't found, nullptr is returned
  if(curr == nullptr)
  {
    return nullptr;
  }
  else
  {
    //search left
    if(searchWord.compare(curr->indexedWord) < 0)
    {
      return searchNodeForWord(searchWord, curr->left);
    }
    //search right
    else if(searchWord.compare(curr->indexedWord) > 0)
    {
      return searchNodeForWord(searchWord, curr->right);
    }
    //if the word is found it is returned
    else
    {
      return curr;
    }
  }
}

/**
 * saveIndexToDisk saves the index to a file specified by the IndexInterface
 * class
 */
void AVLTreeIndex::saveIndexToDisk()
{
  ofstream indexSaver;
  indexSaver.open(persistantFileName);
  //the number of pages read is saved
  indexSaver << getPageCount() << endl;
  //the data in this AVL Tree is saved
  saveNode(indexSaver, root);
  indexSaver.close();
}

/**
 * saveTree is the public member function for saving the AVL Tree to a
 * specified ofstream object
 */
void AVLTreeIndex::saveTree(ofstream& indexSaver)
{
  if(root != nullptr)
  {
    saveNode(indexSaver, root);
  }
}

/**
 * saveNode recursively saves the index to indexSaver
 */
void AVLTreeIndex::saveNode(ofstream& indexSaver, AVLTreeNode* curr)
{
  //the left node is saved
  if(curr->left != nullptr)
  {
    saveNode(indexSaver, curr->left);
  }

  //the current node is saved
  //the word is saved
  indexSaver << curr->getIndexedWord() << endl;
  //the document ID numbers and counts are saved to the file
  vector<pair<int, int>>* currentDocIDs = curr->getDocumentIDNumbers();
  indexSaver << currentDocIDs->size() << endl;
  for(int i = 0; i < currentDocIDs->size(); i++)
  {
    indexSaver << currentDocIDs->at(i).first << " "
      << currentDocIDs->at(i).second << " ";
  }
  indexSaver << endl;

  //the right node is saved
  if(curr->right != nullptr)
  {
    saveNode(indexSaver, curr->right);
  }
}

/**
 * loadPersistantIndex loads the saved index from the persistant index file
 * into the index
 */
void AVLTreeIndex::loadPersistantIndex()
{
  string inputBuffer;
  string intBuffer;
  ifstream indexLoader;
  indexLoader.open(persistantFileName);
  //the page count is set
  indexLoader >> inputBuffer;
  setPageCount(stoi(inputBuffer));
  while(indexLoader >> inputBuffer)
  {
    vector<pair<int, int>> docIDBuffer;
    indexLoader >> intBuffer;
    int numberOfDocs = stoi(intBuffer);
    int wordFreq = 0;
    //the vector of doc IDs and counts is generated
    docIDBuffer.reserve(numberOfDocs);
    for(int i = 0; i < numberOfDocs; i++)
    {
      //the document ID number is loaded
      indexLoader >> intBuffer;
      int docBuffer = stoi(intBuffer);
      //the document ID count is loaded
      indexLoader >> intBuffer;
      int docCount = stoi(intBuffer);
      //the pair is added to the vector that will be given to the word
      docIDBuffer.push_back(make_pair(docBuffer, docCount));
      wordFreq += docCount;
    }
    //the word is inserted into the index
    insertFullWord(wordFreq, inputBuffer, docIDBuffer, root);
  }
  indexLoader.close();
}

/**
 * pushWordToTree is the public memebr function for inserting a full word into
 * the tree
 */
void AVLTreeIndex::pushWordToTree(int& wordFreq, string& incomingWord,
  vector<pair<int, int>>& incomingDocBuffer)
{
  insertFullWord(wordFreq, incomingWord, incomingDocBuffer, root);
}

/**
 * insertFullWord inserts a word and a document ID number vecotr into the index
 */
void AVLTreeIndex::insertFullWord(int& wordFreq, string& incomingWord,
  vector<pair<int, int>>& incomingDocBuffer, AVLTreeNode*& curr)
{
  //full word is saved
  if(curr == nullptr)
  {
    curr = new AVLTreeNode(incomingWord, incomingDocBuffer);
    termFrequencies.insert(make_pair(incomingWord, wordFreq));
  }
  //the index is traversed until the word can be placed in the index
  else
  {
    int compareValue = incomingWord.compare(curr->indexedWord);
    if(compareValue < 0)
    {
      insertFullWord(wordFreq, incomingWord, incomingDocBuffer, curr->left);
      if(getHeight(curr->left) - getHeight(curr->right) == 2)
      {
        if(incomingWord.compare(curr->left->indexedWord) < 0)
        {
          rotateWithLeftChild(curr);  //case 1
        }
        else
        {
          doubleWithLeftChild(curr); //case 3
        }
      }
    }
    else if(compareValue > 0)
    {
      insertFullWord(wordFreq, incomingWord, incomingDocBuffer, curr->right);
      if(getHeight(curr->right) - getHeight(curr->left) == 2)
      {
        if(incomingWord.compare(curr->right->indexedWord) > 0)
        {
          rotateWithRightChild(curr); //case 4
        }
        else
        {
          doubleWithRightChild(curr); //case 2
        }
      }
    }
    //if the words are equal, add the incomingDocVector to the word
  }
  curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
}

/**
 * deleteNode uses post-order traversal to delete this AVL Tree
 */
void AVLTreeIndex::deleteNode(AVLTreeNode*& curr)
{
  //delete left subtree
  if(curr->left != nullptr)
  {
    deleteNode(curr->left);
  }
  //delete right subtree
  if(curr->right != nullptr)
  {
    deleteNode(curr->right);
  }
  //delete current node
  delete curr;
}

/**
 * printMostFrequentWords determines the 50 most frequent words in the index
 * and then prints them
 */
void AVLTreeIndex::printMostFrequentWords()
{
  vector<pair<string, int>> frequentWords = getMostFrequentWords();

  //the most frequent words are printed
  for(int i = 0; i < frequentWords.size(); i++)
  {
    cout << i + 1 << ". " << frequentWords.at(i).first << " || " <<
      frequentWords.at(i).second << endl;
  }
}

/**
 * getMostFrequentWords determines the 50 most frequent words in the index
 */
vector<pair<string, int>> AVLTreeIndex::getMostFrequentWords()
{
  int currLowEnd = 0;
  vector<pair<string, int>> frequentWords;
  //termFrequencies is traversed to find the the 50 most frequent words
  for(int i = 0; i < termFrequencies.bucket_count(); i++)
  {
    for(auto j = termFrequencies.begin(i); j != termFrequencies.end(i); j++)
    {
      //if the current count of the word, word count pair is greater than the
      //count of the least frequent word in the frequentWords vector, then
      //it is pushed to the vector
      if(j->second > currLowEnd)
      {
        frequentWords.push_back(make_pair(j->first, j->second));
        //if the size of the frequentWords vector exceeds 60, the vector is
        //sorted and resized to 50. currLowEnd is updated
        if(frequentWords.size() > 60)
        {
          sort(frequentWords.begin(), frequentWords.end(), compareByCount());
          frequentWords.resize(50);
          currLowEnd = frequentWords.back().second;
        }
      }
    }
  }
  //the vector is sorted a final time and resized of it is larger than 50
  //elements
  sort(frequentWords.begin(), frequentWords.end(), compareByCount());
  if(frequentWords.size() > 50)
  {
    frequentWords.resize(50);
  }
  return frequentWords;
}
