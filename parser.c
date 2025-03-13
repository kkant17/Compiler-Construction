#include "parser.h"
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#define NUMGRAMMAR 95
#define MAX_LINE_LENGTH 1024
#define MAX_TERMINALS 58
#define MAX_NON_TERMINALS 53

// Array to store grammar rules
GrammarRule *grammarRules;
int grammarRuleCount;

// Array to store FIRST and FOLLOW sets
FirstFollowSet *firstFollowSets;

int nonTerminalCount = 0;

ParseTable *parseTable = NULL;

// ==================== Non-Terminal Names ====================
const char* nonTerminalNames[MAX_NON_TERMINALS] = {
    "<program>",
    "<mainFunction>",
    "<otherFunctions>",
    "<function>",
    "<input_par>",
    "<output_par>",
    "<parameter_list>",
    "<dataType>",
    "<primitiveDatatype>",
    "<constructedDatatype>",
    "<remaining_list>",
    "<stmts>",
    "<typeDefinitions>",
    "<actualOrRedefined>",
    "<typeDefinition>",
    "<fieldDefinitions>",
    "<fieldDefinition>",
    "<fieldType>",
    "<moreFields>",
    "<declarations>",
    "<declaration>",
    "<global_or_not>",
    "<otherStmts>",
    "<stmt>",
    "<assignmentStmt>",
    "<oneExpansion>",
    "<moreExpansions>",
    "<singleOrRecId>",
    "<option_single_constructed>",
    "<funCallStmt>",
    "<outputParameters>",
    "<inputParameters>",
    "<iterativeStmt>",
    "<conditionalStmt>",
    "<elsePart>",
    "<ioStmt>",
    "<arithmeticExpression>",
    "<expPrime>",
    "<term>",
    "<termPrime>",
    "<factor>",
    "<highPrecedenceOperators>",
    "<lowPrecedenceOperators>",
    "<booleanExpression>",
    "<var>",
    "<logicalOp>",
    "<relationalOp>",
    "<returnStmt>",
    "<optionalReturn>",
    "<idList>",
    "<more_ids>",
    "<definetypestmt>",
    "<A>"
};

// ===================== Terminal Names =====================
const char* terminalNames[MAX_TERMINALS] = {
    "TK_ASSIGNOP",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "TK_EPS",
    "TK_DOLLAR"
};

void storeGrammarRules() {
    FILE *file = fopen("updated_rules.txt", "r");
    if (!file) {
        fprintf(stderr, "Error opening Parsed Grammar.txt: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    grammarRules = malloc(sizeof(GrammarRule) * NUMGRAMMAR);
    if (!grammarRules) {
        perror("Memory allocation failed for grammarRules");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int ruleNumber = 0;

    while (fgets(line, sizeof(line), file)) {

        //printf("Processing line: %s\n", line);

        char *lhsToken = strtok(line, " ");
        char *rhsToken = strtok(NULL, "===");

        if (lhsToken) {
            // Trim leading and trailing spaces from lhsToken
            while (isspace((unsigned char)*lhsToken)) lhsToken++;
            char *end = lhsToken + strlen(lhsToken) - 1;
            while (end > lhsToken && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';
        }

        //printf("LHS: %s, RHS: %s\n", lhsToken, rhsToken);

        if (lhsToken && rhsToken) {
            NonTerminal lhs;
            if (strcmp(lhsToken, "<program>") == 0) {
                lhs = program;
            }
            else if (strcmp(lhsToken, "<mainFunction>") == 0) {
                lhs = mainFunction;
            }
            else if (strcmp(lhsToken, "<otherFunctions>") == 0) {
                lhs = otherFunctions;
            }
            else if (strcmp(lhsToken, "<function>") == 0) { 
                lhs = function;
            }
            else if (strcmp(lhsToken, "<input_par>") == 0) {
                lhs = input_par;
            }   
            else if (strcmp(lhsToken, "<output_par>") == 0) {
                lhs = output_par;
            }
            else if (strcmp(lhsToken, "<parameter_list>") == 0) {
                lhs = parameter_list;
            }
            else if (strcmp(lhsToken, "<dataType>") == 0) {
                lhs = dataType;
            }
            else if (strcmp(lhsToken, "<primitiveDatatype>") == 0) {
                lhs = primitiveDatatype;
            }
            else if (strcmp(lhsToken, "<constructedDatatype>") == 0) {
                lhs = constructedDatatype;
            }
            else if (strcmp(lhsToken, "<remaining_list>") == 0) {
                lhs = remaining_list;
            }
            else if (strcmp(lhsToken, "<stmts>") == 0) {
                lhs = stmts;
            }
            else if (strcmp(lhsToken, "<typeDefinitions>") == 0) {
                lhs = typeDefinitions;
            }
            else if (strcmp(lhsToken, "<actualOrRedefined>") == 0) {
                lhs = actualOrRedefined;
            }
            else if (strcmp(lhsToken, "<typeDefinition>") == 0) {
                lhs = typeDefinition;
            }
            else if (strcmp(lhsToken, "<fieldDefinitions>") == 0) {
                lhs = fieldDefinitions;
            }
            else if (strcmp(lhsToken, "<fieldDefinition>") == 0) {
                lhs = fieldDefinition;
            }
            else if (strcmp(lhsToken, "<fieldType>") == 0) {
                lhs = fieldType;
            }
            else if (strcmp(lhsToken, "<moreFields>") == 0) {
                lhs = moreFields;
            }
            else if (strcmp(lhsToken, "<declarations>") == 0) {
                lhs = declarations;
            }
            else if (strcmp(lhsToken, "<declaration>") == 0) {
                lhs = declaration;
            }
            else if (strcmp(lhsToken, "<global_or_not>") == 0) {
                lhs = global_or_not;
            }
            else if (strcmp(lhsToken, "<otherStmts>") == 0) {
                lhs = otherStmts;
            }
            else if (strcmp(lhsToken, "<stmt>") == 0) {
                lhs = stmt;
            }
            else if (strcmp(lhsToken, "<assignmentStmt>") == 0) {
                lhs = assignmentStmt;
            }
            else if (strcmp(lhsToken, "<oneExpansion>") == 0) {
                lhs = oneExpansion;
            }
            else if (strcmp(lhsToken, "<moreExpansions>") == 0) {
                lhs = moreExpansions;
            }
            else if (strcmp(lhsToken, "<singleOrRecId>") == 0) {
                lhs = singleOrRecId;
            }
            else if (strcmp(lhsToken, "<option_single_constructed>") == 0) {
                lhs = option_single_constructed;
            }
            else if (strcmp(lhsToken, "<funCallStmt>") == 0) {
                lhs = funCallStmt;
            }
            else if (strcmp(lhsToken, "<outputParameters>") == 0) {
                lhs = outputParameters;
            }
            else if (strcmp(lhsToken, "<inputParameters>") == 0) {
                lhs = inputParameters;
            }
            else if (strcmp(lhsToken, "<iterativeStmt>") == 0) {
                lhs = iterativeStmt;
            }
            else if (strcmp(lhsToken, "<conditionalStmt>") == 0) {
                lhs = conditionalStmt;
            }
            else if (strcmp(lhsToken, "<elsePart>") == 0) {
                lhs = elsePart;
            }
            else if (strcmp(lhsToken, "<ioStmt>") == 0) {
                lhs = ioStmt;
            }
            else if (strcmp(lhsToken, "<arithmeticExpression>") == 0) {
                lhs = arithmeticExpression;
            }
            else if (strcmp(lhsToken, "<expPrime>") == 0) {
                lhs = expPrime;
            }
            else if (strcmp(lhsToken, "<term>") == 0) {
                lhs = term;
            }
            else if (strcmp(lhsToken, "<termPrime>") == 0) {
                lhs = termPrime;
            }
            else if (strcmp(lhsToken, "<factor>") == 0) {
                lhs = factor;
            }
            else if (strcmp(lhsToken, "<highPrecedenceOperators>") == 0) {
                lhs = highPrecedenceOperators;
            }
            else if (strcmp(lhsToken, "<lowPrecedenceOperators>") == 0) {
                lhs = lowPrecedenceOperators;
            }
            else if (strcmp(lhsToken, "<booleanExpression>") == 0) {
                lhs = booleanExpression;
            }
            else if (strcmp(lhsToken, "<var>") == 0) {
                lhs = var;
            }
            else if (strcmp(lhsToken, "<logicalOp>") == 0) {
                lhs = logicalOp;
            }
            else if (strcmp(lhsToken, "<relationalOp>") == 0) {
                lhs = relationalOp;
            }
            else if (strcmp(lhsToken, "<returnStmt>") == 0) {
                lhs = returnStmt;
            }
            else if (strcmp(lhsToken, "<optionalReturn>") == 0) {
                lhs = optionalReturn;
            }
            else if (strcmp(lhsToken, "<idList>") == 0) {
                lhs = idList;
            }
            else if (strcmp(lhsToken, "<more_ids>") == 0) {
                lhs = more_ids;
            }
            else if (strcmp(lhsToken, "<definetypestmt>") == 0) {   
                lhs = definetypestmt;
            }
            else if (strcmp(lhsToken, "<A>") == 0) {
                lhs = A;
            }
            grammarRules[ruleNumber].ruleNumber = ruleNumber;
            grammarRules[ruleNumber].lhs = lhs;
            grammarRules[ruleNumber].rhs = NULL; // Initialize to NULL

            char *rhs = strtok(rhsToken, " ");
            int rhsCount = 0;
            while (rhs) {
                size_t len = strlen(rhs);
                while (len > 0 && (rhs[len - 1] == '\n' || rhs[len - 1] == '\r' || rhs[len - 1] == ' ')) {
                    rhs[--len] = '\0';
                }
                Token *newRhs = realloc(grammarRules[ruleNumber].rhs, sizeof(Token) * (rhsCount + 1));
                if (!newRhs) {
                    perror("Memory allocation failed for RHS tokens");
                    free(grammarRules[ruleNumber].rhs);
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                grammarRules[ruleNumber].rhs = newRhs;

                if (strcmp(rhs, "<program>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = program;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<mainFunction>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = mainFunction;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<otherFunctions>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = otherFunctions;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<function>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = function;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<input_par>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = input_par;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<output_par>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = output_par;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<parameter_list>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = parameter_list;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<dataType>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = dataType;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<primitiveDatatype>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = primitiveDatatype;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<constructedDatatype>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = constructedDatatype;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<remaining_list>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = remaining_list;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<stmts>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = stmts;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<typeDefinitions>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = typeDefinitions;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<actualOrRedefined>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = actualOrRedefined;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<typeDefinition>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = typeDefinition;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<fieldDefinitions>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = fieldDefinitions;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<fieldDefinition>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = fieldDefinition;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<fieldType>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = fieldType;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<moreFields>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = moreFields;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<declarations>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = declarations;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<declaration>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = declaration;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<global_or_not>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = global_or_not;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<otherStmts>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = otherStmts;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<stmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = stmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<assignmentStmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = assignmentStmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<oneExpansion>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = oneExpansion;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<moreExpansions>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = moreExpansions;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<singleOrRecId>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = singleOrRecId;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<option_single_constructed>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = option_single_constructed;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<funCallStmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = funCallStmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<outputParameters>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = outputParameters;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<inputParameters>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = inputParameters;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<iterativeStmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = iterativeStmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<conditionalStmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = conditionalStmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<elsePart>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = elsePart;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<ioStmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = ioStmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<arithmeticExpression>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = arithmeticExpression;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<expPrime>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = expPrime;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<term>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = term;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<termPrime>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = termPrime;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<factor>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = factor;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<highPrecedenceOperators>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = highPrecedenceOperators;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<lowPrecedenceOperators>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = lowPrecedenceOperators;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<booleanExpression>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = booleanExpression;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<var>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = var;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<logicalOp>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = logicalOp;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<relationalOp>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = relationalOp;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<returnStmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = returnStmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<optionalReturn>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = optionalReturn;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<idList>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = idList;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<more_ids>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = more_ids;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<definetypestmt>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = definetypestmt;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs, "<A>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = A;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 0;
                }
                else if (strcmp(rhs,"TK_ASSIGNOP") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_ASSIGNOP;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_FIELDID") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_FIELDID;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_ID") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_ID;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_NUM") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_NUM;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_RNUM") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_RNUM;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_FUNID") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_FUNID;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_RUID") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_RUID;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_WITH") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_WITH;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_PARAMETERS") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_PARAMETERS;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_END") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_END;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_WHILE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_WHILE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_UNION") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_UNION;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_ENDUNION") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_ENDUNION;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_DEFINETYPE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_DEFINETYPE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_AS") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_AS;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_TYPE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_TYPE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_MAIN") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_MAIN;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_GLOBAL") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_GLOBAL;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_PARAMETER") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_PARAMETER;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_LIST") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_LIST;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_SQL") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_SQL;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_SQR") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_SQR;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_INPUT") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_INPUT;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_OUTPUT") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_OUTPUT;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_INT") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_INT;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_REAL") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_REAL;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_COMMA") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_COMMA;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_SEM") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_SEM;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_COLON") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_COLON;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_DOT") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_DOT;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_ENDWHILE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_ENDWHILE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_OP") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_OP;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_CL") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_CL;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_IF") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_IF;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_THEN") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_THEN;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_ENDIF") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_ENDIF;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_READ") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_READ;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_WRITE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_WRITE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_RETURN") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_RETURN;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_PLUS") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_PLUS;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_MINUS") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_MINUS;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_MUL") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_MUL;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_DIV") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_DIV;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_CALL") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_CALL;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_RECORD") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_RECORD;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_ENDRECORD") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_ENDRECORD;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_ELSE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_ELSE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_AND") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_AND;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_OR") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_OR;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_NOT") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_NOT;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_LT") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_LT;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_LE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_LE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_EQ") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_EQ;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_GT") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_GT;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_GE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_GE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"TK_NE") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_NE;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                else if (strcmp(rhs,"eps") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].tk.t = TK_EPS;
                    grammarRules[ruleNumber].rhs[rhsCount].isTerminal = 1;
                }
                rhsCount++;
                rhs = strtok(NULL, " ");
            }

            grammarRules[ruleNumber].rhsCount = rhsCount;
            ruleNumber++;
        }
    }

    grammarRuleCount = ruleNumber;
    fclose(file);
}



void printGrammarRules() {
    for (int i = 0; i < grammarRuleCount; i++) {
        printf("Rule %d: ", grammarRules[i].ruleNumber);
        printf("<%d> ===> ", grammarRules[i].lhs); 

        for (int j = 0; j < grammarRules[i].rhsCount; j++) {
            if (grammarRules[i].rhs[j].isTerminal) {
                printf("TK_%d ", grammarRules[i].rhs[j].tk.t); // Print terminal token
            } else {
                printf("<%d> ", grammarRules[i].rhs[j].tk.n); // Print non-terminal
            }
        }
        printf("\n");
    }
}
void printFirstSets() {
    if (!firstFollowSets) {
        printf("First sets not initialized!\n");
        return;
    }

    for (int i = 0; i < MAX_NON_TERMINALS; i++) {
        printf("FIRST(%s): { ", nonTerminalNames[i]);
        for (int j = 0; j < firstFollowSets[i].firstCount; j++) {
            // Print the terminal name if it's within valid range
            if (firstFollowSets[i].firstSet[j] >= 0 && 
                firstFollowSets[i].firstSet[j] < sizeof(terminalNames)/sizeof(terminalNames[0])) {
                printf("%s", terminalNames[firstFollowSets[i].firstSet[j]]);
                if (j < firstFollowSets[i].firstCount - 1) {
                    printf(", ");
                }
            }
        }
        printf(" }\n");
    }
    printf("\nPrinting FOLLOW sets:\n");
    printf("===================\n");
    for (int i = 0; i < MAX_NON_TERMINALS; i++) {
        printf("FOLLOW(%s): { ", nonTerminalNames[i]);
        for (int j = 0; j < firstFollowSets[i].followCount; j++) {
            if (firstFollowSets[i].followSet[j] == TK_DOLLAR) {
                printf("$");
                if (j < firstFollowSets[i].followCount - 1) {
                    printf(", ");
                }
                continue;
            }
            
            if (firstFollowSets[i].followSet[j] >= 0 && 
                firstFollowSets[i].followSet[j] < sizeof(terminalNames)/sizeof(terminalNames[0])) {
                printf("%s", terminalNames[firstFollowSets[i].followSet[j]]);
                if (j < firstFollowSets[i].followCount - 1) {
                    printf(", ");
                }
            }
        }
        printf(" }\n");
    }
}

FirstFollowSet* ComputeFirstFollowSets() {
    firstFollowSets = malloc(sizeof(FirstFollowSet) * NUMGRAMMAR);

    for (int i = 0; i < NUMGRAMMAR; i++) {
        firstFollowSets[i].firstSet = malloc(sizeof(tk) * MAX_TERMINALS);
        firstFollowSets[i].followSet = malloc(sizeof(tk) * MAX_TERMINALS);
        firstFollowSets[i].firstCount = 0;
        firstFollowSets[i].followCount = 0;
    }

    firstFollowSets[program].firstSet[firstFollowSets[program].firstCount++] = TK_FUNID;
    firstFollowSets[program].firstSet[firstFollowSets[program].firstCount++] = TK_MAIN;
    firstFollowSets[mainFunction].firstSet[firstFollowSets[mainFunction].firstCount++] = TK_MAIN;
    firstFollowSets[otherFunctions].firstSet[firstFollowSets[otherFunctions].firstCount++] = TK_FUNID;
    firstFollowSets[otherFunctions].firstSet[firstFollowSets[otherFunctions].firstCount++] = TK_EPS;
    firstFollowSets[function].firstSet[firstFollowSets[function].firstCount++] = TK_FUNID;
    firstFollowSets[input_par].firstSet[firstFollowSets[input_par].firstCount++] = TK_INPUT;
    firstFollowSets[output_par].firstSet[firstFollowSets[output_par].firstCount++] = TK_EPS;
    firstFollowSets[output_par].firstSet[firstFollowSets[output_par].firstCount++] = TK_OUTPUT;
    firstFollowSets[parameter_list].firstSet[firstFollowSets[parameter_list].firstCount++] = TK_INT;
    firstFollowSets[parameter_list].firstSet[firstFollowSets[parameter_list].firstCount++] = TK_REAL;
    firstFollowSets[parameter_list].firstSet[firstFollowSets[parameter_list].firstCount++] = TK_UNION;
    firstFollowSets[parameter_list].firstSet[firstFollowSets[parameter_list].firstCount++] = TK_RUID;
    firstFollowSets[parameter_list].firstSet[firstFollowSets[parameter_list].firstCount++] = TK_RECORD;
    firstFollowSets[dataType].firstSet[firstFollowSets[dataType].firstCount++] = TK_INT;
    firstFollowSets[dataType].firstSet[firstFollowSets[dataType].firstCount++] = TK_REAL;
    firstFollowSets[dataType].firstSet[firstFollowSets[dataType].firstCount++] = TK_UNION;
    firstFollowSets[dataType].firstSet[firstFollowSets[dataType].firstCount++] = TK_RUID;
    firstFollowSets[dataType].firstSet[firstFollowSets[dataType].firstCount++] = TK_RECORD;
    firstFollowSets[primitiveDatatype].firstSet[firstFollowSets[primitiveDatatype].firstCount++] = TK_INT;
    firstFollowSets[primitiveDatatype].firstSet[firstFollowSets[primitiveDatatype].firstCount++] = TK_REAL;
    firstFollowSets[constructedDatatype].firstSet[firstFollowSets[constructedDatatype].firstCount++] = TK_UNION;
    firstFollowSets[constructedDatatype].firstSet[firstFollowSets[constructedDatatype].firstCount++] = TK_RUID;
    firstFollowSets[constructedDatatype].firstSet[firstFollowSets[constructedDatatype].firstCount++] = TK_RECORD;
    firstFollowSets[remaining_list].firstSet[firstFollowSets[remaining_list].firstCount++] = TK_EPS;
    firstFollowSets[remaining_list].firstSet[firstFollowSets[remaining_list].firstCount++] = TK_COMMA;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_WRITE;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_DEFINETYPE;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_SQL;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_WHILE;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_UNION;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_IF;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_RECORD;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_READ;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_CALL;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_TYPE;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_RETURN;
    firstFollowSets[stmts].firstSet[firstFollowSets[stmts].firstCount++] = TK_ID;
    firstFollowSets[typeDefinitions].firstSet[firstFollowSets[typeDefinitions].firstCount++] = TK_UNION;
    firstFollowSets[typeDefinitions].firstSet[firstFollowSets[typeDefinitions].firstCount++] = TK_EPS;
    firstFollowSets[typeDefinitions].firstSet[firstFollowSets[typeDefinitions].firstCount++] = TK_RECORD;
    firstFollowSets[typeDefinitions].firstSet[firstFollowSets[typeDefinitions].firstCount++] = TK_DEFINETYPE;
    firstFollowSets[actualOrRedefined].firstSet[firstFollowSets[actualOrRedefined].firstCount++] = TK_UNION;
    firstFollowSets[actualOrRedefined].firstSet[firstFollowSets[actualOrRedefined].firstCount++] = TK_RECORD;
    firstFollowSets[actualOrRedefined].firstSet[firstFollowSets[actualOrRedefined].firstCount++] = TK_DEFINETYPE;
    firstFollowSets[typeDefinition].firstSet[firstFollowSets[typeDefinition].firstCount++] = TK_UNION;
    firstFollowSets[typeDefinition].firstSet[firstFollowSets[typeDefinition].firstCount++] = TK_RECORD;
    firstFollowSets[fieldDefinitions].firstSet[firstFollowSets[fieldDefinitions].firstCount++] = TK_TYPE;
    firstFollowSets[fieldDefinition].firstSet[firstFollowSets[fieldDefinition].firstCount++] = TK_TYPE;
    firstFollowSets[fieldType].firstSet[firstFollowSets[fieldType].firstCount++] = TK_INT;
    firstFollowSets[fieldType].firstSet[firstFollowSets[fieldType].firstCount++] = TK_REAL;
    firstFollowSets[fieldType].firstSet[firstFollowSets[fieldType].firstCount++] = TK_UNION;
    firstFollowSets[fieldType].firstSet[firstFollowSets[fieldType].firstCount++] = TK_RUID;
    firstFollowSets[fieldType].firstSet[firstFollowSets[fieldType].firstCount++] = TK_RECORD;
    firstFollowSets[moreFields].firstSet[firstFollowSets[moreFields].firstCount++] = TK_TYPE;
    firstFollowSets[moreFields].firstSet[firstFollowSets[moreFields].firstCount++] = TK_EPS;
    firstFollowSets[declarations].firstSet[firstFollowSets[declarations].firstCount++] = TK_TYPE;
    firstFollowSets[declarations].firstSet[firstFollowSets[declarations].firstCount++] = TK_EPS;
    firstFollowSets[declaration].firstSet[firstFollowSets[declaration].firstCount++] = TK_TYPE;
    firstFollowSets[global_or_not].firstSet[firstFollowSets[global_or_not].firstCount++] = TK_COLON;
    firstFollowSets[global_or_not].firstSet[firstFollowSets[global_or_not].firstCount++] = TK_EPS;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_WRITE;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_SQL;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_WHILE;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_EPS;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_IF;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_READ;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_CALL;
    firstFollowSets[otherStmts].firstSet[firstFollowSets[otherStmts].firstCount++] = TK_ID;
    firstFollowSets[stmt].firstSet[firstFollowSets[stmt].firstCount++] = TK_WRITE;
    firstFollowSets[stmt].firstSet[firstFollowSets[stmt].firstCount++] = TK_SQL;
    firstFollowSets[stmt].firstSet[firstFollowSets[stmt].firstCount++] = TK_WHILE;
    firstFollowSets[stmt].firstSet[firstFollowSets[stmt].firstCount++] = TK_IF;
    firstFollowSets[stmt].firstSet[firstFollowSets[stmt].firstCount++] = TK_READ;
    firstFollowSets[stmt].firstSet[firstFollowSets[stmt].firstCount++] = TK_CALL;
    firstFollowSets[stmt].firstSet[firstFollowSets[stmt].firstCount++] = TK_ID;
    firstFollowSets[assignmentStmt].firstSet[firstFollowSets[assignmentStmt].firstCount++] = TK_ID;
    firstFollowSets[oneExpansion].firstSet[firstFollowSets[oneExpansion].firstCount++] = TK_DOT;
    firstFollowSets[moreExpansions].firstSet[firstFollowSets[moreExpansions].firstCount++] = TK_DOT;
    firstFollowSets[moreExpansions].firstSet[firstFollowSets[moreExpansions].firstCount++] = TK_EPS;
    firstFollowSets[singleOrRecId].firstSet[firstFollowSets[singleOrRecId].firstCount++] = TK_ID;
    firstFollowSets[option_single_constructed].firstSet[firstFollowSets[option_single_constructed].firstCount++] = TK_DOT;
    firstFollowSets[option_single_constructed].firstSet[firstFollowSets[option_single_constructed].firstCount++] = TK_EPS;
    firstFollowSets[funCallStmt].firstSet[firstFollowSets[funCallStmt].firstCount++] = TK_SQL;
    firstFollowSets[funCallStmt].firstSet[firstFollowSets[funCallStmt].firstCount++] = TK_CALL;
    firstFollowSets[outputParameters].firstSet[firstFollowSets[outputParameters].firstCount++] = TK_EPS;
    firstFollowSets[outputParameters].firstSet[firstFollowSets[outputParameters].firstCount++] = TK_SQL;
    firstFollowSets[inputParameters].firstSet[firstFollowSets[inputParameters].firstCount++] = TK_SQL;
    firstFollowSets[iterativeStmt].firstSet[firstFollowSets[iterativeStmt].firstCount++] = TK_WHILE;
    firstFollowSets[conditionalStmt].firstSet[firstFollowSets[conditionalStmt].firstCount++] = TK_IF;
    firstFollowSets[elsePart].firstSet[firstFollowSets[elsePart].firstCount++] = TK_ELSE;
    firstFollowSets[elsePart].firstSet[firstFollowSets[elsePart].firstCount++] = TK_ENDIF;
    firstFollowSets[ioStmt].firstSet[firstFollowSets[ioStmt].firstCount++] = TK_WRITE;
    firstFollowSets[ioStmt].firstSet[firstFollowSets[ioStmt].firstCount++] = TK_READ;
    firstFollowSets[arithmeticExpression].firstSet[firstFollowSets[arithmeticExpression].firstCount++] = TK_ID;
    firstFollowSets[arithmeticExpression].firstSet[firstFollowSets[arithmeticExpression].firstCount++] = TK_RNUM;
    firstFollowSets[arithmeticExpression].firstSet[firstFollowSets[arithmeticExpression].firstCount++] = TK_NUM;
    firstFollowSets[arithmeticExpression].firstSet[firstFollowSets[arithmeticExpression].firstCount++] = TK_OP;
    firstFollowSets[expPrime].firstSet[firstFollowSets[expPrime].firstCount++] = TK_PLUS;
    firstFollowSets[expPrime].firstSet[firstFollowSets[expPrime].firstCount++] = TK_EPS;
    firstFollowSets[expPrime].firstSet[firstFollowSets[expPrime].firstCount++] = TK_MINUS;
    firstFollowSets[term].firstSet[firstFollowSets[term].firstCount++] = TK_ID;
    firstFollowSets[term].firstSet[firstFollowSets[term].firstCount++] = TK_RNUM;
    firstFollowSets[term].firstSet[firstFollowSets[term].firstCount++] = TK_NUM;
    firstFollowSets[term].firstSet[firstFollowSets[term].firstCount++] = TK_OP;
    firstFollowSets[termPrime].firstSet[firstFollowSets[termPrime].firstCount++] = TK_DIV;
    firstFollowSets[termPrime].firstSet[firstFollowSets[termPrime].firstCount++] = TK_EPS;
    firstFollowSets[termPrime].firstSet[firstFollowSets[termPrime].firstCount++] = TK_MUL;
    firstFollowSets[factor].firstSet[firstFollowSets[factor].firstCount++] = TK_ID;
    firstFollowSets[factor].firstSet[firstFollowSets[factor].firstCount++] = TK_RNUM;
    firstFollowSets[factor].firstSet[firstFollowSets[factor].firstCount++] = TK_NUM;
    firstFollowSets[factor].firstSet[firstFollowSets[factor].firstCount++] = TK_OP;
    firstFollowSets[highPrecedenceOperators].firstSet[firstFollowSets[highPrecedenceOperators].firstCount++] = TK_DIV;
    firstFollowSets[highPrecedenceOperators].firstSet[firstFollowSets[highPrecedenceOperators].firstCount++] = TK_MUL;
    firstFollowSets[lowPrecedenceOperators].firstSet[firstFollowSets[lowPrecedenceOperators].firstCount++] = TK_PLUS;
    firstFollowSets[lowPrecedenceOperators].firstSet[firstFollowSets[lowPrecedenceOperators].firstCount++] = TK_MINUS;
    firstFollowSets[booleanExpression].firstSet[firstFollowSets[booleanExpression].firstCount++] = TK_RNUM;
    firstFollowSets[booleanExpression].firstSet[firstFollowSets[booleanExpression].firstCount++] = TK_OP;
    firstFollowSets[booleanExpression].firstSet[firstFollowSets[booleanExpression].firstCount++] = TK_NOT;
    firstFollowSets[booleanExpression].firstSet[firstFollowSets[booleanExpression].firstCount++] = TK_NUM;
    firstFollowSets[booleanExpression].firstSet[firstFollowSets[booleanExpression].firstCount++] = TK_ID;
    firstFollowSets[var].firstSet[firstFollowSets[var].firstCount++] = TK_RNUM;
    firstFollowSets[var].firstSet[firstFollowSets[var].firstCount++] = TK_NUM;
    firstFollowSets[var].firstSet[firstFollowSets[var].firstCount++] = TK_ID;
    firstFollowSets[logicalOp].firstSet[firstFollowSets[logicalOp].firstCount++] = TK_AND;
    firstFollowSets[logicalOp].firstSet[firstFollowSets[logicalOp].firstCount++] = TK_OR;
    firstFollowSets[relationalOp].firstSet[firstFollowSets[relationalOp].firstCount++] = TK_GT;
    firstFollowSets[relationalOp].firstSet[firstFollowSets[relationalOp].firstCount++] = TK_GE;
    firstFollowSets[relationalOp].firstSet[firstFollowSets[relationalOp].firstCount++] = TK_NE;
    firstFollowSets[relationalOp].firstSet[firstFollowSets[relationalOp].firstCount++] = TK_EQ;
    firstFollowSets[relationalOp].firstSet[firstFollowSets[relationalOp].firstCount++] = TK_LT;
    firstFollowSets[relationalOp].firstSet[firstFollowSets[relationalOp].firstCount++] = TK_LE;
    firstFollowSets[returnStmt].firstSet[firstFollowSets[returnStmt].firstCount++] = TK_RETURN;
    firstFollowSets[optionalReturn].firstSet[firstFollowSets[optionalReturn].firstCount++] = TK_EPS;
    firstFollowSets[optionalReturn].firstSet[firstFollowSets[optionalReturn].firstCount++] = TK_SQL;
    firstFollowSets[idList].firstSet[firstFollowSets[idList].firstCount++] = TK_ID;
    firstFollowSets[more_ids].firstSet[firstFollowSets[more_ids].firstCount++] = TK_EPS;
    firstFollowSets[more_ids].firstSet[firstFollowSets[more_ids].firstCount++] = TK_COMMA;
    firstFollowSets[definetypestmt].firstSet[firstFollowSets[definetypestmt].firstCount++] = TK_DEFINETYPE;
    firstFollowSets[A].firstSet[firstFollowSets[A].firstCount++] = TK_UNION;
    firstFollowSets[A].firstSet[firstFollowSets[A].firstCount++] = TK_RECORD;


    // Hardcode the FOLLOW sets
    firstFollowSets[program].followSet[firstFollowSets[program].followCount++] = TK_DOLLAR;
    firstFollowSets[mainFunction].followSet[firstFollowSets[mainFunction].followCount++] = TK_DOLLAR;
    firstFollowSets[otherFunctions].followSet[firstFollowSets[otherFunctions].followCount++] = TK_MAIN;
    firstFollowSets[function].followSet[firstFollowSets[function].followCount++] = TK_FUNID;
    firstFollowSets[function].followSet[firstFollowSets[function].followCount++] = TK_MAIN;
    firstFollowSets[input_par].followSet[firstFollowSets[input_par].followCount++] = TK_SEM;
    firstFollowSets[input_par].followSet[firstFollowSets[input_par].followCount++] = TK_OUTPUT;
    firstFollowSets[output_par].followSet[firstFollowSets[output_par].followCount++] = TK_SEM;
    firstFollowSets[parameter_list].followSet[firstFollowSets[parameter_list].followCount++] = TK_SQR;
    firstFollowSets[dataType].followSet[firstFollowSets[dataType].followCount++] = TK_COLON;
    firstFollowSets[dataType].followSet[firstFollowSets[dataType].followCount++] = TK_ID;
    firstFollowSets[primitiveDatatype].followSet[firstFollowSets[primitiveDatatype].followCount++] = TK_COLON;
    firstFollowSets[primitiveDatatype].followSet[firstFollowSets[primitiveDatatype].followCount++] = TK_ID;
    firstFollowSets[constructedDatatype].followSet[firstFollowSets[constructedDatatype].followCount++] = TK_COLON;
    firstFollowSets[constructedDatatype].followSet[firstFollowSets[constructedDatatype].followCount++] = TK_ID;
    firstFollowSets[remaining_list].followSet[firstFollowSets[remaining_list].followCount++] = TK_SQR;
    firstFollowSets[stmts].followSet[firstFollowSets[stmts].followCount++] = TK_END;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_WRITE;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_SQL;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_WHILE;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_IF;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_READ;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_CALL;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_TYPE;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_RETURN;
    firstFollowSets[typeDefinitions].followSet[firstFollowSets[typeDefinitions].followCount++] = TK_ID;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_WRITE;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_DEFINETYPE;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_SQL;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_WHILE;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_UNION;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_IF;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_RECORD;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_READ;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_CALL;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_TYPE;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_RETURN;
    firstFollowSets[actualOrRedefined].followSet[firstFollowSets[actualOrRedefined].followCount++] = TK_ID;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_WRITE;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_DEFINETYPE;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_SQL;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_WHILE;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_UNION;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_IF;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_RECORD;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_READ;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_CALL;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_TYPE;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_RETURN;
    firstFollowSets[typeDefinition].followSet[firstFollowSets[typeDefinition].followCount++] = TK_ID;
    firstFollowSets[fieldDefinitions].followSet[firstFollowSets[fieldDefinitions].followCount++] = TK_ENDUNION;
    firstFollowSets[fieldDefinitions].followSet[firstFollowSets[fieldDefinitions].followCount++] = TK_ENDRECORD;
    firstFollowSets[fieldDefinition].followSet[firstFollowSets[fieldDefinition].followCount++] = TK_TYPE;
    firstFollowSets[fieldDefinition].followSet[firstFollowSets[fieldDefinition].followCount++] = TK_ENDUNION;
    firstFollowSets[fieldDefinition].followSet[firstFollowSets[fieldDefinition].followCount++] = TK_ENDRECORD;
    firstFollowSets[fieldType].followSet[firstFollowSets[fieldType].followCount++] = TK_COLON;
    firstFollowSets[moreFields].followSet[firstFollowSets[moreFields].followCount++] = TK_ENDUNION;
    firstFollowSets[moreFields].followSet[firstFollowSets[moreFields].followCount++] = TK_ENDRECORD;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_WRITE;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_SQL;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_WHILE;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_IF;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_READ;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_CALL;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_RETURN;
    firstFollowSets[declarations].followSet[firstFollowSets[declarations].followCount++] = TK_ID;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_WRITE;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_SQL;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_WHILE;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_IF;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_READ;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_CALL;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_TYPE;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_RETURN;
    firstFollowSets[declaration].followSet[firstFollowSets[declaration].followCount++] = TK_ID;
    firstFollowSets[global_or_not].followSet[firstFollowSets[global_or_not].followCount++] = TK_SEM;
    firstFollowSets[otherStmts].followSet[firstFollowSets[otherStmts].followCount++] = TK_ENDWHILE;
    firstFollowSets[otherStmts].followSet[firstFollowSets[otherStmts].followCount++] = TK_RETURN;
    firstFollowSets[otherStmts].followSet[firstFollowSets[otherStmts].followCount++] = TK_ELSE;
    firstFollowSets[otherStmts].followSet[firstFollowSets[otherStmts].followCount++] = TK_ENDIF;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_WRITE;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_SQL;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_WHILE;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_ELSE;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_ENDWHILE;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_IF;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_READ;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_CALL;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_RETURN;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_ID;
    firstFollowSets[stmt].followSet[firstFollowSets[stmt].followCount++] = TK_ENDIF;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_WRITE;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_SQL;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_WHILE;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_ELSE;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_ENDWHILE;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_IF;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_READ;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_CALL;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_RETURN;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_ID;
    firstFollowSets[assignmentStmt].followSet[firstFollowSets[assignmentStmt].followCount++] = TK_ENDIF;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_GT;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_MUL;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_GE;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_SEM;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_NE;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_DIV;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_EQ;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_DOT;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_LT;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_ASSIGNOP;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_CL;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_PLUS;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_LE;
    firstFollowSets[oneExpansion].followSet[firstFollowSets[oneExpansion].followCount++] = TK_MINUS;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_GT;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_MUL;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_GE;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_SEM;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_NE;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_DIV;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_EQ;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_LT;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_ASSIGNOP;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_CL;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_PLUS;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_LE;
    firstFollowSets[moreExpansions].followSet[firstFollowSets[moreExpansions].followCount++] = TK_MINUS;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_GT;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_MUL;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_GE;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_SEM;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_NE;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_DIV;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_EQ;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_LT;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_ASSIGNOP;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_CL;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_PLUS;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_LE;
    firstFollowSets[singleOrRecId].followSet[firstFollowSets[singleOrRecId].followCount++] = TK_MINUS;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_GT;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_MUL;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_GE;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_SEM;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_NE;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_DIV;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_EQ;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_LT;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_ASSIGNOP;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_CL;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_PLUS;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_LE;
    firstFollowSets[option_single_constructed].followSet[firstFollowSets[option_single_constructed].followCount++] = TK_MINUS;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_WRITE;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_SQL;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_WHILE;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_ELSE;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_ENDWHILE;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_IF;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_READ;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_CALL;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_RETURN;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_ID;
    firstFollowSets[funCallStmt].followSet[firstFollowSets[funCallStmt].followCount++] = TK_ENDIF;
    firstFollowSets[outputParameters].followSet[firstFollowSets[outputParameters].followCount++] = TK_CALL;
    firstFollowSets[inputParameters].followSet[firstFollowSets[inputParameters].followCount++] = TK_SEM;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_WRITE;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_SQL;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_WHILE;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_ELSE;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_ENDWHILE;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_IF;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_READ;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_CALL;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_RETURN;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_ID;
    firstFollowSets[iterativeStmt].followSet[firstFollowSets[iterativeStmt].followCount++] = TK_ENDIF;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_WRITE;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_SQL;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_WHILE;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_ELSE;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_ENDWHILE;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_IF;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_READ;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_CALL;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_RETURN;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_ID;
    firstFollowSets[conditionalStmt].followSet[firstFollowSets[conditionalStmt].followCount++] = TK_ENDIF;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_WRITE;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_SQL;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_WHILE;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_ELSE;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_ENDWHILE;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_IF;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_READ;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_CALL;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_RETURN;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_ID;
    firstFollowSets[elsePart].followSet[firstFollowSets[elsePart].followCount++] = TK_ENDIF;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_WRITE;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_SQL;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_WHILE;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_ELSE;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_ENDWHILE;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_IF;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_READ;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_CALL;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_RETURN;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_ID;
    firstFollowSets[ioStmt].followSet[firstFollowSets[ioStmt].followCount++] = TK_ENDIF;
    firstFollowSets[arithmeticExpression].followSet[firstFollowSets[arithmeticExpression].followCount++] = TK_CL;
    firstFollowSets[arithmeticExpression].followSet[firstFollowSets[arithmeticExpression].followCount++] = TK_SEM;
    firstFollowSets[expPrime].followSet[firstFollowSets[expPrime].followCount++] = TK_CL;
    firstFollowSets[expPrime].followSet[firstFollowSets[expPrime].followCount++] = TK_SEM;
    firstFollowSets[term].followSet[firstFollowSets[term].followCount++] = TK_CL;
    firstFollowSets[term].followSet[firstFollowSets[term].followCount++] = TK_PLUS;
    firstFollowSets[term].followSet[firstFollowSets[term].followCount++] = TK_SEM;
    firstFollowSets[term].followSet[firstFollowSets[term].followCount++] = TK_MINUS;
    firstFollowSets[termPrime].followSet[firstFollowSets[termPrime].followCount++] = TK_CL;
    firstFollowSets[termPrime].followSet[firstFollowSets[termPrime].followCount++] = TK_PLUS;
    firstFollowSets[termPrime].followSet[firstFollowSets[termPrime].followCount++] = TK_SEM;
    firstFollowSets[termPrime].followSet[firstFollowSets[termPrime].followCount++] = TK_MINUS;
    firstFollowSets[factor].followSet[firstFollowSets[factor].followCount++] = TK_MUL;
    firstFollowSets[factor].followSet[firstFollowSets[factor].followCount++] = TK_SEM;
    firstFollowSets[factor].followSet[firstFollowSets[factor].followCount++] = TK_DIV;
    firstFollowSets[factor].followSet[firstFollowSets[factor].followCount++] = TK_CL;
    firstFollowSets[factor].followSet[firstFollowSets[factor].followCount++] = TK_PLUS;
    firstFollowSets[factor].followSet[firstFollowSets[factor].followCount++] = TK_MINUS;
    firstFollowSets[highPrecedenceOperators].followSet[firstFollowSets[highPrecedenceOperators].followCount++] = TK_RNUM;
    firstFollowSets[highPrecedenceOperators].followSet[firstFollowSets[highPrecedenceOperators].followCount++] = TK_OP;
    firstFollowSets[highPrecedenceOperators].followSet[firstFollowSets[highPrecedenceOperators].followCount++] = TK_ID;
    firstFollowSets[highPrecedenceOperators].followSet[firstFollowSets[highPrecedenceOperators].followCount++] = TK_NUM;
    firstFollowSets[lowPrecedenceOperators].followSet[firstFollowSets[lowPrecedenceOperators].followCount++] = TK_RNUM;
    firstFollowSets[lowPrecedenceOperators].followSet[firstFollowSets[lowPrecedenceOperators].followCount++] = TK_OP;
    firstFollowSets[lowPrecedenceOperators].followSet[firstFollowSets[lowPrecedenceOperators].followCount++] = TK_ID;
    firstFollowSets[lowPrecedenceOperators].followSet[firstFollowSets[lowPrecedenceOperators].followCount++] = TK_NUM;
    firstFollowSets[booleanExpression].followSet[firstFollowSets[booleanExpression].followCount++] = TK_CL;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_GT;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_MUL;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_GE;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_SEM;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_NE;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_DIV;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_EQ;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_LT;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_CL;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_PLUS;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_LE;
    firstFollowSets[var].followSet[firstFollowSets[var].followCount++] = TK_MINUS;
    firstFollowSets[logicalOp].followSet[firstFollowSets[logicalOp].followCount++] = TK_OP;
    firstFollowSets[relationalOp].followSet[firstFollowSets[relationalOp].followCount++] = TK_RNUM;
    firstFollowSets[relationalOp].followSet[firstFollowSets[relationalOp].followCount++] = TK_NUM;
    firstFollowSets[relationalOp].followSet[firstFollowSets[relationalOp].followCount++] = TK_ID;
    firstFollowSets[returnStmt].followSet[firstFollowSets[returnStmt].followCount++] = TK_END;
    firstFollowSets[optionalReturn].followSet[firstFollowSets[optionalReturn].followCount++] = TK_SEM;
    firstFollowSets[idList].followSet[firstFollowSets[idList].followCount++] = TK_SQR;
    firstFollowSets[more_ids].followSet[firstFollowSets[more_ids].followCount++] = TK_SQR;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_WRITE;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_DEFINETYPE;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_SQL;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_WHILE;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_UNION;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_IF;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_RECORD;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_READ;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_CALL;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_TYPE;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_RETURN;
    firstFollowSets[definetypestmt].followSet[firstFollowSets[definetypestmt].followCount++] = TK_ID;
    firstFollowSets[A].followSet[firstFollowSets[A].followCount++] = TK_RUID;
    return firstFollowSets;
}


//void createParseTable(FirstFollowSet* F, ParseTable* T){
    // T = (ParseTable*)malloc(sizeof(ParseTable));
    // for (int i = 0; i < MAX_NON_TERMINALS; i++) {
    //     T[i] = (GrammarRule*)malloc(sizeof(GrammarRule)*MAX_NON_TERMINALS)
    //     for (int j = i; j < MAX_TERMINALS; j++) {
    //         T[i][j]->rule = NULL;
    //     }
    // }
    // for(int i = 0; i < grammarRuleCount; i++){
    //     int eps_in_first = false;
    //     for(int j = 0; j <grammarRules[i].rhsCount; j++ ){
    //         for(int m = 0; m < F[grammarRules[i]->rhs[j]]->firstCount; m++){
    //             tk first = F[grammarRules[i]->rhs[j]]->firstSet[m];
    //             if(first != TK_EPS;){
    //                 T[i][j]->rule = grammarRules[i];
    //             }
    //             else eps_in_first = true;
    //         }
    //         if(eps_in_first){
    //             for(int k = 0; k< grammarRules[i].followCount;k++){
    //             tk follow = F[grammarRules[i].lhs]->followSet[j];
    //             T[i][j]->rule = grammarRules[i];
    //             }
    //         }
    //     }
    // }
    // parser.c
void createParseTable(FirstFollowSet* F, ParseTable** T) {
    // Allocate the ParseTable struct
    *T = malloc(sizeof(ParseTable));

    // Allocate 2D array of pointers
    (*T)->cells = malloc(MAX_NON_TERMINALS * sizeof(ParseTableCell**));

    // Initialize each row
    for (int i = 0; i < MAX_NON_TERMINALS; i++) {
        (*T)->cells[i] = calloc(MAX_TERMINALS, sizeof(ParseTableCell*));
        for (int t = 0; t < MAX_TERMINALS; t++) {
            (*T)->cells[i][t] = malloc(sizeof(ParseTableCell));
            (*T)->cells[i][t]->error = true;
            (*T)->cells[i][t]->syn = false;
            (*T)->cells[i][t]->rulePresent = false;
            (*T)->cells[i][t]->rule = NULL;
    }
}

    // Populate the table
    for (int i = 0; i < grammarRuleCount; i++) {
        GrammarRule* rule = &grammarRules[i];
        NonTerminal A = rule->lhs;
        // printf("Rule: %s\n", nonTerminalNames[A]);
        int can_derive_epsilon = 0;

        for (int j = 0; j < rule->rhsCount; j++) {
            Token symbol = rule->rhs[j];
            
            if (symbol.isTerminal) {
                if (symbol.tk.t != TK_EPS) {
                    ParseTableCell* cell = (*T)->cells[A][symbol.tk.t];
                    cell->rule = rule;
                    cell->rulePresent = true;
                    cell->error = false;
                }
                //printf("eps encountered\n");
                else{
                    can_derive_epsilon = 1;
                }
                break;
            } else {
                NonTerminal B = symbol.tk.n;
                // printf("  Checking FIRST(%s)\n", nonTerminalNames[B]);
                can_derive_epsilon = 1;
                
                for (int k = 0; k < F[B].firstCount; k++) {
                    tk term = F[B].firstSet[k];
                    if (term != TK_EPS) {
                        (*T)->cells[A][term]->rule = rule;  // Assign pointer
                        (*T)->cells[A][term]->error = false;
                        (*T)->cells[A][term]->rulePresent = true;
                    } else {
                        can_derive_epsilon = 1;
                    }
                }
                
                if (!containsEpsilon(F[B].firstSet, F[B].firstCount)) {
                    can_derive_epsilon = 0;
                    break;
                }
            }
        }

        if (can_derive_epsilon) {
            printf("Can derive epsilon: %s\n", nonTerminalNames[A]);
            for (int j = 0; j < F[A].followCount; j++) {
                tk term = F[A].followSet[j];
                (*T)->cells[A][term]->rule = rule;  // Assign pointer
            }
        }
    }
    for (int nt = 0; nt < MAX_NON_TERMINALS; nt++) {
        for (int j = 0; j < F[nt].followCount; j++) {
            tk term = F[nt].followSet[j];
            if ((*T)->cells[nt][term]->error) {
                (*T)->cells[nt][term]->syn = true;
                }
            }
        }

}

int containsEpsilon(tk* firstSet, int count) {
    for (int i = 0; i < count; i++) {
        if (firstSet[i] == TK_EPS) return 1;
    }
    return 0;
}

void freeParseTable(ParseTable* T) {
    if (!T) return;
    
    for (int i = 0; i < MAX_NON_TERMINALS; i++) {
        for (int j = 0; j < MAX_TERMINALS; j++)
        free(T->cells[i][j]);
        free(T->cells[i]);
    }
    free(T->cells);
    free(T);
}

// Helper function to convert a grammar rule to a string
void getRuleString(GrammarRule* rule, char* buffer, const char** nonTerminalNames, const char** terminalNames) {
    // Start with LHS
    sprintf(buffer, "%s → ", nonTerminalNames[rule->lhs]);
    
    // Handle RHS symbols
    for (int i = 0; i < rule->rhsCount; i++) {
        Token symbol = rule->rhs[i];
        if (symbol.isTerminal) {
            if (symbol.tk.t == TK_EPS) {
                strcat(buffer, "ε");
            } else {
                strcat(buffer, terminalNames[symbol.tk.t]);
            }
        } else {
            strcat(buffer, nonTerminalNames[symbol.tk.n]);
        }
        
        // Add space between symbols
        if (i != rule->rhsCount - 1) {
            strcat(buffer, " ");
        }
    }
}

void printParseTable(ParseTable* T, const char** nonTerminalNames, const char** terminalNames) {
    printf("\nParse Table:\n");
    printf("====================================================================\n");
    
    for (int nt = 0; nt < MAX_NON_TERMINALS; nt++) {
        printf("%s:\n", nonTerminalNames[nt]);
        
        for (int t = 0; t < MAX_TERMINALS; t++) {
            if (T->cells[nt][t]->rulePresent) {
                char ruleStr[256];
                GrammarRule* rule = T->cells[nt][t]->rule;
                getRuleString(rule, ruleStr, nonTerminalNames, terminalNames);
                printf("  On %-15s: %s\n", terminalNames[t], ruleStr);
            }
            else{
                if(T->cells[nt][t]->syn){
                    printf("  On %-15s: Syn\n", terminalNames[t]);
                }
                else{
                    printf("  On %-15s: Error\n", terminalNames[t]);
                }
            }
        }
        printf("------------------------------------------------------------\n");
    }
    printf("====================================================================\n");
}

// Create non-terminal node
ParseTreeNode* create_non_terminal_node(NonTerminal nt) {
    ParseTreeNode* node = malloc(sizeof(ParseTreeNode));
    node->symbol.isTerminal = 0;
    node->symbol.tk.n = nt;
    node->node.non_terminal.children = NULL;
    node->node.non_terminal.child_count = 0;
    return node;
}

// Create terminal node
ParseTreeNode* create_terminal_node(tk token_type, const char* lexeme, int line) {
    ParseTreeNode* node = malloc(sizeof(ParseTreeNode));
    node->symbol.isTerminal = 1;
    node->symbol.tk.t = token_type;
    node->node.terminal.lexeme = lexeme ? strdup(lexeme) : NULL;
    node->node.terminal.line_number = line;
    return node;
}

// Add child to non-terminal node
void add_child(ParseTreeNode* parent, ParseTreeNode* child) {
    if (parent->symbol.isTerminal) {
        fprintf(stderr, "Error: Cannot add children to terminal node\n");
        return;
    }
    
    parent->node.non_terminal.child_count++;
    parent->node.non_terminal.children = realloc(
        parent->node.non_terminal.children,
        parent->node.non_terminal.child_count * sizeof(ParseTreeNode*)
    );
    parent->node.non_terminal.children[parent->node.non_terminal.child_count - 1] = child;
}

// Free parse tree
void free_parse_tree(ParseTreeNode* root) {
    if (!root) return;
    
    if (!root->symbol.isTerminal) {
        for (int i = 0; i < root->node.non_terminal.child_count; i++) {
            free_parse_tree(root->node.non_terminal.children[i]);
        }
        free(root->node.non_terminal.children);
    } else {
        free(root->node.terminal.lexeme);
    }
    free(root);
}

// Print parse tree
void print_parse_tree(const ParseTreeNode* node, int indent_level,
                     const char** non_term_names, const char** term_names) {
    for (int i = 0; i < indent_level; i++) printf("  ");
    
    if (node->symbol.isTerminal) {
        printf("%s", term_names[node->symbol.tk.t]);
        if (node->node.terminal.lexeme) printf(" '%s'", node->node.terminal.lexeme);
        printf(" (line %d)\n", node->node.terminal.line_number);
    } else {
        printf("%s\n", non_term_names[node->symbol.tk.n]);
        for (int i = 0; i < node->node.non_terminal.child_count; i++) {
            print_parse_tree(node->node.non_terminal.children[i], indent_level + 1, 
                            non_term_names, term_names);
        }
    }
}

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->items = (StackEntry*)malloc(capacity * sizeof(StackEntry));
    return stack;
}

void push(Stack* stack, StackEntry entry) {
    if (stack->top == stack->capacity - 1) {
        stack->capacity *= 2;
        stack->items = realloc(stack->items, stack->capacity * sizeof(StackEntry));
    }
    stack->items[++stack->top] = entry;
}

StackEntry pop(Stack* stack) {
    return stack->items[stack->top--];
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void freeStack(Stack* stack) {
    free(stack->items);
    free(stack);
}

// Helper function to check if a token is in the FIRST set of a non-terminal
int isInFirstSet(FirstFollowSet* F, NonTerminal nt, tk token) {
    for (int i = 0; i < F[nt].firstCount; i++) {
        if (F[nt].firstSet[i] == token) {
            return 1;
        }
    }
    return 0;
}

ParseTreeNode* parseInputSourceCode(char *testcaseFile, ParseTable* T, FirstFollowSet* F) {
    // Preprocess input file to remove comments
    char cleanFile[] = "clean_input.tmp";
    removeComments(testcaseFile, cleanFile);
    
    // Open cleaned file for lexical analysis
    FILE* fp = fopen(cleanFile, "r");
    if (!fp) {
        printf("Error opening cleaned file\n");
        return NULL;
    }

    // Initialize lexical analysis components
    tokenInfo currentToken = getNextToken(&buffer, fp, keywordTable);
    ParseTreeNode* root = NULL;
    Stack* stack = createStack(100);

    // Push start symbol
    StackEntry startEntry = {
        .isTerminal = false,
        .symbol.nt = program,
        .parent = NULL
    };
    push(stack, startEntry);

    int errorFlag = 0;

    while (!isEmpty(stack)) {
        StackEntry entry = pop(stack);

        if (!entry.isTerminal) {
            NonTerminal nt = entry.symbol.nt;
            tk tokenType = currentToken.tkid;

            // Check for lexical errors first
            if (currentToken.err) {
                printf("Lexical error at line %d: Invalid token '%s'\n", 
                      currentToken.lno, currentToken.strlex);
                errorFlag = 1;
                currentToken = getNextToken(&buffer, fp, keywordTable);
                push(stack, entry);  // Retry current non-terminal
                continue;
            }

            ParseTableCell* cell = T->cells[nt][tokenType];

            if (cell->rulePresent) {
                /* ... existing rule handling code ... */
            }
            else if (cell->syn) {
                /* ... existing SYN error handling ... */
            }
            else {
                /* ... existing normal error handling ... */
            }
        } else {
            // Handle terminal matching
            tk expected = entry.symbol.terminal;
            
            if (expected == currentToken.tkid) {
                // Format lexeme value
                char lexVal[31];
                if (currentToken.tkid == TK_NUM) {
                    snprintf(lexVal, 30, "%d", currentToken.val.ival);
                } else if (currentToken.tkid == TK_RNUM) {
                    snprintf(lexVal, 30, "%.2f", currentToken.val.rval);
                } else {
                    strncpy(lexVal, currentToken.strlex, 30);
                }
                
                // Create terminal node
                ParseTreeNode* termNode = create_terminal_node(
                    currentToken.tkid, lexVal, currentToken.lno
                );
                add_child(entry.parent, termNode);
                currentToken = getNextToken(&buffer, fp, keywordTable);
            } else {
                printf("Syntax Error at line %d: Expected %s, found %s\n",
                      currentToken.lno, 
                      terminalNames[expected],
                      terminalNames[currentToken.tkid]);
                errorFlag = 1;
            }
        }
    }
    // Final checks
    if (currentToken.tkid != TK_EOF && !errorFlag) {
        printf("Unexpected token '%s' at line %d after valid parse\n",
               terminalNames[currentToken.tkid], currentToken.lno);
        errorFlag = 1;
    }

    freeStack(stack);

    if (!errorFlag) {
        printf("Input source code is syntactically correct.\n");
    } else {
        free_parse_tree(root);
        root = NULL;
    }

    return root;
}

int main() {

    printf("hello testing");
    firstFollowSets = ComputeFirstFollowSets();
    printf("\nPrinting FIRST sets:\n");
    printf("===================\n");
   // printFirstSets();
    storeGrammarRules(); 
    //printGrammarRules();
   createParseTable(firstFollowSets, &parseTable);
   printParseTable(parseTable, nonTerminalNames, terminalNames); 
    return 0;
}
