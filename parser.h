#ifndef PARSER_H
#define PARSER_H
#include "lexerDef.h"

// Array to store grammar rules
extern GrammarRule *grammarRules;
extern int grammarRuleCount;

// Array to store FIRST and FOLLOW sets
extern FirstFollowSet *firstFollowSets;

extern int nonTerminalCount;

extern ParseTable *parseTable;

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
int isInFirstSet(FirstFollowSet* F, NonTerminal nt, tk token);
ParseTreeNode* parseInputSourceCode(char *testcaseFile, ParseTable* T, FirstFollowSet* F);
int checkSynSet(tk token);

#endif
