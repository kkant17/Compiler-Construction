#include "parser.h"
#include <errno.h>
#include <ctype.h>
#include <string.h>
#define NUMGRAMMAR 95
#define MAX_LINE_LENGTH 1024
#define MAX_TERMINALS 56
#define MAX_NONTERMINALS 52
// Array to store grammar rules
GrammarRule *grammarRules;
int grammarRuleCount = 0;

// Array to store FIRST and FOLLOW sets
FirstFollowSet *firstFollowSets;
int nonTerminalCount = 0;

// Array to store the parse table
ParseTableCell *parseTable;

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

        printf("Processing line: %s\n", line);
        /*char *lhsToken = strtok(line, " ");
        char *rhsToken = strtok(NULL, " ");
        printf("LHS: %s, RHS: %s\n", lhsToken, rhsToken);*/
        char *lhsToken = strtok(line, " ");
        char *rhsToken = strtok(NULL, "===");

        if (lhsToken) {
            // Trim leading and trailing spaces from lhsToken
            while (isspace((unsigned char)*lhsToken)) lhsToken++;
            char *end = lhsToken + strlen(lhsToken) - 1;
            while (end > lhsToken && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';
        }

        /*if (rhsToken) {
            // Trim leading and trailing spaces from rhsToken
            while (isspace((unsigned char)*rhsToken)) rhsToken++;
            char *end = rhsToken + strlen(rhsToken) - 1;
            while (end > rhsToken && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';
        }*/

        printf("LHS: %s, RHS: %s\n", lhsToken, rhsToken);

        if (lhsToken && rhsToken) {
            NonTerminal lhs;
            printf("lhsToken: %s\n", lhsToken);
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
                printf("rhs: %s\n", rhs);
                Token *newRhs = realloc(grammarRules[ruleNumber].rhs, sizeof(Token) * (rhsCount + 1));
                if (!newRhs) {
                    perror("Memory allocation failed for RHS tokens");
                    free(grammarRules[ruleNumber].rhs);
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                grammarRules[ruleNumber].rhs = newRhs;

                if (strcmp(rhs, "<otherFunctions>") == 0) {
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
                    grammarRules[ruleNumber].rhs[rhsCount].tk.n = eps;
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


FirstFollowSet* ComputeFirstFollowSets() {
    FILE *file = fopen("grammar.txt", "r");
    if (!file) {
        perror("Error opening grammar.txt");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for firstFollowSets based on the number of non-terminals
    firstFollowSets = malloc(sizeof(FirstFollowSet) * NUMGRAMMAR);

    char line[MAX_LINE_LENGTH];
    FirstFollowSet *currentSet = NULL;
    NonTerminal currentNonTerminal;
    int isFirstSet = 1; // Flag to determine if we're reading FIRST or FOLLOW sets

    while (fgets(line, sizeof(line), file)) {
        // Check if the line indicates a switch between FIRST and FOLLOW sets
        if (strstr(line, "FIRST sets:")) {
            isFirstSet = 1;
            continue;
        } else if (strstr(line, "FOLLOW sets:")) {
            isFirstSet = 0;
            continue;
        }

        // Parse the non-terminal and its set
        char *token = strtok(line, ":");
        if (token) {
            // Map the non-terminal string to the enum value
            if (strcmp(token, "<program>") == 0) {
                currentNonTerminal = program;
            }
            else if (strcmp(token, "<mainFunction>") == 0) {
                currentNonTerminal = mainFunction;
            }
            else if (strcmp(token, "<otherFunctions>") == 0) {
                currentNonTerminal = otherFunctions;
            }
            else if (strcmp(token, "<function>") == 0) {
                currentNonTerminal = function;
            }
            else if (strcmp(token, "<input_par>") == 0) {
                currentNonTerminal = input_par;
            }
            else if (strcmp(token, "<output_par>") == 0) {
                currentNonTerminal = output_par;
            }
            else if (strcmp(token, "<parameter_list>") == 0) {
                currentNonTerminal = parameter_list;
            }
            else if (strcmp(token, "<dataType>") == 0) {
                currentNonTerminal = dataType;
            }
            else if (strcmp(token, "<primitiveDatatype>") == 0) {
                currentNonTerminal = primitiveDatatype;
            }
            else if (strcmp(token, "<constructedDatatype>") == 0) {
                currentNonTerminal = constructedDatatype;
            }
            else if (strcmp(token, "<remaining_list>") == 0) {
                currentNonTerminal = remaining_list;
            }
            else if (strcmp(token, "<stmts>") == 0) {
                currentNonTerminal = stmts;
            }
            else if (strcmp(token, "<typeDefinitions>") == 0) {
                currentNonTerminal = typeDefinitions;
            }
            else if (strcmp(token, "<actualOrRedefined>") == 0) {
                currentNonTerminal = actualOrRedefined;
            }
            else if (strcmp(token, "<typeDefinition>") == 0) {
                currentNonTerminal = typeDefinition;
            }
            else if (strcmp(token, "<fieldDefinitions>") == 0) {
                currentNonTerminal = fieldDefinitions;
            }
            else if (strcmp(token, "<fieldDefinition>") == 0) {
                currentNonTerminal = fieldDefinition;
            }
            else if (strcmp(token, "<fieldType>") == 0) {
                currentNonTerminal = fieldType;
            }
            else if (strcmp(token, "<moreFields>") == 0) {
                currentNonTerminal = moreFields;
            }
            else if (strcmp(token, "<declarations>") == 0) {
                currentNonTerminal = declarations;
            }
            else if (strcmp(token, "<declaration>") == 0) {
                currentNonTerminal = declaration;
            }
            else if (strcmp(token, "<global_or_not>") == 0) {
                currentNonTerminal = global_or_not;
            }
            else if (strcmp(token, "<otherStmts>") == 0) {
                currentNonTerminal = otherStmts;
            }
            else if (strcmp(token, "<stmt>") == 0) {
                currentNonTerminal = stmt;
            }
            else if (strcmp(token, "<assignmentStmt>") == 0) {
                currentNonTerminal = assignmentStmt;
            }
            else if (strcmp(token, "<oneExpansion>") == 0) {
                currentNonTerminal = oneExpansion;
            }
            else if (strcmp(token, "<moreExpansions>") == 0) {
                currentNonTerminal = moreExpansions;
            }
            else if (strcmp(token, "<singleOrRecId>") == 0) {
                currentNonTerminal = singleOrRecId;
            }
            else if (strcmp(token, "<option_single_constructed>") == 0) {
                currentNonTerminal = option_single_constructed;
            }
            else if (strcmp(token, "<funCallStmt>") == 0) {
                currentNonTerminal = funCallStmt;
            }
            else if (strcmp(token, "<outputParameters>") == 0) {
                currentNonTerminal = outputParameters;
            }
            else if (strcmp(token, "<inputParameters>") == 0) {
                currentNonTerminal = inputParameters;
            }
            else if (strcmp(token, "<iterativeStmt>") == 0) {
                currentNonTerminal = iterativeStmt;
            }
            else if (strcmp(token, "<conditionalStmt>") == 0) {
                currentNonTerminal = conditionalStmt;
            }
            else if (strcmp(token, "<elsePart>") == 0) {
                currentNonTerminal = elsePart;
            }
            else if (strcmp(token, "<ioStmt>") == 0) {
                currentNonTerminal = ioStmt;
            }
            else if (strcmp(token, "<arithmeticExpression>") == 0) {
                currentNonTerminal = arithmeticExpression;
            }
            else if (strcmp(token, "<expPrime>") == 0) {
                currentNonTerminal = expPrime;
            }
            else if (strcmp(token, "<term>") == 0) {
                currentNonTerminal = term;
            }
            else if (strcmp(token, "<termPrime>") == 0) {
                currentNonTerminal = termPrime;
            }
            else if (strcmp(token, "<factor>") == 0) {
                currentNonTerminal = factor;
            }
            else if (strcmp(token, "<highPrecedenceOperators>") == 0) {
                currentNonTerminal = highPrecedenceOperators;
            }
            else if (strcmp(token, "<lowPrecedenceOperators>") == 0) {
                currentNonTerminal = lowPrecedenceOperators;
            }
            else if (strcmp(token, "<booleanExpression>") == 0) {
                currentNonTerminal = booleanExpression;
            }
            else if (strcmp(token, "<var>") == 0) {
                currentNonTerminal = var;
            }
            else if (strcmp(token, "<logicalOp>") == 0) {
                currentNonTerminal = logicalOp;
            }
            else if (strcmp(token, "<relationalOp>") == 0) {
                currentNonTerminal = relationalOp;
            }
            else if (strcmp(token, "<returnStmt>") == 0) {
                currentNonTerminal = returnStmt;
            }
            else if (strcmp(token, "<optionalReturn>") == 0) {
                currentNonTerminal = optionalReturn;
            }
            else if (strcmp(token, "<idList>") == 0) {
                currentNonTerminal = idList;
            }
            else if (strcmp(token, "<more_ids>") == 0) {
                currentNonTerminal = more_ids;
            }
            else if (strcmp(token, "<definetypestmt>") == 0) {
                currentNonTerminal = definetypestmt;
            }
            else if (strcmp(token, "<A>") == 0) {
                currentNonTerminal = A;
            }

            // Allocate memory for the current non-terminal's FIRST/FOLLOW set
            currentSet = &firstFollowSets[currentNonTerminal];
            currentSet->firstSet = malloc(sizeof(tk) * MAX_TERMINALS);
            currentSet->followSet = malloc(sizeof(tk) * MAX_TERMINALS);
            currentSet->firstCount = 0;
            currentSet->followCount = 0;

            // Parse the set of terminals
            token = strtok(NULL, "{}, ");
            while (token) {
                tk terminal;
                // Map the terminal string to the enum value
                if (strcmp(token, "TK_ASSIGNOP") == 0) {
                    terminal = TK_ASSIGNOP;
                }
                else if (strcmp(token, "TK_FIELDID") == 0) {
                    terminal = TK_FIELDID;
                }
                else if (strcmp(token, "TK_ID") == 0) {
                    terminal = TK_ID;
                }
                else if (strcmp(token, "TK_NUM") == 0) {
                    terminal = TK_NUM;
                }
                else if (strcmp(token, "TK_RNUM") == 0) {
                    terminal = TK_RNUM;
                }
                else if (strcmp(token, "TK_FUNID") == 0) {
                    terminal = TK_FUNID;
                }
                else if (strcmp(token, "TK_RUID") == 0) {
                    terminal = TK_RUID;
                }
                else if (strcmp(token, "TK_WITH") == 0) {
                    terminal = TK_WITH;
                }
                else if (strcmp(token, "TK_PARAMETERS") == 0) {
                    terminal = TK_PARAMETERS;
                }
                else if (strcmp(token, "TK_END") == 0) {
                    terminal = TK_END;
                }
                else if (strcmp(token, "TK_WHILE") == 0) {
                    terminal = TK_WHILE;
                }
                else if (strcmp(token, "TK_UNION") == 0) {
                    terminal = TK_UNION;
                }
                else if (strcmp(token, "TK_ENDUNION") == 0) {
                    terminal = TK_ENDUNION;
                }
                else if (strcmp(token, "TK_DEFINETYPE") == 0) {
                    terminal = TK_DEFINETYPE;
                }
                else if (strcmp(token, "TK_AS") == 0) {
                    terminal = TK_AS;
                }
                else if (strcmp(token, "TK_TYPE") == 0) {
                    terminal = TK_TYPE;
                }
                else if (strcmp(token, "TK_MAIN") == 0) {
                    terminal = TK_MAIN;
                }
                else if (strcmp(token, "TK_GLOBAL") == 0) {
                    terminal = TK_GLOBAL;
                }
                else if (strcmp(token, "TK_PARAMETER") == 0) {
                    terminal = TK_PARAMETER;
                }
                else if (strcmp(token, "TK_LIST") == 0) {
                    terminal = TK_LIST;
                }
                else if (strcmp(token, "TK_SQL") == 0) {
                    terminal = TK_SQL;
                }
                else if (strcmp(token, "TK_SQR") == 0) {
                    terminal = TK_SQR;
                }
                else if (strcmp(token, "TK_INPUT") == 0) {
                    terminal = TK_INPUT;
                }
                else if (strcmp(token, "TK_OUTPUT") == 0) {
                    terminal = TK_OUTPUT;
                }
                else if (strcmp(token, "TK_INT") == 0) {
                    terminal = TK_INT;
                }
                else if (strcmp(token, "TK_REAL") == 0) {
                    terminal = TK_REAL;
                }
                else if (strcmp(token, "TK_COMMA") == 0) {
                    terminal = TK_COMMA;
                }
                else if (strcmp(token, "TK_SEM") == 0) {
                    terminal = TK_SEM;
                }
                else if (strcmp(token, "TK_COLON") == 0) {
                    terminal = TK_COLON;
                }
                else if (strcmp(token, "TK_DOT") == 0) {
                    terminal = TK_DOT;
                }
                else if (strcmp(token, "TK_ENDWHILE") == 0) {
                    terminal = TK_ENDWHILE;
                }
                else if (strcmp(token, "TK_OP") == 0) {
                    terminal = TK_OP;
                }
                else if (strcmp(token, "TK_CL") == 0) {
                    terminal = TK_CL;
                }
                else if (strcmp(token, "TK_IF") == 0) {
                    terminal = TK_IF;
                }
                else if (strcmp(token, "TK_THEN") == 0) {
                    terminal = TK_THEN;
                }
                else if (strcmp(token, "TK_ENDIF") == 0) {
                    terminal = TK_ENDIF;
                }
                else if (strcmp(token, "TK_READ") == 0) {
                    terminal = TK_READ;
                }
                else if (strcmp(token, "TK_WRITE") == 0) {
                    terminal = TK_WRITE;
                }
                else if (strcmp(token, "TK_RETURN") == 0) {
                    terminal = TK_RETURN;
                }
                else if (strcmp(token, "TK_PLUS") == 0) {
                    terminal = TK_PLUS;
                }
                else if (strcmp(token, "TK_MINUS") == 0) {
                    terminal = TK_MINUS;
                }
                else if (strcmp(token, "TK_MUL") == 0) {
                    terminal = TK_MUL;
                }
                else if (strcmp(token, "TK_DIV") == 0) {
                    terminal = TK_DIV;
                }
                else if (strcmp(token, "TK_CALL") == 0) {
                    terminal = TK_CALL;
                }
                else if (strcmp(token, "TK_RECORD") == 0) {
                    terminal = TK_RECORD;
                }
                else if (strcmp(token, "TK_ENDRECORD") == 0) {
                    terminal = TK_ENDRECORD;
                }
                else if (strcmp(token, "TK_ELSE") == 0) {
                    terminal = TK_ELSE;
                }
                else if (strcmp(token, "TK_AND") == 0) {
                    terminal = TK_AND;
                }
                else if (strcmp(token, "TK_OR") == 0) {
                    terminal = TK_OR;
                }
                else if (strcmp(token, "TK_NOT") == 0) {
                    terminal = TK_NOT;
                }
                else if (strcmp(token, "TK_LT") == 0) {
                    terminal = TK_LT;
                }
                else if (strcmp(token, "TK_LE") == 0) {
                    terminal = TK_LE;
                }
                else if (strcmp(token, "TK_EQ") == 0) {
                    terminal = TK_EQ;
                }
                else if (strcmp(token, "TK_GT") == 0) {
                    terminal = TK_GT;
                }
                else if (strcmp(token, "TK_GE") == 0) {
                    terminal = TK_GE;
                }
                else if (strcmp(token, "TK_NE") == 0) {
                    terminal = TK_NE;
                }
                else if (strcmp(token, "eps") == 0) {
                    continue;
                }

                // Add the terminal to the appropriate set
                if (isFirstSet) {
                    currentSet->firstSet[currentSet->firstCount++] = terminal;
                } else {
                    currentSet->followSet[currentSet->followCount++] = terminal;
                }

                token = strtok(NULL, "{}, ");
            }
        }
    }

    fclose(file);
    return firstFollowSets;
}

void createParseTable(FirstFollowSet* F, ParseTableCell* T){
    T = (ParseTableCell*)malloc(sizeof(ParseTableCell) * MAX_NONTERMINALS * MAX_TERMINALS);
    for (int i = 0; i < MAX_NONTERMINALS; i++) {
        for (int j = i; j < MAX_TERMINALS; j++) {
            T[j].t = j;
            T[j].nt = i;
            T[j]->rule = NULL;
        }
    }
    for(int i = 0; i < grammarRuleCount; i++){
        for(int j = 0; j <grammarRules[i].rhsCount; j++ ){
            for(int m = 0; m < F[grammarRules[i]->rhs[j]]->firstCount; m++){
                tk first = F[grammarRules[i]->rhs[j]]->firstSet[m];
                if(first != TK_EPS){
                    for (int k = 0; k < MAX_NONTERMINALS; k++) {
                        if (T[k].nt ==grammarRules[i].lhs){
                        for (int l = k; l < MAX_TERMINALS; l++) {
                            if(T[l].t == first){
                                T[l]->rule = grammarRules[i]
                                break;
                            }
                        }
                        break;
                        }
                    }
                    break;
                }
            else if(j == grammarRules[i].rhsCount - 1){
                for(int k = 0; k< grammarRules[i].followCount;k++){
               tk follow = F[grammarRules[i].lhs]->followSet[j];
               for (int l = 0; l < MAX_NONTERMINALS; i++) {
                if (T[l].nt ==grammarRules[i].lhs){
                for (int m = i; m < MAX_TERMINALS; j++) {
                    if(T[m].t == follow){
                        T[m]->rule = grammarRules[i]
                        break;
                    }
                }
                break;
                }
            }

            }
        }
            }
        }
    }
}

int main() {

    printf("hello testing");
    //storeGrammarRules(); // Ensure this function is called to populate the grammarRules
    //printGrammarRules(); // Call the function to print the rules
    printf("nt = %d\n", T[23].nt);
    printf("t = %d\n", T[45].t);
    // Free allocated memory here if needed
    return 0;
}
