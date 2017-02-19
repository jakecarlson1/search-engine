#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <dirent.h>

#include "indexinterface.h"
#include "avltreeindex.h"
#include "hashtableindex.h"
#include "documentparser.h"
#include "queryengine.h"
#include "searchreturnlist.h"

using namespace std;

class UserInterface
{
public:
  UserInterface();
  ~UserInterface();

  bool chooseMode();
  void enterMode(bool);
  bool isOpen();
  void readFileFromCommandLine(string);

private:
  IndexInterface* indexAccess;
  DocumentParser corpusReader;
  QueryEngine queryProcessor;
  string inputFileName;
  string directoryName;
  string persistantParsedFileList;
  vector<string> directoryFiles;
  vector<string> allParsedFiles;
  int totalNumberOfPages;
  int choice;
  bool sessionState;
  bool hasBeenSaved;

  bool maintenanceMode();
  bool interactiveMode();
  void chooseDocOrDir();
  void addDocument();
  void addDirectory();
  bool searchIndex();
  void saveIndex();
  void chooseDataStructure();
  void checkPersistantIndex();
  bool adjustIndex();
  void purgeIndex();
  bool displayDocuments(SearchReturnList*&);
  void printIndexStatistics();

};

#endif // USERINTERFACE_H
