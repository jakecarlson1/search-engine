/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This HashTableIndex class maintains the hash table index data structure
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added basic implementation                               4/18
  *     0.2   +added implementation for persistant index handling       4/25
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "hashtableindex.h"

/**
 * constructor initailizes an array of AVLTreeIndex pointers
 */
HashTableIndex::HashTableIndex():
  numberOfBuckets(1440000)
{
  hashTableIndex = new AVLTreeIndex*[numberOfBuckets];
  for(long i = 0; i < numberOfBuckets; i++)
  {
    hashTableIndex[i] = new AVLTreeIndex;
  }
}

/**
 * destructor deallocates the array of AVLTreeIndex pointers
 */
HashTableIndex::~HashTableIndex()
{
  for(long i = 0; i < numberOfBuckets; i++)
  {
    delete hashTableIndex[i];
  }
}

/**
 * hashString returns the key hash for the string incomingWord
 */
long HashTableIndex::hashString(string& incomingWord)
{
  hash<string> hasher;
  size_t hashed = hasher(incomingWord);
  return hashed % numberOfBuckets;
}

/**
 * addWordToIndex determines which bucket to place incomingWord in and then
 * adds the word to the AVLTreeIndex bucket. The word count is updated
 */
void HashTableIndex::addWordToIndex(string& incomingWord, int& incomingDocID)
{
  long bucketNumber = hashString(incomingWord);
  addWordCount(-1 * hashTableIndex[bucketNumber]->getWordCount());
  hashTableIndex[bucketNumber]->addWordToIndex(incomingWord, incomingDocID);
  addWordCount(hashTableIndex[bucketNumber]->getWordCount());
}

/**
 * loadPersistantIndex loads the saved index from the persistant index file
 * into the index
 */
void HashTableIndex::loadPersistantIndex()
{
  string inputBuffer;
  string intBuffer;
  ifstream indexLoader;
  indexLoader.open(persistantFileName);
  //page count is set
  indexLoader >> inputBuffer;
  setPageCount(stoi(inputBuffer));
  while(indexLoader >> inputBuffer)
  {
    vector<pair<int, int>> docIDBuffer;
    indexLoader >> intBuffer;
    int numberOfDocs = stoi(intBuffer);
    int wordFreq = 0;
    //the vector of doc IDs and counts is generated
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
    long bucketNumber = hashString(inputBuffer);
    hashTableIndex[bucketNumber]->pushWordToTree(wordFreq, inputBuffer,
      docIDBuffer);
  }
  indexLoader.close();
}

/**
 * saveIndexToDisk saves the index to a file specified by the IndexInterface
 * class
 */
void HashTableIndex::saveIndexToDisk()
{
  ofstream indexSaver;
  indexSaver.open(persistantFileName);
  //page count is saved
  indexSaver << getPageCount() << endl;
  //hash table is saved
  for(long i = 0; i < numberOfBuckets; i++)
  {
    //each AVL Tree bucket is saved
    if(hashTableIndex[i] != nullptr)
    {
      hashTableIndex[i]->saveTree(indexSaver);
    }
  }
  indexSaver.close();
}

/**
 * searchForWord recieves a string and determines which bucket to search. That
 * bucket is searched
 */
int* HashTableIndex::searchForWord(string searchWord)
{
  long bucketNumber = hashString(searchWord);
  return hashTableIndex[bucketNumber]->searchForWord(searchWord);
}

/**
 * printMostFrequentWords determines the most frequent words in the Hash Table
 * and prints them
 */
void HashTableIndex::printMostFrequentWords()
{
  vector<pair<string, int>> frequentWords;
  //the 50 most frequent words from each bucket are found and merged with a
  //persistant list that is sorted and resized
  for(long i = 0; i < numberOfBuckets; i++)
  {
    vector<pair<string, int>> tempFreqWords =
      hashTableIndex[i]->getMostFrequentWords();
    //the vectors are merged
    frequentWords.insert(frequentWords.end(), tempFreqWords.begin(),
      tempFreqWords.end());
    //if the size of the frequentWords vector exceeds 60, the vector is
    //sorted and resized to 50
    if(frequentWords.size() > 60)
    {
      sort(frequentWords.begin(), frequentWords.end(), compareByCount());
      frequentWords.resize(50);
    }
  }
  //the vector is sorted a final time and resized of it is larger than 50
  //elements
  sort(frequentWords.begin(), frequentWords.end(), compareByCount());
  if(frequentWords.size() > 50)
  {
    frequentWords.resize(50);
  }
  //the most frequent words are printed
  for(int i = 0; i < frequentWords.size(); i++)
  {
    cout << i + 1 << ". " << frequentWords.at(i).first << " || " <<
      frequentWords.at(i).second << endl;
  }
}
