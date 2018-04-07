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

//************************************************************************************
//	LOCAL DATA
int				state;
bool			isInteger = true, isKeyWord;
ifstream        inputFile;
string          fileName, outputFileName, word, result, s2, s3;
ofstream        outputFile;
int             wordIT;
bool            printon = true;

//************************************************************************************
//	LISTS:  separators, operators, keywords
char sepList[NUM_SEP] = { '[',']',',', ':', '{', '}', ';','(',')' };
char opList[NUM_OP] = { '^','>','<','=','*','/','+','-' };
string keywords[NUM_KEYWDS] = { "int", "boolean", "real", "if", "else", "endif", "return", "put", "get", "while", "true", "false", "function" };

//************************************************************************************
//	OBJECT CALLS
DocumentFile    documentFile1;


//****************************************************************************************
//	LEXER PROGRAM
//****************************************************************************************
void lexer ()
{
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

            while(wordIT<word.length())
            {
                for(int n=0; n<NUM_SEP; n++)    // check if separator
                {
                    if (word[wordIT] == sepList[n])
                        isSep();
                }
                for(int n=0; n<NUM_OP;n++)      // check if operator
                {
					if (word[wordIT] == opList[n]) 
						isOp();
                }
                if(word[wordIT]>47 && word[wordIT]<58 || word[wordIT]=='.') // check if digit
                    isInt();

                if(isalpha(word[wordIT]))   // check if letter
                    isID();
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
}

//****************************************************************************************
//	SYNTAX ANALYZER
//****************************************************************************************
bool Rat18s()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Rat18S>  ::=   <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>  ";
	}

	OptFunctDef();
	if (word == "%%")
	{
		wordIT++;
		OptDecList();
		StateList();
	}
	else return false; //error
}

bool OptFunctDef()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Opt Function Definitions> ::= <Function Definitions> |  <Empty>";
	}

	if (word[wordIT] != NULL)
	{
		FunctDef();
	}
	else
	{
		Empty();
	}
}

bool FunctDef()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Function Definitions>  ::= <Function> | <Function> <Function Definitions>";
	}

	Function();
	if (word[wordIT] != NULL)
	{
		Function();
	}
}

bool Function()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Function> ::= function  <Identifier>  [ <Opt Parameter List> ]  <Opt Declaration List>  <Body>";
	}

	if (word == "function")
	{
		wordIT++;
		isID;
		wordIT++;
		if (word[wordIT] == '[')
		{
			wordIT++;
			OptParamList();
			if (word[wordIT] == ']')
			{
				OptDecList();
				body();
			}
			else return false; //error
		}
		else return false; //error
	}
	else return false; //error
}

bool OptParamList()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Opt Parameter List> ::=  <Parameter List>    |     <Empty>";
	}

	if (word[wordIT] != NULL)
	{
		ParamList();
	}
	else
	{
		Empty();
	}
}

bool ParamList()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>";
	}

	IDs();
	if (word[wordIT] == ':')
	{
		wordIT;
		Qualifier();
	}
	else return false;
}

bool Parameter()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Parameter> ::=  <IDs > : <Qualifier> ";
	}

	IDs();
	if (word[wordIT] == ':')
	{
		wordIT++;
		Qualifier();
	}
	else return false; //error
}

bool Qualifier()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Qualifier> ::= int     |    boolean    |  real ";
	}

	if (isInt() || isBool())
	{
		return true;
	}
	else return false; //error
}

bool body()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Body>  ::=  {  < Statement List>  }";
	}

	if (word[wordIT] == '{')
	{
		wordIT++;
		StateList();
	}
}

bool OptDecList()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Opt Declaration List> ::= <Declaration List>   |    <Empty>";
	}

	if (DecList() == false)
	{
		Empty();
	}
	else return true;
}

bool DecList()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>";
	}

	Declaration();
	if (word[wordIT] != NULL)
	{
		DecList();
	}
}

bool Declaration()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Declaration> ::=   <Qualifier > <IDs>";
	}

	Qualifier();
	IDs();
}

bool IDs()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<IDs> ::=     <Identifier>    | <Identifier>, <IDs>";
	}

	isID();
	if (word[wordIT] != NULL)
	{
		IDs();
	}
}

bool StateList()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Statement List> ::=   <Statement>   | <Statement> <Statement List>";
	}

	Statement();
	if (word[wordIT] != NULL)
	{
		StateList();
	}
}

bool Statement()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While> ";
	}

	if (word[wordIT] == '{')//lots of ors == true
	{
		Compound();
	}
	else if (isID())
	{
		Assign();
	}
	else if (word[wordIT] == 'if')
	{
		If();
	}
	else if (word == "return")
	{
		Return();
	}
	else if (word[wordIT] == 'put')
	{
		Print();
	}
	else if (word[wordIT] == 'get')
	{
		Scan();
	}
	else if (word == "while")
	{
		While();
	}
	else return false; //error
}

bool Compound()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Compound> ::=   {  <Statement List>  }";
	}

	if (word[wordIT] == '{')
	{
		wordIT++;
		StateList();
	}
	else return false; //error
}

bool Assign()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Assign> ::=     <Identifier> = <Expression> ;";
	}

	if (isID())
	{
		//iterate
		if (word[wordIT] == '=')
		{
			//iterate
			Expression();
		}
		else return false; //error
	}
	else return false; //error
}

bool If()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<If> ::=     if  ( <Condition>  ) <Statement>   endif    |   if (<Condition>) <Statement>   else  <Statement>  endif";
	}

	if (word[wordIT] == 'if')
	{
		wordIT++;
		if (word[wordIT] == '(')
		{
			wordIT++;
			Condition();
			if (word[wordIT] == ')')
			{
				wordIT++;
				Statement();
				if (word[wordIT] == 'else')
				{
					wordIT++;
					Statement();
					if (word == "endif")
					{
						return true;
					}
					else return false; //error
				}
				else if (word == "endif")
				{
					return true;
				}
				else return false; //error
			}
			else return false; //error
		}
		else return false; //error
	}
	else return false; //error
}

bool Return()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Return> ::=  return ; |  return <Expression> ;";
	}

	if (word == "return")
	{
		wordIT++;
		if (word[wordIT] == ';')
		{
			return true;
		}
		else
		{
			Expression();
		}
	}
}

bool Print()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Print> ::=    put ( <Expression>);";
	}

	if (word[wordIT] == 'put')
	{
		wordIT++;
		Expression();
	}
	else return false; //error
}

bool Scan()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Scan> ::=    get ( <IDs> );";
	}

	if (word[wordIT] == 'get')
	{
		wordIT++;
		IDs();
	}
	else return false; //error
}

bool While()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<While> :: = while (<Condition>)  <Statement> ";
	}

	if (word == "while")
	{
		wordIT++;
		if (word[wordIT] == '(')
		{
			wordIT++;
			Condition();
			if (word[wordIT] == ')')
			{
				wordIT++;
				Statement();
			}
			else return false; //error
		}
		else return false; //error
	}
	else return false; //error
}

bool Condition() //iterate between calls?
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Condition> ::=     <Expression>  <Relop>   <Expression>";
	}

	Expression();
	Relop();
	Expression();
}

bool Relop()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<";
	}

	if (word[wordIT] == '==' || word[wordIT] == '^=' || word[wordIT] == '>' || word[wordIT] == '<' || word[wordIT] == '=>' || word[wordIT] == '=<')
	{
		return true;
	}
	else return false; //error
}

bool Expression()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Expression>  ::=    <Term>  <ExpressionPrime>";
	}

	Term();
	ExpPrime();
}

bool ExpPrime()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<ExpressionPrime>  ::=  + <Term> <ExpressionPrime>  |  + <Term> <ExpressionPrime>  |   <Empty> ";
	}

	if (word[wordIT] != NULL)
	{
		if (word[wordIT] == '+' || word[wordIT] == '-')
		{
			wordIT++;
			Term();
			ExpPrime();
		}
		else return false; //error
	}
	else
	{
		Empty();
	}
}

bool Term()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Term>  ::=    <Factor>  <TermPrime>";
	}

	Factor();
	TermPrime();
}

bool TermPrime()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<TermPrime>  ::=  * <Factor> <TermPrime>  |  / <Factor> <TermPrime>  |   <Empty> ";
	}

	if (word[wordIT] != NULL)
	{
		if (word[wordIT] == '*' || word[wordIT] == '/')
		{
			wordIT++;
			Factor();
			TermPrime();
		}
		else return false; //error
	}
	else
	{
		Empty();
	}
}

bool Factor()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Factor> ::=      -  <Primary>    |    <Primary>";
	}

	if (word[wordIT] == '-')
	{
		wordIT++;
		Primary();
	}
	else
	{
		Primary();
	}
}

bool Primary()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |  <Real> | true | false";
	}

	if (isID())
	{
		if (word[wordIT] == '(')
		{
			wordIT++;
			IDs();
			if (word[wordIT] == ')')
			{
				return true;
			}
		}
		else return true;
	}
	else if (isInt() || word == "true" || word == "false")
	{
		return true;
	}
	else if (word[wordIT] == '(')
	{
		wordIT++;
		Expression();
		if (word[wordIT] == ')')
		{
			return true;
		}
		else return false; //error
	}
	else return false; //error
}

bool Empty()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Empty>   ::= ";
	}

	return true;
}

//****************************************************************************************
//	TYPE CHECKS
//****************************************************************************************
void initialcheck()
{
	while (wordIT<word.length())
	{
		for (int n = 0; n<NUM_SEP; n++)    // check if separator
		{
			if (word[wordIT] == sepList[n])
				isSep();
		}
		for (int n = 0; n<NUM_OP; n++)      // check if operator
		{
			if (word[wordIT] == opList[n])
				isOp();
		}
		if (word[wordIT]>47 && word[wordIT]<58 || word[wordIT] == '.') // check if digit
			isInt();

		if (isalpha(word[wordIT]))   // check if letter
			isID();
	}
}

bool isID()
{
	check4: // is char a letter?
	isKeyWord = false;
	s3 = "";
	while (wordIT<word.length() && (isalpha(word[wordIT]) || isdigit(word[wordIT]) || word[wordIT] == '$'))  // check if letter, digit, $
	{
		if (isalpha(word[wordIT]) || isdigit(word[wordIT]) || word[wordIT] == '$')
		{
			s3 += word[wordIT];
			wordIT++;
		}
	}
	for (int n = 0; n < NUM_SEP; n++) //check if sep
	{
		if (word[wordIT] == sepList[n])
		{
			FSM_ID(s3);
		}
	}

	for (int n = 0; n < NUM_OP; n++)  // check if operator
	{
		if (word[wordIT] == opList[n])
		{
			FSM_ID(s3);
		}
	}

	if (word[wordIT - 1] == '$' || (word[wordIT - 1]>65 && word[wordIT - 1]<91) || (word[wordIT - 1]>96 && word[wordIT - 1]<123) || (word[wordIT - 1]>47 && word[wordIT - 1]<58))
	{
		FSM_ID(s3);
	}
	else
	{
		deadState();
	}
}

bool isInt()
{
	//check3: // is char a digit?
	s2 = "";
	while (wordIT < word.length())
	{
		if (word[wordIT] > 47 && word[wordIT] < 58 || word[wordIT] == '.')
		{
			s2 += word[wordIT];
			wordIT++;
			if (wordIT + 1 == word.length())
			{
				return true;
			}
		}
		else return false;

	}
	for (int n = 0; n<NUM_SEP; n++)   // check if separator
		if (word[wordIT] == sepList[n])
			FSMDigit(s2);
	for (int n = 0; n<NUM_OP; n++)     // check if operator
		if (word[wordIT] == opList[n])
			FSMDigit(s2);
	if ((word[wordIT]>65 && word[wordIT]<91) || (word[wordIT]>96 && word[wordIT]<123))           // check if letter
		deadState();
	else
		FSMDigit(s2);
}


bool isBool()
{

}

bool isSep()
{
	check1: // is char a separator?
	for (int n = 0; n<NUM_SEP; n++)
	{
		if (word[wordIT] == sepList[n])
		{
			outputFile << left << setw(20) << "Separator";
			outputFile << left << word[wordIT] << endl;
			wordIT++;
			initialcheck();
		}
		else if (word[wordIT] == '%')
		{
			if (word[wordIT + 1] == '%')
			{
				outputFile << left << setw(20) << "Separator";
				outputFile << left << word[wordIT] << word[wordIT++] << endl;
				wordIT++;
				initialcheck();
			}
			else
			{
				deadState();
			}
		}
	}
}

bool isOp()
{
	check2:  // is char an operator?
	for (int n = 0; n<NUM_OP; n++)
	{
		if (word[wordIT] == '^')
		{
			int temp1 = wordIT + 1;
			outputFile << left << setw(20) << "Operator";
			outputFile << left << word[wordIT] << word[temp1] << endl;
			if (wordIT + 2 < word.length())
			{
				wordIT = wordIT + 2;
				initialcheck();
			}
				
			else deadState();
		}
		else if (word[wordIT] == '=')
		{
			if (word[wordIT + 1] == '=' || word[wordIT + 1] == '>' || word[wordIT + 1] == '<')
			{
				int temp2 = wordIT + 1;

				outputFile << left << setw(20) << "Operator";
				outputFile << left << word[wordIT] << word[temp2] << endl;
				if (wordIT + 2 < word.length())
				{
					wordIT = wordIT + 2;
					initialcheck();
				}
				else
					deadState();
			}
			else
			{
				outputFile << left << setw(20) << "Operator";
				outputFile << left << word[wordIT] << endl;
				wordIT++;
				initialcheck();
			}
		}
		else if (word[wordIT] == opList[n])
		{
			outputFile << left << setw(20) << "Operator";
			outputFile << left << setw(30) << word[wordIT] << endl;
			wordIT++;
			initialcheck();
		}
	}
}

void FSMDigit(string s2)
{
	FSMdigit:
	state = documentFile1.FSMdigit(s2);
	if (state == 1 || state == 3)
	{
		for (int i = 0; i<s2.length(); i++)
		{
			if (s2[i] == '.')
			{
				isInteger = false;
				break;
			}
			else
				isInteger = true;
		}
		if (isInteger == true)
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
	if (wordIT<word.length())
		initialcheck();
}

void FSM_ID(string s3)
{
	FSM_ID:
	state = documentFile1.FSMid(s3);
	if (state == 1 || state == 2 || state == 4)
	{
		for (int n = 0; n<NUM_KEYWDS; n++)
		{
			if (s3 == keywords[n]) {
				outputFile << left << setw(20) << "Keyword";
				outputFile << left << setw(30) << s3 << endl;
				isKeyWord = true;
				break;
			}
		}
		if (isKeyWord == false)
		{
			outputFile << left << setw(20) << "Identifier";
			outputFile << left << setw(30) << s3 << endl;
		}
	}
	if (wordIT < word.length())
		initialcheck();
}

void deadState()
{
	deadState:  // move onto next word
	wordIT = word.length();
}

//****************************************************************************************
//	MAIN PROGRAM
//****************************************************************************************
int main()
{
	lexer();
	Rat18s();
	return 0;
}
