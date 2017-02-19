/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This IndexInterface class provides an interface between the UserInterface
 * and the other index classes, and the DocumentParser and the other index
 * classes
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added implementation for persistantIndexFound            4/3
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "indexinterface.h"

/**
 * constructor initializes values for the index interface, including the name
 * of the persistant file
 */
IndexInterface::IndexInterface():
  persistantFileName("persistantindexlog.txt"), numPagesInIndex(0),
  numWordsIndexed(0)
{

}

/**
 * destructor
 */
IndexInterface::~IndexInterface()
{

}

/**
 * persistantIndexFound returns 1 if the persistantindexlog.txt is found and
 * opened. 0 is returned if the file is not found
 */
bool IndexInterface::persistantIndexFound()
{
  persistantFileReader.open(persistantFileName.c_str());
  if(!persistantFileReader.is_open())
  {
    persistantFileReader.close();
    return 0;
  }
  else
  {
    persistantFileReader.close();
    return 1;
  }
}

/**
 * incrementPageCount increments the number of pages that the index has indexed
 */
void IndexInterface::incrementPageCount()
{
  numPagesInIndex++;
}

/**
 * incrementWordCount increments the number of words that the index has indexed
 */
void IndexInterface::incrementWordCount()
{
  numWordsIndexed++;
}

/**
 * getPageCount returns the total number of pages the index has indexed
 */
int IndexInterface::getPageCount()
{
  return numPagesInIndex;
}

/**
 * getWordCount returns the total number of pages the index has indexed
 */
int IndexInterface::getWordCount()
{
  return numWordsIndexed;
}

/**
 * addWordCount receives an integer to adjust the count of the number of words
 * the index has indexed by
 */
void IndexInterface::addWordCount(int adjustment)
{
  numWordsIndexed += adjustment;
}

/**
 * setPageCount sets the number of pages indexed by the index
 */
void IndexInterface::setPageCount(int count)
{
  numPagesInIndex = count;
}
