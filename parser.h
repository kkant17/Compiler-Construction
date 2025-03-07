#ifndef PARSER_H
#define PARSER_H

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
    TokenInner *rhs; // Array of symbols (terminals or non-terminals)
    int rhsCount;    // Number of symbols on the RHS
} GrammarRule;

// Define a structure for storing FIRST and FOLLOW sets
typedef struct {
    tk *firstSet;    // Array of terminal symbols
    int firstCount;  // Number of terminals in the FIRST set
    tk *followSet;   // Array of terminal symbols
    int followCount; // Number of terminals in the FOLLOW set
} FirstFollowSet;

// Function prototypes
void storeGrammarRules();
FirstFollowSet* storeFirstFollowSets();
#endif