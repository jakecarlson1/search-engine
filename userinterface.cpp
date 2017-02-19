/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This UserInterface class presents all of the control options to the user
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added implementation for the chooseMode and enterMode    4/3
  *            public functions and began work on the different modes
  *           +added inplementation for the basic maintenance mode
  *           +added implementation for addDocument
  *     0.2   +added readFileFromCommandLine                            4/6
  *     0.3   +added looping for mode types and options to cancel       4/11
  *            file input
  *     0.4   +began reading from directories                           4/12
  *     0.5   +added implementation for interactive mode and searching  4/13
  *     0.6   +added access to persistant index files                   4/14
  *     0.7   +divided the maintenance mode into distinct functions     4/16
  *           +added purge index implementation
  *           +added habanero asci art
  *     1.0   +added comments and organized header, cleaned UI, final   5/1
  ****************************************************************************/

#include "userinterface.h"

/**
 * constructor initializes values and displays the pepper logo
 */
UserInterface::UserInterface():
  indexAccess(nullptr), totalNumberOfPages(0), choice(0), sessionState(1),
  hasBeenSaved(1), persistantParsedFileList("persistantlistofparseddocs.txt")
{
  cout <<
      "                                                                      .......    " << endl
   << "                                                                      .III:.     " << endl
   << "          Habanero                                                     .ZI7..    " << endl
   << "             Search                                                     .I7+.    " << endl
   << "               Engine                                                   .I$=.    " << endl
   << "                                                 ........             ..,I$..    " << endl
   << "                                                ...,,..               .+IZ...    " << endl
   << "                                       .......~?I$$ZZZZ$7:.      .  .IIOO..      " << endl
   << "                                       ..~?I7$Z$$$7$$$ZO88$      .IIZOO:...      " << endl
   << "      .......... .. .   ..... ...:=+?7$$ZZ$7I?++===+?7$ZOO88I777IZ8O7...         " << endl
   << " . ..+$$77$$ZZOZ$777IIIII777777$ZZZ$$77II??+=~~=I7ZZZ8O88OOO8O8Z8$~. .           " << endl
   << " ..$7$$ZOZ7=~~++?77777$$$7II?+==+=~=~~=+?++?IZ88DDD8DDDDDNDOO78I.....            " << endl
   << " ..$8DDNNNNNDD8ZI++===++?++=~=+7$ZOO8DDDDDDDDDDDDNNNNNNNNNNDO$...                " << endl
   << "   .$ZOOO8DNDNNNNNNNDDDDDDDDDDDNDNDNNNNNNNNNNNNNNNNNDD88888OO7Z..                " << endl
   << "   ...+$ZOOOO88DNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNDD888888OOOOOZI...       ........ " << endl
   << "       ..7$ZZOOOO8888DDNNNNNNNNNNNNDDD88888888888888OOOOOOOZ$=.. ............... " << endl
   << "      .....,7$ZZZOOOOO8O888888888888888888888888888OOOOOOOOZ=................... " << endl
   << "           ...~$ZZZOOOOOOO8OO888888888888O8O8OOO8OOOOOOOOOO~::,,,,,...........   " << endl
   << "        .........:ZOOOOOOOOOOOOOOOOOOOOOOOOOOO88888888DDZ++=~~::,,.........      " << endl
   << "      .....,,,,:::~~~=+ZDDDD8888888888DDDDDNNNNNNND8OZ$$I??+=~:,,........        " << endl
   << "     .....,,,::~~===++??I7$ZZO8DNNMMMMMNNNNNDDDD88OOZ$7I+=~::,,.......           " << endl
   << "      .........,,:::~==++??II77777$777II?++==~~~::::,,,,,,.........              " << endl
   << "             .................................................                   " << endl
   << "                          ...............   ...                                  " << endl;
}

/**
 * destructor first asks the user if they want to save the index if they
 * haven't. The index is then deleted
 */
UserInterface::~UserInterface()
{
  if(hasBeenSaved == 0)
  {
    saveIndex();
  }
  delete indexAccess;
}

/**
 * isOpen returns the state of the session
 */
bool UserInterface::isOpen()
{
  return sessionState;
}

/**
 * readFileFromCommandLine promts the user if they would want to index a file
 * that was a command line argument
 */
void UserInterface::readFileFromCommandLine(string commandLineFile)
{
  cout << "A file was entered in the command line. Would you like to index it?"
   << endl;
  cout << "\t1. Yes\t\t2. No" << endl << "|>> ";
  cin >> choice;

  while(choice < 1 || choice > 2)
  {
    cout << "Invalid choice, please choose one of the follwing:" << endl;
    cout << "\t1. Yes\t\t2. No" << endl << "|>> ";
    cin >> choice;
  }

  if(choice == 1)
  {
    if(!corpusReader.openFile(commandLineFile))
    {
      cout << "That file could not be located." << endl;
    }
    else if(corpusReader.openFile(commandLineFile))
    {
      //a data structure must be choosen for the command line file
      chooseDataStructure();
      //timing starts for the parse time
      clock_t inputTimer;
      inputTimer = clock();
      //the command line file is saved to the list of parsed files
      allParsedFiles.push_back(commandLineFile);
      //the file is parsed
      totalNumberOfPages += corpusReader.readFile(indexAccess, commandLineFile);
      //the load time is displayed
      inputTimer = clock() - inputTimer;
      cout << "Load time: " << ((float)inputTimer)/CLOCKS_PER_SEC << " seconds"
        << endl;
      //the index has been updated to hasBeenSaved is set to false
      hasBeenSaved = 0;
    }
  }
}

/**
 * chooseMode prompts the user to choose whether to enter maintenance mode or
 * interactive mode and returns 0 if the prior is choosen and 1 if the later is
 * choosen
 */
bool UserInterface::chooseMode()
{
  // IndexInterface::persistantIndexFound(); //if-statement???
  cout << endl << "******************************** Search Engine"
    << " *********************************" << endl;
  cout << "Please choose the number corresponding to one of the following "
   << "modes:" << endl;
  cout << "\t1. Maintenance Mode" << endl;
  cout << "\t2. Interactive Mode" << endl;
  cout << "Maintenance Mode: Allows editing of the index." << endl;
  cout << "Interactive Mode: Allows searching of the index." << endl;
  cout << "\t\t\t\t\t\t\t0. Close Session" << endl << "|>> ";
  cin >> choice;

  while(choice < 0 || choice > 2)
  {
    cout << "Invalid choice, please choose one of the follwing:" << endl;
    cout << "\t1. Maintenance Mode" << endl;
    cout << "\t2. Interactive Mode" << endl;
    cout << "\t\t\t\t\t\t\t0. Close Session" << endl << "|>> ";
    cin >> choice;
  }

  if(choice == 0)
  {
    sessionState = 0;
    return 0;
  }
  else if(choice == 1)
  {
    return 0;
  }
  else if(choice == 2)
  {
    return 1;
  }
  return 0;
}

/**
 * enterMode is the public interface for entering either maintenance mode or
 * interactive mode
 */
void UserInterface::enterMode(bool modeType)
{
  if(modeType == 0)
  {
    while(maintenanceMode());
  }
  else if(modeType == 1)
  {
    while(interactiveMode());
  }
}

/**
 * maintenanceMode is the private function call that shown the maintenance mode
 * UI on the console
 */
bool UserInterface::maintenanceMode()
{
  cout << endl << "******************************* Maitenance Mode"
    << " ********************************" << endl;
  if(indexAccess == nullptr)
  {
    chooseDataStructure();
    checkPersistantIndex();
  }

  return adjustIndex();
}

/**
 * interactiveMode is the private function call that shown the interactive mode
 * UI on the console
 */
bool UserInterface::interactiveMode()
{
  cout << endl << "****************************** Interactive Mode"
    << " ********************************" << endl;
  cout << "Please choose how you would like to interact with the index:"
    << endl;
  cout << "\t1. Search" << endl;
  cout << "\t2. Print Index Statistics" << endl;
  cout << "\t\t\t\t\t\t\t0. Cancel" << endl << "|>> ";
  cin >> choice;

  while(choice < 0 || choice > 2)
  {
    cout << "Invalid choice, please choose one of the following:" << endl;
    cout << "\t1. Search" << endl;
    cout << "\t\t\t\t\t\t\t0. Cancel" << endl << "|>> ";
    cin >> choice;
  }

  if(choice == 0)
  {
    return 0;
  }
  else if(choice == 1)
  {
    cin.ignore(80, '\n');
    while(searchIndex());
  }
  else if(choice == 2)
  {
    printIndexStatistics();
  }

  return 1;
}

/**
 * chooseDocOrDir allows the user to choose between inputing either a single
 * document or a number of documents in a directory
 */
void UserInterface::chooseDocOrDir()
{
  cout << "Would you like to index a document or a directory?" << endl;
  cout << "\t1. Add document" << endl;
  cout << "\t2. Add directory" << endl;
  cout << "\t\t\t\t\t\t\t0. Cancel" << endl << "|>> ";
  cin >> choice;

  while(choice < 0 || choice > 2)
  {
    cout << "Invalid choice, please choose one of the following:" << endl;
    cout << "\t1. Add document" << endl;
    cout << "\t2. Add directory" << endl;
    cout << "\t\t\t\t\t\t\t0. Cancel" << endl << "|>> ";
    cin >> choice;
  }

  if(choice == 0)
  {
    return;
  }
  else if(choice == 1)
  {
    addDocument();
  }
  else if(choice == 2)
  {
    addDirectory();
  }
}

/**
 * addDocument requests a file name and then passes that file to the
 * DocumentParser
 */
void UserInterface::addDocument()
{
  cout << "Please enter a file name.\n|>> ";
  //cancel input
  cin >> inputFileName;
  while(!corpusReader.openFile(inputFileName))
  {
    cout << "That file could not be located." << endl;
    cout << "\t0. Cancel file input" << endl;
    cout << "Please enter another file.\n|>> ";
    cin >> inputFileName;

    if(inputFileName.compare("0") == 0)
    {
      break;
    }
  }
  if(corpusReader.openFile(inputFileName))
  {
    //timing starts for the parse time
    clock_t inputTimer;
    inputTimer = clock();
    //the file is saved to the list of parsed files
    allParsedFiles.push_back(inputFileName);
    //the file is parsed
    totalNumberOfPages += corpusReader.readFile(indexAccess, inputFileName);
    //the total load time is displayed
    inputTimer = clock() - inputTimer;
    cout << "\033[0m" << "Load time: " << ((float)inputTimer)/CLOCKS_PER_SEC << " seconds"
      << endl;
    //the index has been updated to hasBeenSaved is set to false
    hasBeenSaved = 0;
  }
}

/**
 * addDirectory allows for reading in of a folder or directory rather than a
 * single file
 */
void UserInterface::addDirectory()
{
  string directoryName;
  cout << "Please enter a directory.\n|>> ";
  cin >> directoryName;

  // directoryName = "./Wiki_Dumps/FullDump1000pgs/";
  // directoryName = "./Wiki_Dumps/First3k/";

  DIR* directoryAccess = opendir(directoryName.c_str());

  while(directoryAccess == nullptr)
  {
    directoryName.clear();
    cout << "Directory not found: " << directoryName << endl;
    cout << "\t0. Cancel file input" << endl;
    cout << "Please enter another directory.\n|>> ";
    cin >> directoryName;

    if(directoryName.compare("0") == 0)
    {
      return;
    }
    directoryAccess = opendir(directoryName.c_str());
  }
  cout << "Directory: " << directoryName << endl;
  //the name of every file in the directory is determined
  if(directoryAccess != nullptr)
  {
    struct dirent* dirp;
    dirp = readdir(directoryAccess);
    while(dirp != nullptr)
    {
      directoryFiles.push_back(string(dirp->d_name));
      dirp = readdir(directoryAccess);
    }
    //timing starts for the parse time
    clock_t inputTimer;
    inputTimer = clock();
    //every file in the directory is parsed
    for(int i = 0; i < directoryFiles.size(); i++)
    {
      //the file path is generated
      string dirFileName = directoryName;
      dirFileName += directoryFiles[i];
      //the file path is added to the list of all parsed files
      cout << "\033[34;1m" << "File:\t\t\t\t\t" << "\033[0m" << dirFileName << endl;
      allParsedFiles.push_back(dirFileName);
      //the file is parsed
      totalNumberOfPages += corpusReader.readFile(indexAccess, dirFileName);
      //the elapsed load time is displayed
      cout << "\033[30;1m" << "Time elapsed:\t\t\t\t" << "\033[0m" << ((float)clock() - inputTimer)/CLOCKS_PER_SEC
        << " seconds" << endl;
      cout << "\033[0m" << "----------------------------------------" <<
        "----------------------------------------" << endl;
    }
    //the total load time is displayed
    inputTimer = clock() - inputTimer;
    cout << "Load time: " << ((float)inputTimer)/CLOCKS_PER_SEC << " seconds"
      << endl;
    //the index has been updated to hasBeenSaved is set to false
    hasBeenSaved = 0;
  }
  closedir(directoryAccess);
}

/**
 * searchIndex searched the index for the word entered by the user
 */
bool UserInterface::searchIndex()
{
  string searchRequest;
  cout << "\nSearch index" << "\t\t\t0. Cancel Search\n" << endl;
  cout << "Please enter a search.\n|>> ";
  getline(cin, searchRequest);
  if(searchRequest.compare("0") == 0)
  {
    return 0;
  }
  else
  {
    cout << searchRequest << endl;
    SearchReturnList* searchReturn = queryProcessor.issueSearch(indexAccess,
      searchRequest);
    while(displayDocuments(searchReturn));
    return 1;
  }
}

/**
 * displayDocuments first shows the user the list of documents returned by the
 * QueryEngine and asks the user which document they would like to view. It
 * then sends the requested doc ID to the DocumentParser to find
 */
bool UserInterface::displayDocuments(SearchReturnList*& searchReturn)
{
  //the document id, tfidf value, and page header are displayed for all search
  //returns
  for(int i = 0; i < searchReturn->getReturnList().size(); i++)
  {
    cout << "\033[30;1m" << "Document ID: " << searchReturn->
      getReturnList().at(i).first << " tfidf score: " <<
      searchReturn->getReturnList().at(i).second << endl;
    corpusReader.displayHeader(searchReturn->getReturnList().at(i).first,
      directoryName, allParsedFiles);
    cout << "\033[0m" << endl;
  }
  //promts the user to chosoe a document to view
  cout << "Which document would you like to view?" << endl;
  cout << "Enter the Document ID of the page you would like to view." << endl;
  cout << "\t\t\tEnter '0' to exit" << endl << "|>> ";
  cin >> choice;
  if(choice == 0)
  {
    cin.ignore(80, '\n');

    return 0;
  }
  else
  {
    string waitBuffer;
    corpusReader.displayPage(choice, directoryName, allParsedFiles);
    cout << endl << "\033[30;1m" << "Enter any button to leave page"
      << "\033[0m" << endl << "|>> ";
    cin >> waitBuffer;
    return 1;
  }
}

/**
 * saveIndex allows the user to save the index to a persistant file
 */
void UserInterface::saveIndex()
{
  if(totalNumberOfPages != 0)
  {
    cout << "Would you like to save this index?" << endl;
    cout << "\t1. Yes" << endl;
    cout << "\t2. No" << endl << "|>> ";
    cin >> choice;

    while(choice < 1 || choice > 2)
    {
      cout << "Invalid choice, please choose one of the following:" << endl;
      cout << "\t1. Yes, save index" << endl;
      cout << "\t2. No, do not save index" << endl << "|>> ";
      cin >> choice;
    }
    //the index and list of parsed files are both saved to persistant files
    if(choice == 1)
    {
      indexAccess->saveIndexToDisk();
      corpusReader.saveDocumentMaintainer();

      ofstream parsedFilesSaver;
      parsedFilesSaver.open(persistantParsedFileList);
      for(int i = 0; i < allParsedFiles.size(); i++)
      {
        parsedFilesSaver << allParsedFiles.at(i) << endl;
      }
      parsedFilesSaver.close();
      hasBeenSaved = 1;
    }
    else if(choice == 2)
    {
      return;
    }
  }
}

/**
 * chooseDataStructure allows the user to choose the data structure they would
 * like the index to be loaded into
 */
void UserInterface::chooseDataStructure()
{
  cout << "Please choose a data structure type for the index:" << endl;
  cout << "\t1. AVL Tree" << endl;
  cout << "\t2. Hash Table" << endl << "|>> ";
  cin >> choice;

  while(choice < 1 || choice > 2)
  {
    cout << "Invalid choice, please choose one of the following:" << endl;
    cout << "\t1. AVL Tree" << endl;
    cout << "\t2. Hash Table" << endl << "|>> ";
    cin >> choice;
  }

  if(choice == 1)
  {
    indexAccess = new AVLTreeIndex;
  }
  else if(choice == 2)
  {
    indexAccess = new HashTableIndex;
  }
}

/**
 * checkPersistantIndex asks the user if they would like to load a persistant
 * index if one is found
 */
void UserInterface::checkPersistantIndex()
{
  if(indexAccess->persistantIndexFound())
  {
    cout << "A persistant index has been found, would you like to load it?"
      << endl;
    cout << "\t1. Yes" << endl;
    cout << "\t2. No" << endl << "|>> ";
    cin >> choice;

    while(choice < 1 || choice > 2)
    {
      cout << "Invalid choice, please choose one of the following:" << endl;
      cout << "\t1. Yes, load index" << endl;
      cout << "\t2. No, do not load index" << endl << "|>> ";
      cin >> choice;
    }
    //the index is loaded from the persistant file and the list of parsed files
    //is loaded
    if(choice == 1)
    {
      indexAccess->loadPersistantIndex();
      corpusReader.loadDocumentMaintainer();

      ifstream parsedFilesLoader;
      parsedFilesLoader.open(persistantParsedFileList);
      string parsedFileBuffer;

      while(parsedFilesLoader >> parsedFileBuffer)
      {
        allParsedFiles.push_back(parsedFileBuffer);
      }
      parsedFilesLoader.close();
    }
  }
}

/**
 * adjustIndex allows the user to interact with the index, either adding
 * documents or deleting the index
 */
bool UserInterface::adjustIndex()
{
  cout << "How would you like to adjust the index?" << endl;
  cout << "\t1. Add to index" << endl;
  cout << "\t2. Purge index" << endl;
  cout << "\t\t\t\t\t\t\t0. Cancel" << endl << "|>> ";
  cin >> choice;

  while(choice < 0 || choice > 2)
  {
    cout << "Invalid choice, please choose one of the following:" << endl;
    cout << "\t1. Add to index" << endl;
    cout << "\t2. Purge index" << endl;
    cout << "\t\t\t\t\t\t\t0. Cancel" << endl << "|>> ";
    cin >> choice;
  }
  if(choice == 0)
  {
    saveIndex();
    return 0;
  }
  else if(choice == 1)
  {
    chooseDocOrDir();
    return 1;
  }
  else if(choice == 2)
  {
    purgeIndex();
    return 0;
  }
  return 0;
}

/**
 * purgeIndex allows the user to delete the loaded index, but first prompts the
 * user to save the index if the current index has not been saved
 */
void UserInterface::purgeIndex()
{
  if(hasBeenSaved == 0 && indexAccess != nullptr)
  {
    cout << "You are about to delete a index that has not been saved." << endl;
    saveIndex();
  }

  cout << "Are you sure you want to delete the index?" << endl;
  cout << "\t1. Yes" << endl;
  cout << "\t2. No" << endl << "|>> ";
  cin >> choice;

  while(choice < 1 || choice > 2)
  {
    cout << "Invalid choice, please choose one of the following:" << endl;
    cout << "\t1. Yes, delete index" << endl;
    cout << "\t2. No, do not delete index" << endl << "|>> ";
    cin >> choice;
  }
  //the index is deleted and the list of files are cleared
  if(choice == 1)
  {
    if(indexAccess != nullptr)
    {
      delete indexAccess;
      indexAccess = nullptr;
    }
    allParsedFiles.clear();
    directoryFiles.clear();
  }
}

/**
 * printIndexStatistics prints all relevant index statistics
 */
void UserInterface::printIndexStatistics()
{
  cout << "Index Statistics:" << endl;
  cout << "Number of pages indexed: " << indexAccess->getPageCount() << endl;
  cout << "Number of words indexed: " << indexAccess->getWordCount() << endl;
  cout << "Top 50 most frequent words:" << endl;
  indexAccess->printMostFrequentWords();
  cout << endl;
}
