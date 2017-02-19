/**
 * Search Engine
 * Created by: Jake Carlson
 * Date: 2016-04-02
 *
 * This program indexes over 200,000 pages of a wikibooks dump and then allows
 * for searching through the file. The user of this program has the option of
 * storing the index in either an AVL tree or a hash table.
 */

 /*****************************************************************************
  * CHANGE LOG
  *
  * Version   Changes                                                  Date
  *     0.1   +added first instantiation of the user interface and      4/3
  *            choosing/entering a version of the UI
  *     0.2   +added indexing a document from command line              4/6
  *     0.3   +made adjustments to porter2_stemmer to make faster       4/14
  *     0.4   +the corpus can now be parsed in under 4 minutes into     4/15
  *            an AVLTree
  ****************************************************************************/

#include <iostream>

#include "userinterface.h"

using namespace std;

int main(int argc, char* argv[])
{
  //this bool determines the state the UI is in, 0 is maintenace mode, 1 is
  //interactive mode
  bool modeType = 0;

  UserInterface session;
  //command line gets  processed
  if(argc > 1)
  {
    string commandLineFile (argv[1]);
    session.readFileFromCommandLine(commandLineFile);
  }
  //main session loop
  while(session.isOpen())
  {
    modeType = session.chooseMode();
    if(session.isOpen())
    {
      session.enterMode(modeType);
    }
  }

  return 0;
}
