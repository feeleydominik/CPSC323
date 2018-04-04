
#include	<fstream>
#include	<iostream>
#include	<sstream>
#include	<string>
#include    <iomanip>

using namespace std;


//**********************************************************************************************************************************


bool printon = true;


bool Rat18s()
{
	if(printon)
	{
		outputFile << left << setw(20) << "<Rat18S>  ::=   <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>  ";
	}

	OptFunctDef();
	if (word == '%%')
	{
		//iterate
		OptDecList();
		StateList();
	}
	else return false; //error
}

bool OptFunctDef()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Opt Function Definitions> ::= <Function Definitions>     |  <Empty>";
	}

	if (word != NULL)
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
	if (word != NULL)
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
		//iterate
		isID();
		//iterate
		if (word == '[')
		{
			//iterate
			OptParamList();
			if (word == ']')
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

	if (word != NULL)
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
	if (word == ':')
	{
		//iterate
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
	if (word == ':')
	{
		//iterate
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

	if (isint(word) || isbool(word) || isreal(word))
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

	if (word == '{')
	{
		//iterate
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
	if (word != NULL)
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
	if (word != NULL)
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
	if (word != NULL)
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

	if(word == '{')//lots of ors == true
	{
		Compound();
	}
	else if (isID())
	{
		Assign();
	}
	else if (word == 'if')
	{

	}
	else if (word == 'return')
	{

	}
	else if (word == 'put')
	{

	}
	else if (word == 'get')
	{

	}
	else if (word == 'while')
	{

	}
	else return false; //error
}

bool Compound()
{
	if (printon)
	{
		outputFile << left << setw(20) << "<Compound> ::=   {  <Statement List>  }";
	}

	if (word == '{')
	{
		//iterate
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
		if (word == '=')
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

	if (word == 'if')
	{
		//iterate
		if (word == '(')
		{
			//iterate
			Condition();
			if (word == ')')
			{
				//iterate
				Statement();
				if (word == 'else')
				{
					//iterate
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
		//iterate
		if (word == ';')
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

	if (word == 'put')
	{
		//iterate
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

	if (word == 'get')
	{
		//iterate
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
		//iterate
		if (word == '(')
		{
			//iterate
			Condition();
			if (word == ')')
			{
				//iterate
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

	if (word == '==' || word == '^=' || word == '>' || word == '<' || word == '=>' || word == '=<')
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

	if (word != NULL)
	{
		if (word == '+' || word == '-')
		{
			//iterate 
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

	if (word != NULL)
	{
		if (word == '*' || word == '/')
		{
			//iterate 
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

	if (word == '-')
	{
		//iterate
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
		if (word == '(')
		{
			//iterate
			IDs();
			if (word == ')')
			{
				return true;
			}
		}
		else return true;
	}
	else if (isInt() || isReal() || word == 'true' || word == "false")
	{
		return true;
	}
	else if (word == '(')
	{
		//iterate
		Expression();
		if (word == ')')
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