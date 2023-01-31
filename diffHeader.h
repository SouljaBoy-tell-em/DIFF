#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>


enum error_code {

    ERROR_OFF,
    ERROR_ON
};


enum graph_dump_commands {

	HEAD = 0,
	FIRSTLY
};


enum Type {

	OP,
	VAR,
	NUM
};


enum TypeVar {

	ADD = '+',
	SUB = '-',
	MUL = '*',
	DIV = '/',
	DEG = '^'
};


enum change {

	NO,
	YES
};



#define EXP     		   2.71
#define LENFUNCTION           5
#define MAXLENTITLE 		100
#define STARTLETTERGRAPHDUMP 23


#define CHECK_ERROR(condition, message_error) 		      \
            do {                                          \
               if  (condition) {                          \
                   printf ("%s", message_error);          \
                   exit (ERROR_ON);                       \
               }                                          \
            } while(false)


#define FUNCTIONS(name, num, ...) \
    FUNC_##name = num,

enum func {

	#include "diffFunctions.h"
};

#undef FUNCTIONS


typedef struct node {

	struct node * parent;
	struct node *   left;
	struct node *  right;
	enum   Type     type;

	union {

		double 		 dbl;
		char          op;
		int   		 num;
	};
} Node;

typedef struct {

	Node *      head;
	int         size;
} Tree;


void checkIn (FILE * dumpFile);
void closeFiles (FILE * dumpFile, FILE * graphDumpFile);
int constructor (Node ** currentNode, Node * parentCurrentNode);
Node * CopyUnderTheTree (Node * node);
Node * diff (FILE * dumpFile, Node * node);
void diffNode(FILE * dumpFile, Node * node);
void GraphDiffNode (FILE * dumpFile, Node * node);
int graphDump (Node * head, FILE * graphDumpFile);
void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile);
void GraphNodeOpAdd (FILE * dumpFile, Node * node);
void GraphNodeOpSub (FILE * dumpFile, Node * node);
void GraphTreePrint (FILE * dumpFile, Node * node);
int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode);
Node * NodeAdd (Type type, Node * node, Node currentNode, Node * left, Node * right);
Node * NodeOpAdd (Node * left, Node * right);
Node * NodeOpDegree (Node * left, Node * right);
Node * NodeOpDiv (Node * left, Node * right);
void   NodeDtor (Node * node);
Node * NodeOpMul (Node * left, Node * right);
Node * NodeOpSub (Node * left, Node * right);
Node * NodeOpFunc (Node * node, enum func _FUNC);
Node * num (int num);
Node * print (FILE * dumpFile, Node * node);
void PrintAdd (FILE * dumpFile, Node * node);
void PrintCh (FILE * dumpFile, Node * node);
void PrintConst (FILE * dumpFile, Node * node, int num);
void PrintCos (FILE * dumpFile, Node * node);
void PrintCtg (FILE * dumpFile, Node * node);
void PrintCth (FILE * dumpFile, Node * node);
void PrintDeg1st (FILE * dumpFile, Node * node);
void PrintDeg2nd (FILE * dumpFile, Node * node);
void PrintDeg3rd (FILE * dumpFile, Node * node);
void PrintDiffNode (FILE * dumpFile, Node * node);
void PrintDiv (FILE * dumpFile, Node * node);
void PrintLn (FILE * dumpFile, Node * node);
void PrintLog (FILE * dumpFile, Node * node);
void PrintMul (FILE * dumpFile, Node * node);
void PrintNode (FILE * dumpFile, Node * node);
void PrintSin (FILE * dumpFile, Node * node);
void PrintSh (FILE * dumpFile, Node * node);
void PrintStartDiffTex (FILE * dumpFile, Node * node);
void PrintSub (FILE * dumpFile, Node * node);
void PrintTg (FILE * dumpFile, Node * node);
void PrintTh (FILE * dumpFile, Node * node);
Node * simplificationDiffTree (Node ** node, int * change);
