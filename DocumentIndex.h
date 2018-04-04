#ifndef	DocumentIndex_h
#define	DocumentIndex_h

//****************************************************************************************
//	INCLUDE FILES
//****************************************************************************************
#include	<fstream>
#include	<string>

using namespace std;

//****************************************************************************************
//	CONSTANT DEFINITIONS
//****************************************************************************************

const int ROWS_STATES = 5, COLUMN_INPUT = 3;
const int NUM_SEP = 9, NUM_OP = 8, NUM_KEYWDS=13;


//****************************************************************************************
//	CLASSES, TYPEDEFS AND STRUCTURES
//****************************************************************************************
class	DocumentFile
{
public:
    int     digitIntegerCol(char c);
    int     FSMdigit(string s);
    int     FSMid(string s);
    int     letterCol(char c);

private:

};


#endif