#include "parser.h"
#include <errno.h>
#define NUMGRAMMAR 54
#define MAX_LINE_LENGTH 1024
#define MAX_TERMINALS 56
// Array to store grammar rules
GrammarRule *grammarRules;
int grammarRuleCount = 0;

// Array to store FIRST and FOLLOW sets
FirstFollowSet *firstFollowSets;
int nonTerminalCount = 0;
void storeGrammarRules() {
    FILE *file = fopen("ParsedGrammar.txt", "r");
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
        if (strstr(line, "Parsed Grammar:") || strstr(line, "FIRST sets:") || strstr(line, "FOLLOW sets:")) {
            continue;
        }

        char *lhsToken = strtok(line, " ");
        char *rhsToken = strtok(NULL, " ");

        if (lhsToken && rhsToken) {
            NonTerminal lhs;
            if (strcmp(lhsToken, "<program>") == 0) {
                lhs = program;
            }
            // Add more mappings for other non-terminals as needed

            grammarRules[ruleNumber].ruleNumber = ruleNumber;
            grammarRules[ruleNumber].lhs = lhs;
            grammarRules[ruleNumber].rhs = NULL; // Initialize to NULL

            char *rhs = strtok(rhsToken, " ");
            int rhsCount = 0;
            while (rhs) {
                TokenInner *newRhs = realloc(grammarRules[ruleNumber].rhs, sizeof(TokenInner) * (rhsCount + 1));
                if (!newRhs) {
                    perror("Memory allocation failed for RHS tokens");
                    free(grammarRules[ruleNumber].rhs); // Free previously allocated memory
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                grammarRules[ruleNumber].rhs = newRhs;

                if (strcmp(rhs, "<otherFunctions>") == 0) {
                    grammarRules[ruleNumber].rhs[rhsCount].n = otherFunctions;
                }
                // Add more mappings for other tokens as needed

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
        printf("<%d> ===> ", grammarRules[i].lhs); // Replace with actual non-terminal names if needed

        for (int j = 0; j < grammarRules[i].rhsCount; j++) {
            printf("<%d> ", grammarRules[i].rhs[j].n); // Replace with actual token names if needed
        }
        printf("\n");
    }
}
FirstFollowSet* storeFirstFollowSets() {
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

void printFirstFollowSets(FirstFollowSet* sets, int nonTerminalCount) {
    for (int i = 0; i < nonTerminalCount; i++) {
        printf("Non-terminal %d:\n", i);
        printf("  FIRST set: ");
        for (int j = 0; j < sets[i].firstCount; j++) {
            printf("%d ", sets[i].firstSet[j]); // Replace with actual terminal names if needed
        }
        printf("\n  FOLLOW set: ");
        for (int j = 0; j < sets[i].followCount; j++) {
            printf("%d ", sets[i].followSet[j]); // Replace with actual terminal names if needed
        }
        printf("\n");
    }
}

int main() {

    printf("hello testing");
    storeGrammarRules(); // Ensure this function is called to populate the grammarRules
    //printGrammarRules(); // Call the function to print the rules

    // Free allocated memory here if needed
    return 0;
}