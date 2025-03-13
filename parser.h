#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include "lexer.h"


typedef enum {
    program,
    mainFunction,
    otherFunctions,
    function,
    input_par,
    output_par,
    parameter_list,
    dataType,
    primitiveDatatype,
    constructedDatatype,
    remaining_list,
    stmts,
    typeDefinitions,
    actualOrRedefined,
    typeDefinition,
    fieldDefinitions,
    fieldDefinition,
    fieldType,
    moreFields,
    declarations,
    declaration,
    global_or_not,
    otherStmts,
    stmt,
    assignmentStmt,
    oneExpansion,
    moreExpansions,
    singleOrRecId,
    option_single_constructed,
    funCallStmt,
    outputParameters,
    inputParameters,
    iterativeStmt,
    conditionalStmt,
    elsePart,
    ioStmt,
    arithmeticExpression,
    expPrime,
    term,
    termPrime,
    factor,
    highPrecedenceOperators,
    lowPrecedenceOperators,
    booleanExpression,
    var,
    logicalOp,
    relationalOp,
    returnStmt,
    optionalReturn,
    idList,
    more_ids,
    definetypestmt,
    A
} NonTerminal;

typedef union{
    NonTerminal n;
    tk t;
} TokenInner;

typedef struct{
    TokenInner tk;
    int isTerminal;
}Token;

typedef struct {
    int ruleNumber;
    NonTerminal lhs;
    Token *rhs; // Array of symbols (terminals or non-terminals)
    int rhsCount;    // Number of symbols on the RHS
} GrammarRule;

// Define a structure for storing FIRST and FOLLOW sets
typedef struct {
    tk *firstSet;    // Array of terminal symbols
    int firstCount;  // Number of terminals in the FIRST set
    tk *followSet;   // Array of terminal symbols
    int followCount; // Number of terminals in the FOLLOW set
} FirstFollowSet;

// Define a structure for storing the parse table cells
typedef struct {
    bool rulePresent;  // Is there a rule in this cell?
    bool error;
    bool syn;
    GrammarRule* rule;  // Only valid if type == PT_ENTRY_RULE
} ParseTableCell;
typedef struct{
    ParseTableCell*** cells;
}ParseTable;

typedef struct ParseTreeNode {
    Token symbol;
    union {
        struct {
            struct ParseTreeNode** children;
            int child_count;
        } non_terminal;          
        struct {
            char* lexeme;
            int line_number;
        } terminal;
    } node;  // Named union member
} ParseTreeNode;

typedef struct {
    bool isTerminal;
    union {
        NonTerminal nt;
        tk terminal;
    } symbol;
    ParseTreeNode* parent;
} StackEntry;

typedef struct {
    StackEntry* items;
    int top;
    int capacity;
} Stack;

// Function prototypes
void storeGrammarRules();
void printGrammarRules();
// void printFirstSets();
FirstFollowSet* ComputeFirstFollowSets();
void createParseTable(FirstFollowSet* F, ParseTable** T);
// void freeParseTable(ParseTable* T);
int containsEpsilon(tk* firstSet, int count);
// void getRuleString(GrammarRule* rule, char* buffer, const char** nonTerminalNames, const char** terminalNames);
void printParseTable(ParseTable* T, const char** nonTerminalNames, const char** terminalNames) ;
ParseTreeNode* create_non_terminal_node(NonTerminal nt);
ParseTreeNode* create_terminal_node(tk token_type, const char* lexeme, int line);
void add_child(ParseTreeNode* parent, ParseTreeNode* child);
void free_parse_tree(ParseTreeNode* root);
void print_parse_tree(const ParseTreeNode* node, int indent_level, const char** non_term_names, const char** term_names);
Stack* createStack(int capacity);
void push(Stack* stack, StackEntry entry);
StackEntry pop(Stack* stack);
int isEmpty(Stack* stack);
StackEntry top(Stack* stack);
void freeStack(Stack* stack);
int isInFirstSet(FirstFollowSet* F, NonTerminal nt, tk token)

#endif
