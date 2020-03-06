/*	____________________________________________________________________________

	                Parser Component for the Micro Compiler

	                                tunparse.h

	                              Version 2020

	                          created by MOSS

	The routines in this unit are based on those provided in the book
	"Crafting A Compiler" by Charles N. Fischer and Richard J. LeBlanc, Jr.,
	Benjamin Cummings Publishing Co. (1991).

	See Section 2.3-2.4, pp. 31-40.
	____________________________________________________________________________

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~  tundra Grammar 2020 Augmented Productions  ~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


     1.   <<goal>>            --> <program> "$eof$"

     2.   <program>           --> [Start] "greenlight" <dec_list> <stmt_list>
                                         "redlight" [Finish]

     3.   <stmt_list>         --> <statement> <stmt_tail>

     4.   <stmt_tail>         --> <statement> <stmt_tail>

     5.   <stmt_tail>         --> ""

     6.   <statement>         --> <simple_stmt>

     7.   <statement>         --> <struct_stmt>

     8.   <simple_stmt>       --> <assign_stmt>

     9.   <simple_stmt>       --> <input_stmt>

    10.   <simple_stmt>       --> <output_stmt>

    11.   <simple_stmt>       --> <newline_stmt>

    12.   <simple_stmt>       --> <break_stmt>

    13.   <assign_stmt>       --> <variable> "=" <expression> [Assign] ";"

    14.   <input_stmt>        --> "instream" "(" <var_list> ")" ";"

    15.   <var_list>          --> <variable> [InputVar] <var_list_tail>

    16.   <var_list_tail>     --> "," <variable> [InputVar] <var_list_tail>

    17.   <var_list_tail>     --> ""

    18.   <variable>          --> "ID" <variable_tail> [ProcessVar]

    19.   <variable_tail>     --> "[" <expression> "]"

    20.   <variable_tail>     --> ""

    21.   <output_stmt>       --> "outstream" "(" <item_list> ")" ";"

    22.   <item_list>         --> <expression> [WriteExpr] <item_list_tail>

    23.   <item_list_tail>    --> "," <expression> [WriteExpr] <item_list_tail>

    24.   <item_list_tail>    --> ""

    25.   <newline_stmt>      --> "newline" [NewLine] ";"

    26.   <break_stmt>        --> "break" [Break] ";"

    27.   <struct_stmt>       --> <if_stmt>

    28.   <struct_stmt>       --> <while_stmt>

    29.   <struct_stmt>       --> <repeat_stmt>

    30.   <struct_stmt>       --> <for_stmt>

    31.   <struct_stmt>       --> <select_stmt>

    32.   <struct_stmt>       --> <loop_stmt>

    33.   <if_stmt>           --> "if" <condition> "then" [ProcessIf] <stmt_list>
                                         <else_clause> "endif" [IfEnd]

    34.   <else_clause>       --> "else" [ProcessElse] <stmt_list>

    35.   <else_clause>       --> ""

    36.   <repeat_stmt>       --> "repeat" [RepeatBegin] <stmt_list> "until"
                                         <condition> [RepeatEnd] ";"

    37.   <while_stmt>        --> "while" [WhileBegin] <condition> [WhileTest] "do"
                                         <stmt_list> "endwhile" [WhileEnd]

    38.   <for_stmt>          --> "for" <variable> "in" <value_range> [ForBegin]
                                         "step" <variable> [ForUpdate] "do"
                                         <stmt_list> "endfor" [ForEnd]

    39.   <value_range>       --> <variable> ".." <variable>

    40.   <loop_stmt>         --> "loop" [LoopBegin] <stmt_list> "endloop" [LoopEnd]

    41.   <condition>         --> <expression> <cond_tail> [SetCondition]

    42.   <cond_tail>         --> <rel_op> [ProcessOp] <expression>

    43.   <cond_tail>         --> ""

    44.   <expression>        --> <factor> <expr_tail>

    45.   <expr_tail>         --> <add_op> [ProcessOp] <factor> [GenInfix]
                                         <expr_tail>

    46.   <expr_tail>         --> ""

    47.   <factor>            --> <primary> <factor_tail>

    48.   <factor_tail>       --> <mult_op> [ProcessOp] <primary> [GenInfix]
                                         <factor_tail>

    49.   <factor_tail>       --> ""

    50.   <primary>           --> <literal>

    51.   <primary>           --> <variable>

    52.   <primary>           --> "(" <expression> ")"

    53.   <type>              --> "int"

    54.   <type>              --> "bool"

    55.   <type>              --> "float"

    56.   <type>              --> "str"

    57.   <literal>           --> "INT_LIT"

    58.   <literal>           --> "BOOL_LIT"

    59.   <literal>           --> "FLOAT_LIT"

    60.   <literal>           --> "STRING_LIT"

    61.   <mult_op>           --> "*"

    62.   <mult_op>           --> "/"

    63.   <add_op>            --> "+"

    64.   <add_op>            --> "-"

    65.   <rel_op>            --> "<"

    66.   <rel_op>            --> "<="

    67.   <rel_op>            --> ">"

    68.   <rel_op>            --> ">="

    69.   <rel_op>            --> "=="

    70.   <rel_op>            --> "!="

    71.   <dec_list>          --> "yellowlight" <declaration> <dec_tail>
                                         "yellowlight"

    72.   <dec_tail>          --> <declaration> <dec_tail>

    73.   <dec_tail>          --> ""

    74.   <declaration>       --> <type> <var_dec_list> ";"

    75.   <var_dec_list>      --> <var_dec> <var_dec_list_tail>

    76.   <var_dec_list_tail> --> "," <var_dec> <var_dec_list_tail>

    77.   <var_dec_list_tail> --> ""

    78.   <var_dec>           --> "ID" <var_dec_tail> [DefineVar]

    79.   <var_dec_tail>      --> "[" "INT_LIT" "]"

    80.   <var_dec_tail>      --> ""

    81.   <select_stmt>       --> "select" <expression> [SelectEval] "from" <case_list>
                                         <otherwise> "endselect"

    82.   <otherwise>         --> "otherwise" [OtherwiseLabel] <stmt_list>

    83.   <otherwise>         --> ""

    84.   <case_list>         --> <case_clause> <case_list_tail>

    85.   <case_list_tail>    --> <case_clause> <case_list_tail>

    86.   <case_list_tail>    --> ""

    87.   <case_clause>       --> "case" <value_list> [CaseLabel] <stmt_list>

    88.   <value_list>        --> "INT_LIT" [IntAppend] <value_list_tail>

    89.   <value_list_tail>   --> "," <value_list> <value_list_tail>

    90.   <value_list_tail>   --> ":"

	____________________________________________________________________________
*/

#ifndef PARSER
#define PARSER

#include "tuncode.h"

class Parser
{
public:

	Parser();
	// Initializes the parser object.

	void SystemGoal();
	// Checks <system goal> syntax and semantics.

private:

	Token savedToken;
	bool tokenAvailable;

/* _____________________________________________________________________________
*/
	void SyntaxError(Token t);
	// Processes a syntax error and aborts compilation.

	Token NextToken();
	// Gets the next token and saves it.

	void Match(Token t);
	// Returns successfully if the next token matches t; otherwise, a syntax
	// error occurs and compilation is aborted.

/* _____________________________________________________________________________
*/
	void ProgramProc();
	// Checks <program> syntax and semantics.

	void StatementList();
	// Checks <statement list> syntax and semantics.

	void Statement();
	// Checks <statement> syntax and semantics.

	void IdList();
	// Checks <id list> syntax and semantics.

	void ExprList();
	// Checks <expr item> syntax and semantics.

	void ExprItem(ExprRec& item);
	//

	void MakeStr(ExprRec& string);
	// Checks strings for syntax

	void Expression(ExprRec& result);
	// Checks <expression> syntax and semantics.

	void Primary(ExprRec& result);
	// Checks <primary> syntax and semantics.

	void AddOp(OpRec& op);
	// Attempts to match an operator and process it.

	void Ident(ExprRec& result);
	// Attempts to match an identifier and process it.
};

#endif
