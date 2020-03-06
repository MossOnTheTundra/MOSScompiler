/*	____________________________________________________________________________

	                 Scanner Component for the MicroA Compiler

	                                tunscan.h

	                              Version 2020
 								created by MOSS

	The routines in this unit are based on those provided in the book
	"Crafting A Compiler" by Charles N. Fischer and Richard J. LeBlanc, Jr.,
	Benjamin Cummings Publishing Co. (1991).

	See Section 2.2, pp. 25-29.
	____________________________________________________________________________
*/

#ifndef SCANNER
#define SCANNER
#include <string>

const int ID_STRING_LEN = 32;

enum Token  { BEGIN_SYM, END_SYM, EOF_SYM, BOOL_SYM, DEC_SYM, BY_SYM, CASE_SYM, OTHERWISE_SYM,
				READ_SYM, WRITE_SYM, SELECT_SYM, ENDSELECT_SYM, 
				COLON_SYM, DO_SYM, FOR_SYM, BY_SYM,
				LBANANA, RBANANA, SEMICOLON, COMMA, FROM_SYM, RANGE_SYM, STEP_SYM, 					ENDFOR_SYM, FLOAT_SYM, BREAK_SYM
				ASSIGN_OP, PLUS_OP, MINUS_OP, IN_SYM, INPUT_SYM, INT_SYM, NEWLINE_SYM, 					OUTPUT_SYM, STR_SYM, RIGHT_SYM,WRONG_SYM,
				REPEAT_SYM, UNTIL_SYM, WHILE_SYM, LOOP_SYM, ENDLOOP_SYM, ENDWHILE_SYM, 					IF_SYM, THEN_SYM, ELSE_SYM, 
				MULT_OP, REALDIV_OP, LT_OP, LE_OP, GT_OP, GE_OP, EQ_OP, NE_OP, 					ENDSTMT_SYM, ENDIF_SYM, LSTAPLE, RSTAPLE, 
				ID, INT_LIT, BOOL_LIT, STRING_LIT, FLOAT_LIT };

class TunScanner
{
public:

	string	        tokenBuffer, // current token
			lineBuffer;  // current line
	int		lineNumber;  // line counter

	TunScanner();
	// Initializes the scanner object.

	Token GetNextToken();
	// Retrieves the next token available in the input stream.

private:

   void BufferChar(char c);
   // Appends character c to the end of the token buffer provided that the
   // buffer is not full.

   void BufferString(char c);
   // Appends upper and lower case char c to the end of the token buffer
   // for transport to stringTable 

	void MatchString();
	// takes string and places into tokenBuffer
	//

   Token CheckReserved();
   // Checks the identifier in the token buffer to see if it is a reserved word
   // or a user-defined identifier and returns the appropriate token.

   void ClearBuffer();
   // Erases the contents of the tokenBuffer.

   void LexicalError(char& c);
   // Processes a lexical error at character c by ignoring that character and
   // getting the next character.

   char NextChar();
   // Gets next available character from the source file.
};
#endif
