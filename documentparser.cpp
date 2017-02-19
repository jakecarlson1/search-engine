/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This DocumentParser class parses through an XML document and pulls key words
 * and information
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added implementation for openFile                        4/3
  *           +added implementation for the key private parsing
  *            functions findNextPage, saveTitle, findContributor
  *            saveUsername, findTextTag, readText, and endOfText
  *           +added stop word removal and stemming
  *     0.2   -removed just about everything                            4/5
  *           +added use of pugixml for parsing the xml document
  *     0.3   +added parseTextField                                     4/6
  *     0.4   -cleaned up class and moved stem and stop word            4/12
  *            functions to a dedicated class
  *     0.5   +tried to make parsing faster by reading in from the      4/13
  *            pugixml reference to the textfield (do more with this)
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "documentparser.h"

/**
 * constructor initializes numPagesRead
 */
DocumentParser::DocumentParser():
  numPagesRead(0)//, currentTextField(nullptr)
{

}

/**
 * destructor
 */
DocumentParser::~DocumentParser()
{

}

/**
 * openFile sets this.documentReader to inputFileName and returns 0 if the file
 * is not opened and 1 if the file is opened
 */
bool DocumentParser::openFile(string inputFileName)
{
  documentReader.open(inputFileName);
  if(!documentReader.is_open())
  {
    documentReader.close();
    return 0;
  }
  else
  {
    documentReader.close();
    return 1;
  }
}

/**
 * readFile receives a pointer to an IndexInterface object by reference. This
 * will allow this DocumentParser to send words to the IndexInterface for
 * placement in the index
 */
int DocumentParser::readFile(IndexInterface*& indexAccess,
  string inputFileName)
{
  //use pugiXML to parse the XML document into a DOM tree
  pugi::xml_document inputXMLDocument;
  pugi::xml_parse_result result = inputXMLDocument.load_file(
    inputFileName.c_str());
  //print the load result to the user incase any error were found int he file
  cout << "\033[31;1m" << "Load result:\t\t\t\t" << "\033[0m"
    << result.description() << endl;

  pugi::xml_node pagesOfDocument = inputXMLDocument.child("mediawiki");

  int docIDHolder = 0;
  //parse the text field of each page
  for(pugi::xml_node page = pagesOfDocument.child("page"); page;
    page = page.next_sibling("page"))
  {
    currentTextField = page.child("revision").child_value("text");

    docIDHolder = stoi(page.child_value("id"));
    documentMaintainer.insert(make_pair(docIDHolder, inputFileName));
    //currentTextField is given to parseTextField() to be parsed
    parseTextField(indexAccess, currentTextField, docIDHolder);
    indexAccess->incrementPageCount();
    numPagesRead++;
  }
  cout << "\033[30;1m" << "Total pages read:\t\t\t" << "\033[0m"
    << numPagesRead << endl;

  return numPagesRead;
}

/**
 * parseTextFiled parses the text field of a page. This function first
 * instantiates a string stream for reading out of the tet field word by word.
 * the word is then trimmed and stemmed and sent to the index
 */
void DocumentParser::parseTextField(IndexInterface*& indexAccess,
  string& incomingTextField, int& pageID)
{
  istringstream textFieldReader(incomingTextField);
  string textBuffer;

  while(textFieldReader >> textBuffer)
  {
    if(!wordAdjuster.isStopWord(textBuffer))
    {
      //trim and stem the textBuffer
      wordAdjuster.stemWord(textBuffer);
      //if the word is not alphabetical it is not saved to the index
      if(isalpha(textBuffer[0]) && isalpha(textBuffer.back()) &&
        isalpha(textBuffer[textBuffer.size()/2]))
      {
        //if the word is not a stop word it is placed in the index
        if(!wordAdjuster.isStopWord(textBuffer))
        {
          indexAccess->addWordToIndex(textBuffer, pageID);
        }
      }
    }
  }
}

/**
 * displayPage looks through the documents until the requested page ID is
 * found. The page is then printed
 */
void DocumentParser::displayPage(int docID, string dirRoot,
  vector<string>& documentBank)
{
  cout << "Document:" << endl;
  //first looks to see if the document docID is found on can be determined
  //instantly
  if(documentMaintainer.count(docID) > 0)
  {
    pugi::xml_document inputXMLDocument;
    inputXMLDocument.load_file(documentMaintainer.at(docID).c_str());

    pugi::xml_node pagesOfDocument = inputXMLDocument.child("mediawiki");
    for(pugi::xml_node page = pagesOfDocument.child("page"); page;
      page = page.next_sibling("page"))
    {
      //when the page is found, it is printed
      int docIDHolder = stoi(page.child_value("id"));
      if(docID == docIDHolder)
      {
        printPage(docID, page);
        return;
      }
    }
  }
  //or the whole list of documents is searched through
  else
  {
    for(int i = 0; i < documentBank.size(); i++)
    {
      pugi::xml_document inputXMLDocument;
      string location(dirRoot);
      location += documentBank.at(i);
      inputXMLDocument.load_file(location.c_str());

      pugi::xml_node pagesOfDocument = inputXMLDocument.child("mediawiki");
      for(pugi::xml_node page = pagesOfDocument.child("page"); page;
        page = page.next_sibling("page"))
      {
        //when the page is found it is printed
        int docIDHolder = stoi(page.child_value("id"));
        if(docID == docIDHolder)
        {
          printPage(docID, page);
          return;
        }
      }
    }
  }
}

/**
 * displayHeader looks through the documents until the requested page ID is
 * found. The header is then printed
 */
void DocumentParser::displayHeader(int docID, string dirRoot,
  vector<string>& documentBank)
{
  //first looks to see if the document docID is found on can be determined
  //instantly
  if(documentMaintainer.count(docID) > 0)
  {
    pugi::xml_document inputXMLDocument;
    inputXMLDocument.load_file(documentMaintainer.at(docID).c_str());

    pugi::xml_node pagesOfDocument = inputXMLDocument.child("mediawiki");
    for(pugi::xml_node page = pagesOfDocument.child("page"); page;
      page = page.next_sibling("page"))
    {
      //when the page is found, the header is printed
      int docIDHolder = stoi(page.child_value("id"));
      if(docID == docIDHolder)
      {
        printHeader(page);
        return;
      }
    }
  }
  //or the whole list of documents is searched through
  else
  {
    for(int i = 0; i < documentBank.size(); i++)
    {
      pugi::xml_document inputXMLDocument;
      string location(dirRoot);
      location += documentBank.at(i);
      inputXMLDocument.load_file(location.c_str());

      pugi::xml_node pagesOfDocument = inputXMLDocument.child("mediawiki");
      for(pugi::xml_node page = pagesOfDocument.child("page"); page;
        page = page.next_sibling("page"))
      {
        //when the page is found, the header is printed
        int docIDHolder = stoi(page.child_value("id"));
        if(docID == docIDHolder)
        {
          printHeader(page);
          return;
        }
      }
    }
  }
}

/**
 * printHeader prints relevant information about the xml_node page
 */
void DocumentParser::printHeader(pugi::xml_node& page)
{
  string timeStamp = page.child("revision").child_value("timestamp");
  timeStamp = timeStamp.substr(0, 10);
  cout << "\033[34m" << "Title: " << page.child_value("title") << endl;
  cout << "\033[32m" << "Author: " << page.child("revision")
    .child("contributor").child_value("username") << endl;
  cout << "\033[0m" << "Date added: " << timeStamp << endl;
}

/**
 * printPage prints the relevant information and text on xml_node page
 */
void DocumentParser::printPage(int docID, pugi::xml_node& page)
{
  currentTextField = page.child("revision").child_value("text");
  cout << endl << docID << ":" << endl;
  cout << "\033[34m" << "Title: " << page.child_value("title") << endl;
  cout << "\033[32m" << "Author: " << page.child("revision")
    .child("contributor").child_value("username") << endl;
  cout << "\033[0m" << currentTextField << endl;
}

/**
 * saveDocumentMaintainer saves the map of document ID numbers to the file they
 * are located in
 */
void DocumentParser::saveDocumentMaintainer()
{
  string nameOfPersistantFile("documentmaintainerfile.txt");
  ofstream maintainerSaver;
  maintainerSaver.open(nameOfPersistantFile);
  for(int i = 0; i < documentMaintainer.bucket_count(); i++)
  {
    for(auto j = documentMaintainer.begin(i); j != documentMaintainer.end(i);
      j++)
    {
      maintainerSaver << j->first << endl << j->second << endl;
    }
  }
  maintainerSaver.close();
}

/**
 * loadDocumentMaintainer loads the map of document ID numbers to the file they
 * are loacted in
 */
void DocumentParser::loadDocumentMaintainer()
{
  string nameOfPersistantFile("documentmaintainerfile.txt");
  string mapBuffer;
  ifstream maintainerLoader;
  maintainerLoader.open(nameOfPersistantFile);
  while(maintainerLoader >> mapBuffer)
  {
    int docIDHolder = stoi(mapBuffer);
    maintainerLoader >> mapBuffer;
    documentMaintainer.insert(make_pair(docIDHolder, mapBuffer));
  }
  maintainerLoader.close();
}
