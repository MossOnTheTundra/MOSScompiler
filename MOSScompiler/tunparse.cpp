/*	____________________________________________________________________________

	          Parser Component Implementation for the Micro Compiler

	                               tunparse.cpp

	                              Version 2020
								  created by MOSS

	The routines in this unit are based on those provided in the book
	"Crafting A Compiler" by Charles N. Fischer and Richard J. LeBlanc, Jr.,
	Benjamin Cummings Publishing Co. (1991).

	See Section 2.3-2.4, pp. 31-40.
	____________________________________________________________________________
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

#include "tunscan.h"   // Scanner class definition
#include "tunparse.h"  // Parser class definition
#include "tuncode.h"   // CodeGen class definition

extern Scanner scan; // global scanner declared in micro.cpp
extern CodeGen code; // global code generator declared in micro.cpp

Parser::Parser()
{
	tokenAvailable = false;  // no token initially available to parser
}

void Parser::Match(Token t)
{
	if (t != NextToken())
	{
	  cerr << " **** Didnt find the Token **** " << t << endl;
		SyntaxError(t);
	}
	else
		tokenAvailable = false;
}

Token Parser::NextToken()
{
	if (!tokenAvailable)
	{
		savedToken = scan.GetNextToken();
		tokenAvailable = true;
	}
		return savedToken;
}

void Parser::SyntaxError(Token t)
{
	cout << " *** Syntax Error Detected " << int(t) << ' '
		<< int(savedToken);
	listFile << " *** Syntax Error Detected " << int(t) << ' '
		<< int(savedToken);
	sourceFile.close();
	listFile.close();
	outFile.close();
	cin.ignore(256, '\n');
	cin.get();
	exit(1);  // error termination
}

void Parser::SystemGoal()
{
	ProgramProc();
	Match(EOF_SYM);
	code.Finish();  /*** CODE ***/
}

void Parser::ProgramProc()
{
	code.Start();  /*** CODE ***/
	Match(BEGIN_SYM);
	StatementList();
	Match(END_SYM);
}

void Parser::StatementList()
{
	Statement();
	for (;;)
	{  // loop exited by return
		switch (NextToken())
		{
		case ID:
		case READ_SYM:
		case WRITE_SYM:
			Statement();
			break;
		default:
			return;
		}
	}
}

void Parser::Statement()
{
	ExprRec identifier, expr;

	switch (NextToken())
	{
	case ID:
		Ident(identifier);
		Match(ASSIGN_OP);
		Expression(expr);
		code.Assign(identifier, expr);  /*** CODE ***/
		Match(SEMICOLON);
		break;
	case READ_SYM:
		Match(READ_SYM);
		Match(LPAREN);
		IdList();
		Match(RPAREN);
		Match(SEMICOLON);
		break;
	case WRITE_SYM:
															//std::cerr << " *** Write Symbol Found *** \n";
		Match(WRITE_SYM);
		Match(LPAREN);
		ExprList();
															//std::cerr << " *** Return from exprList() *** \n";
		Match(RPAREN); // error happening here
															//std::cerr << " *** Found RPAREN *** \n";
		Match(SEMICOLON);
		code.NewLine();  /*** CODE ***/
		break;
	default:
		SyntaxError(NextToken());
	}
}

void Parser::IdList()
{
	ExprRec identifier;

	Ident(identifier);
	code.ReadId(identifier);  /*** CODE ***/
	for (;;)
	{  // loop exited by return
		if (NextToken() == COMMA)
		{
			Match(COMMA);
			Ident(identifier);
			code.ReadId(identifier);  /*** CODE ***/
		}
		else
			return; // exit the function
	}
}

void Parser::MakeStr(ExprRec& e)
{
// string is already in scan.tokenBuffer
//passes string from buffer into string data structure object; stringTable
	code.ProcessString(e);
	code.WriteString(e);
		return;
}

void Parser::ExprList()
{
	ExprRec exprItem;
	ExprItem(exprItem);
	for (;;)
	{  // loop exited by return
		switch (NextToken())
		{
		case COMMA:
			Match(COMMA);
			ExprItem(exprItem);
		default:
			return;
		}
	}
}


void Parser::ExprItem(ExprRec& item)
{
	if(NextToken()!=STR_LITERAL)
    {
       	Expression(item);
        code.WriteExpr(item);
    }
    else
    {
	    Match(STR_LITERAL);
  	    MakeStr(item);
	}
    return;
}


void Parser::Expression(ExprRec& result)
{
	ExprRec leftOperand, rightOperand;
	OpRec op;

	Primary(result);
	for (;;)
	{  // loop exited by return
		if (NextToken() == PLUS_OP || NextToken() == MINUS_OP)
		{
			// from call to Primary of gen_infix
			leftOperand.kind = result.kind;
			leftOperand.val = result.val;
			leftOperand.name= result.name;
			AddOp(op);
			Primary(rightOperand);
         /*** CODE ***/
			code.GenInfix (leftOperand, op, rightOperand, result);
		}
		else
			return; // exit loop
	}
}

void Parser::Primary(ExprRec& result)
{
	switch (NextToken())
	{
	case LPAREN:
		Match(LPAREN);
		Expression(result);
		Match(RPAREN);
		break;
	case ID:
		Match(ID);
		code.ProcessId(result);  /*** CODE ***/
		break;
	case INT_LITERAL:
		Match(INT_LITERAL);
		code.ProcessLiteral(result);  /*** CODE ***/
		break;
	default:
		SyntaxError(NextToken());
	}
}

void Parser::AddOp(OpRec& op)
{
	switch (NextToken())
	{
	case PLUS_OP:
		Match(PLUS_OP);
		code.ProcessOp(op);  /*** CODE ***/
		break;
	case MINUS_OP:
		Match(MINUS_OP);
		code.ProcessOp(op);  /*** CODE ***/
		break;
	default:
		SyntaxError(NextToken());
	}
}

void Parser::Ident(ExprRec& result)
{
	Match(ID);
	code.ProcessId(result);  /*** CODE ***/
}
