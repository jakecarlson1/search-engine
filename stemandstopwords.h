#ifndef STEMANDSTOPWORDS_H
#define STEMANDSTOPWORDS_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
//https://bitbucket.org/smassung/porter2_stemmer/src/88e796642033?at=master
#include "porter2_stemmer.h"

using namespace std;

class StemAndStopWords
{
public:
  StemAndStopWords();
  ~StemAndStopWords();

  void stemWord(string&);
  bool isStopWord(string&);

private:
  string inputBuffer;
  int sizeOfStopList;
  unordered_set<string> stopWordList;
  unordered_map<string, string> instantStem;

};

#endif // STEMANDSTOPWORDS_H
