/*	____________________________________________________________________________

	         Scanner Component Implementation for the MicroA Compiler

	                               tunscan.cpp

	                              Version 2020
								created by MOSS

	The routines in this unit are based on those provided in the book
	"Crafting A Compiler" by Charles N. Fischer and Richard J. LeBlanc, Jr.,
	Benjamin Cummings Publishing Co. (1991).

	See Section 2.2, pp. 25-29.
	____________________________________________________________________________
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

#include "tunscan.h"

// *******************
// **  Constructor  **
// *******************

TunScanner::TunScanner()
{
	tokenBuffer = "";
	lineBuffer = "";
	lineNumber = 0;
}

// ********************************
// **  Private Member Functions  **
// ********************************

void TunScanner::BufferChar(char c)
{
	                                                 //std::cerr << " *** Buffer Char ***\n";
	if (tokenBuffer.length() < ID_STRING_LEN)
		tokenBuffer += toupper(c);
}

void TunScanner::BufferString(char c)
{
													//std::cerr << " *** Buffer Char ***\n";
	if (tokenBuffer.length() < ID_STRING_LEN)
		tokenBuffer += c;
}

Token TunScanner::CheckReserved()
{
	if (tokenBuffer == "greenlight") return BEGIN_SYM;
	if (tokenBuffer == "redlight") return END_SYM;
	if (tokenBuffer == "instream") return READ_SYM;
	if (tokenBuffer == "outstream") return WRITE_SYM;
	if (tokenBuffer == "bool") return BOOL_SYM;
	if (tokenBuffer == "break") return BREAK_SYM;
	if (tokenBuffer == "by") return BY_SYM;
	if (tokenBuffer == "case") return CASE_SYM;
	if (tokenBuffer == "endfor") return ENDFOR_SYM;
	if (tokenBuffer == "endif") return ENDIF_SYM;
	if (tokenBuffer == "endloop") return ENDLOOP_SYM;
	if (tokenBuffer == "endselect") return ENDSELECT_SYM;
	if (tokenBuffer == "endwhile") return ENDWHILE_SYM;
	if (tokenBuffer == "float") return FLOAT_LIT;
	if (tokenBuffer == "for") return FOR_SYM;
	if (tokenBuffer == "from") return FROM_SYM;
	if (tokenBuffer == "if") return IF_SYM;
	if (tokenBuffer == "in") return IN_SYM;
	if (tokenBuffer == "int") return INT_LIT;
	if (tokenBuffer == "loop") return LOOP_SYM;
	if (tokenBuffer == "newline") return NEWLINE_SYM;
	if (tokenBuffer == "otherwise") return OTHERWISE_SYM;
	if (tokenBuffer == "repeat") return REPEAT_SYM;
	if (tokenBuffer == "then") return THEN_SYM;
	if (tokenBuffer == "right") return RIGHT_SYM;
	if (tokenBuffer == "str") return STRING_LIT;
	if (tokenBuffer == "until") return UNTIL_SYM;
	if (tokenBuffer == "while") return WHILE_SYM;
	if (tokenBuffer == "wrong") return WRONG_SYM;
	if (tokenBuffer == "yellowlight") return DEC_SYM;
	return ID;
}

void TunScanner::ClearBuffer()
{
	tokenBuffer = "";
}

void TunScanner::MatchString()
{



}


void TunScanner::LexicalError(char& c)
{
	cout << " *** Lexical Error: '" << c
		<< "' ignored at position " << int(lineBuffer.size())
		<< " on line #" << lineNumber+1 << '.' << endl;
	listFile << " *** Lexical Error: '" << c
		<< "' ignored at position " << int(lineBuffer.size())
		<< " on line #" << lineNumber+1 << '.' << endl;
	c = NextChar();
}

char TunScanner::NextChar()
{
	char c;

	sourceFile.get(c);
	if (c == '\n')
	{
		listFile.width(6);
		listFile << ++lineNumber << "  " << lineBuffer << endl;
		lineBuffer = "";
	}
	else
		lineBuffer += c;
	return c;
}

// *******************************
// **  Public Member Functions  **
// *******************************

Token TunScanner::GetNextToken()
{
	char currentChar, c;

	ClearBuffer();
	currentChar = NextChar();
	while (!sourceFile.eof())
	{
		if (isspace(currentChar))
			currentChar = NextChar();     // do nothing
		else if (isalpha(currentChar))
		{                                // identifier
			BufferChar(currentChar);
			c = sourceFile.peek();
			while (isalnum(c) || c == '_')
			{
				currentChar = NextChar();
				BufferChar(currentChar);
				c = sourceFile.peek();
			}
			return CheckReserved();
		}
		else if (isdigit(currentChar))
		{                                // integer literal
			BufferChar(currentChar);
			c = sourceFile.peek();
			while (isdigit(c))
			{
				currentChar = NextChar();
				BufferChar(currentChar);
				c = sourceFile.peek();
			}
			return INT_LITERAL;
		}
		else if (currentChar == '(')
			return LBANANA;
		else if (currentChar == ')')
			return RBANANA;
		else if (currentChar == ';')
			return SEMICOLON;
		else if (currentChar == ',')
			return COMMA;
		else if (currentChar == '+')
		{
			BufferChar(currentChar);
			return PLUS_OP;
		}
		else if (currentChar == '=')
		
				currentChar = NextChar();
				return ASSIGN_OP;
			
		else if (currentChar == '"')
				{
						currentChar = NextChar();// moves from " to first char in string
						BufferString(currentChar);
						c = sourceFile.peek();
						while (c != '"')
						{
						    currentChar = NextChar();
							BufferString(currentChar);
							c = sourceFile.peek();
						}
						currentChar = NextChar();
				    return STRING_LIT;
				}
		else if (currentChar == '^')  // figure out how to add multiple line  \^ ... ^\
			if (sourceFile.peek() == '^') // comment
				do  // skip comment
					currentChar = NextChar();
				while (currentChar != '\n');
		else if (currentChar == '-') 
		{
			BufferChar(currentChar);      // minus operator
			return MINUS_OP;
		}
		else
			LexicalError(currentChar);
	} // end while
	return EOF_SYM;
}
