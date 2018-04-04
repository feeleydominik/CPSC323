//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<iostream>
#include	<string>
#include    <iomanip>

#include	"DocumentIndex.h"

using namespace std;


//****************************************************************************************
//	DocumentFile::digitIntegerColumn
//****************************************************************************************

int DocumentFile::digitIntegerCol(char c)
{
    int col;

    if(c>47 && c<58)    // check if c is digit
        col = 1;
    else if(c=='.')     // check if c is decimal
        col = 2;
    else
        col = 3;        // check if c is neither digit nor decimal

    return col;
}

//****************************************************************************************
//	DocumentFile::letterColumn
//****************************************************************************************
int DocumentFile::letterCol(char c)
{
    int col;

    if(isalpha(c))
        col = 0;
    else if(isdigit(c))
        col = 1;
    else if(c=='$')
        col = 2;

    return col;
}


//****************************************************************************************
//	DocumentFile::digitIntFSM
//****************************************************************************************
int DocumentFile::FSMdigit(string s)
{
    int state = 0, col; //starting states
    string convertS;

    int stateTableDigit[ROWS_STATES][COLUMN_INPUT]= {
            {0, 1, 4},
            {1, 1, 2},
            {2, 3, 4},
            {3, 3, 4},
            {4, 4, 4} };

    for(int i=0; i<s.length(); i++) {
        if((s[i]>64&&s[i]<91) || (s[i]>96&&s[i]<123))       // checks if s[i] is alpha
        {
            state = 5;
            break;
        }
        else if (s[i]>47 && s[i]<58)  // checks if s[i] is digit
        {
            col = digitIntegerCol(s[i]);
            state = stateTableDigit[state][col];
        }
        else if(s[i]=='.')
        {
            col = digitIntegerCol(s[i]);
            state = stateTableDigit[state][col];
        }
    }

    return state;
}

//****************************************************************************************
//	DocumentFile::FSMid
//****************************************************************************************
int DocumentFile::FSMid(string s)
{
    string s2;
    int state = 0, col = 0; // starting state;

    int stateTableID[6][3]={
            1, 5, 5,
            2, 3, 4,
            2, 3, 4,
            2, 3, 4,
            5, 5, 5,
            5, 5, 5
    };

    for(int i=0; i<s.length();i++)
    {
        if(isalpha(s[i]) || isdigit(s[i]) ||s[i]=='$')
        {
            col = letterCol(s[i]);
            state = stateTableID[state][col];
        } else
        {
            state = 6;  // dead state
            break;
        }
    }

    return state;
}