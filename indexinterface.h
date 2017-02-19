#ifndef INDEXINTERFACE_H
#define INDEXINTERFACE_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class IndexInterface
{
public:
  IndexInterface();
  virtual ~IndexInterface() = 0;

  virtual void loadPersistantIndex() = 0;
  virtual void addWordToIndex(string&, int&) = 0;
  virtual int* searchForWord(string) = 0;
  virtual void saveIndexToDisk() = 0;
  virtual void printMostFrequentWords() = 0;

  bool persistantIndexFound();
  void incrementPageCount();
  void incrementWordCount();
  int getPageCount();
  int getWordCount();
  void addWordCount(int);
  void setPageCount(int);

  string persistantFileName;

private:
  ifstream persistantFileReader;
  int numPagesInIndex;
  int numWordsIndexed;

};

#endif // INDEXINTERFACE_H
