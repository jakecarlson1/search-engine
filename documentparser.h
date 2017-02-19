#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

#include "indexinterface.h"
#include "stemandstopwords.h"
//https://bitbucket.org/smassung/porter2_stemmer/src/88e796642033?at=master
#include "porter2_stemmer.h"
//http://pugixml.org
#include "pugixml.hpp"

using namespace std;

class DocumentParser
{
public:
  DocumentParser();
  ~DocumentParser();

  bool openFile(string);
  int readFile(IndexInterface*&, string);
  void displayPage(int, string, vector<string>&);
  void displayHeader(int, string, vector<string>&);
  void saveDocumentMaintainer();
  void loadDocumentMaintainer();

private:
  ifstream documentReader;
  StemAndStopWords wordAdjuster;
  unordered_map<int, string> documentMaintainer;
  int numPagesRead;
  string inputBuffer;
  string currentTitle;
  string currentUsername;
  string currentTextField;

  void parseTextField(IndexInterface*&, string&, int&);
  void printHeader(pugi::xml_node&);
  void printPage(int, pugi::xml_node&);

};

#endif // DOCUMENTPARSER_H
