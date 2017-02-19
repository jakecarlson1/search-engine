/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This StemAndStopWords class maintains the list of stop words and stems words
 * given to it
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +adjusted functions from DocumentParser for this class    4/12
  *     0.2   +made use of unordered_set to reduce the time             4/12
  *            complexity of isStopWord from O(n) to O(1)
  *     0.3   +converted to inline in header                            4/14
  *     0.4   +made use of unordered_map to instantly stem words whos   4/15
  *            stem has already been determined
  *     1.0   +added comments and organized header, final               4/30
  ****************************************************************************/

#include "stemandstopwords.h"

/**
 * constructor reads in the file stopwordlist.txt and populates an
 * unordered_set that has all of the stop words in it for comparison later
 */
StemAndStopWords::StemAndStopWords():
  sizeOfStopList(0)
{
  ifstream stopListReader;
  stopListReader.open("stopwordlist.txt");
  stopListReader >> inputBuffer;

  sizeOfStopList = stoi(inputBuffer);
  for(int i = 0; i < sizeOfStopList; i++)
  {
    stopListReader >> inputBuffer;
    stopWordList.insert(inputBuffer);
  }
  stopListReader.close();
}

/**
 * constructor
 */
StemAndStopWords::~StemAndStopWords()
{

}

/**
 * stemWord trims and stems words given to it using the Porter2 Stemming
 * algorithm. stemWord also implements an unordered_map to save words that have
 * already been stemmed to make stemming quicker
 */
void StemAndStopWords::stemWord(string& incomingWord)
{
  if(instantStem.count(incomingWord) > 0)
  {
    incomingWord = instantStem.at(incomingWord);
  }
  else
  {
    string temp = incomingWord;
    //trim and stem the textBuffer
    Porter2Stemmer::trim(incomingWord);
    Porter2Stemmer::stem(incomingWord);
    instantStem.insert(make_pair(temp, incomingWord));
  }
}

/**
 * isStopWord checks to see if the word in incomingWord is in the stop word
 * list and returns 1 if the word is a stop word. 0 is returned if the word in
 * textbuffer is not a stop word
 */
bool StemAndStopWords::isStopWord(string& incomingWord)
{
  if(stopWordList.count(incomingWord) > 0)
  {
    return 1;
  }
  return 0;
}
