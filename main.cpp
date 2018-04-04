//****************************************************************************************
//	INCLUDE FILES
//****************************************************************************************
#include	<fstream>
#include	<iostream>
#include	<sstream>
#include	<string>
#include    <iomanip>

#include	"DocumentIndex.h"

using namespace std;


//****************************************************************************************
//	MAIN PROGRAM
//****************************************************************************************
int main ()
{
    //************************************************************************************
    //	LOCAL DATA
    int				state;
    bool			isInteger=true, isKeyWord;
    ifstream        inputFile;
    string          fileName, outputFileName, word, result, s2, s3;
    ofstream        outputFile;
    int             wordIT;

    //************************************************************************************
    //	LISTS:  separators, operators, keywords
    char sepList[NUM_SEP]={'[',']',',', ':', '{', '}', ';','(',')'};
    char opList[NUM_OP]={'^','>','<','=','*','/','+','-'};
    string keywords[NUM_KEYWDS] = {"int", "boolean", "real", "if", "else", "endif", "return", "put", "get", "while", "true", "false", "function"};

    //************************************************************************************
    //	OBJECT CALLS
    DocumentFile    documentFile1;

    // prompt user to enter the data file name
    cout << "Enter the name of the file to read from: ";
    //cin >> fileName;

    cout << "Enter where you want to store the output file: " << endl;
    //cin >> outputFileName;

    //open the input file
    inputFile.open("/Users/phoebeshieh/ClionProjects/CPSC323_Proj1_v5/loadFile");


    //open the output file
    outputFile.open("/Users/phoebeshieh/ClionProjects/CPSC323_Proj1_v5/outputTest");
    outputFile << left << setw(20) << "Token";
    outputFile << left << setw(30) << "Lexeme" << endl;


    // If the file is opened successfully, process it
    if(inputFile)
    {   // loop until EOF is reached
        while(inputFile >> word)
        {
            // check if comment.  i.e., starts with '!'
            if(word[0]=='!') {
                while(word[word.length()-1] != '!'){
                    inputFile >> word;
                    cout << "word iterated through: " << word << endl;
                }
                if(word[word.length()-1]=='!')
                    inputFile >> word;
            }
            else{
                goto beginning;
            }
            beginning:
            int wordIT = 0;

            initialCheck:
            while(wordIT<word.length())
            {
                for(int n=0; n<NUM_SEP; n++)    // check if separator
                {
                    if (word[wordIT] == sepList[n])
                        goto check1;
                }
                for(int n=0; n<NUM_OP;n++)      // check if operator
                {
                    if (word[wordIT] == opList[n])
                        goto check2;
                }
                if(word[wordIT]>47 && word[wordIT]<58 || word[wordIT]=='.') // check if digit
                    goto check3;

                if(isalpha(word[wordIT]))   // check if letter
                    goto check4;

                check1: // is char a separator?
                for(int n=0; n<NUM_SEP; n++)
                {
                    if(word[wordIT]==sepList[n]) {
                        outputFile << left << setw(20) << "Separator";
                        outputFile << left << word[wordIT] << endl;
                        wordIT++;
                        goto initialCheck;
                    }
                    else if(word[wordIT]=='%') {
                        if(word[wordIT+1]=='%') {
                            outputFile << left << setw(20) << "Separator";
                            outputFile << left << word[wordIT] << word[wordIT++] << endl;
                            wordIT++;
                            goto initialCheck;
                        } else {
                            goto deadState;
                        }
                    }
                }

                check2:  // is char an operator?
                for(int n=0; n<NUM_OP;n++)
                {
                    if(word[wordIT]=='^')
                    {
                        int temp1=wordIT+1;
                        outputFile << left << setw(20) << "Operator";
                        outputFile << left << word[wordIT] << word[temp1] << endl;
                        if (wordIT+2 < word.length())
                        {
                            wordIT=wordIT+2;
                            goto initialCheck;
                        }else
                            goto deadState;
                    }
                    else if(word[wordIT]=='=')
                    {
                        if(word[wordIT+1]=='=' || word[wordIT+1]=='>' || word[wordIT+1]=='<')
                        {
                            int temp2=wordIT+1;

                            outputFile << left << setw(20) << "Operator";
                            outputFile << left << word[wordIT] << word[temp2] << endl;
                            if(wordIT+2 < word.length())
                            {
                                wordIT=wordIT+2;
                                goto initialCheck;
                            } else
                                goto deadState;
                        }
                        else
                        {
                            outputFile << left << setw(20) << "Operator";
                            outputFile << left << word[wordIT] << endl;
                            wordIT++;
                            goto initialCheck;
                        }
                    }
                    else if(word[wordIT]==opList[n])
                    {
                        outputFile << left << setw(20) << "Operator";
                        outputFile << left << setw(30) << word[wordIT] << endl;
                        wordIT++;
                        goto initialCheck;
                    }
                }
                check3: // is char a digit?
                s2 = "";
                while(wordIT < word.length() && ((word[wordIT]>47&&word[wordIT]<58) || word[wordIT]=='.')) {
                    if (word[wordIT] > 47 && word[wordIT] < 58 || word[wordIT] == '.') {
                        s2 += word[wordIT];
                        wordIT++;
                    }
                }
                for(int n=0; n<NUM_SEP; n++)   // check if separator
                    if(word[wordIT]==sepList[n])
                        goto FSMdigit;
                for(int n=0; n<NUM_OP; n++)     // check if operator
                    if(word[wordIT]==opList[n])
                        goto FSMdigit;
                if((word[wordIT]>65 && word[wordIT]<91) || (word[wordIT]>96 && word[wordIT]<123))           // check if letter
                    goto deadState;
                else
                    goto FSMdigit;

                FSMdigit:
                state = documentFile1.FSMdigit(s2);
                if(state==1 || state==3)
                {
                    for(int i=0; i<s2.length(); i++)
                    {
                        if(s2[i]=='.')
                        {
                            isInteger=false;
                            break;
                        } else
                            isInteger = true;
                    }
                    if(isInteger==true)
                    {
                        outputFile << left << setw(20) << "Integer";
                        outputFile << left << setw(30) << s2 << endl;
                    }
                    else
                    {
                        outputFile << left << setw(20) << "Real";
                        outputFile << left << setw(30) << s2 << endl;
                    }
                }
                if(wordIT<word.length())
                    goto initialCheck;

                check4: // is char a letter?
                isKeyWord=false;
                s3="";
                while(wordIT<word.length() && (isalpha(word[wordIT]) || isdigit(word[wordIT]) || word[wordIT]=='$'))  // check if letter, digit, $
                {
                    if(isalpha(word[wordIT]) || isdigit(word[wordIT]) || word[wordIT]=='$') {
                        s3+=word[wordIT];
                        wordIT++;
                    }
                }
                for(int n=0; n<NUM_SEP; n++)
                    if (word[wordIT] == sepList[n]) {
                        goto FSM_ID;
                    }

                for(int n=0; n<NUM_OP; n++)     // check if operator
                    if(word[wordIT]==opList[n]) {
                        goto FSM_ID;
                    }

                if(word[wordIT-1]=='$' || (word[wordIT-1]>65 && word[wordIT-1]<91) || (word[wordIT-1]>96 && word[wordIT-1]<123) || (word[wordIT-1]>47&&word[wordIT-1]<58)){
                    goto FSM_ID;
                }
                else{
                    goto deadState;
                }


                FSM_ID:
                state = documentFile1.FSMid(s3);
                if(state==1 || state==2 || state==4)
                {
                    for(int n=0; n<NUM_KEYWDS; n++)
                    {
                        if(s3==keywords[n]) {
                            outputFile << left << setw(20) << "Keyword";
                            outputFile << left << setw(30) << s3 << endl;
                            isKeyWord = true;
                            break;
                        }
                    }
                    if(isKeyWord==false)
                    {
                        outputFile << left << setw(20) << "Identifier";
                        outputFile << left << setw(30) << s3 << endl;
                    }
                }
                if(wordIT < word.length())
                    goto initialCheck;

                deadState:  // move onto next word
                wordIT = word.length();
            }
        }
        //close the file
        outputFile.close();
    }
    else  // file could not be found and opened
    {
        // display error message
        cout << "Error opening the file " << endl;
    }

    return(0);
}
