#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


enum error_code {

    ERROR_OFF,
    ERROR_ON
};


enum graph_dump_commands {

	HEAD,
	FIRSTLY
};


enum Type {

	OP,
	NUM
};


const char * YES = "y";
const char * NO =  "n";
const char * GREEN = "green";
const char * RED = "red";
const char * ORANGE = "orange";


#define MAXLENTITLE 		100
#define STARTLETTERGRAPHDUMP 23


#define CHECK_ERROR(condition, message_error) 		      \
            do {                                          \
               if  (condition) {                          \
                   printf ("%s", message_error);          \
                   return ERROR_ON;                       \
               }                                          \
            } while(false)


typedef struct node {

	struct node * parent;
	struct node *   left;
	struct node *  right;
	enum   Type     type;
	union {

		char            op;
		int   		number;
	};
} Node;

typedef struct {

	Node *      head;
	int         size;
} Tree;


int constructor (Node ** currentNode, Node * parentCurrentNode);
int graphDump (Node * head);
void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile, int * commandGraphDump);
int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode);


int main (void) {

	FILE * dumpFile = fopen ("equation.txt", "r");
	Tree tree = {};
	InitializeNode (&(tree.head), dumpFile, NULL);
	graphDump (tree.head);

	return 0;
}


int constructor (Node ** currentNode, Node * parentCurrentNode) {

	* currentNode = (Node * )          malloc   							  (sizeof (Node));
	CHECK_ERROR(!( * currentNode), 		   "Problem with allocating memory for currentNode.");

	//( * currentNode)->data = (char * ) malloc 				    (MAXLENTITLE * sizeof (char));
	//CHECK_ERROR(!( * currentNode), "Problem with allocating memory for currentNode->data.\n");

	( * currentNode)->right  =               NULL;
	( * currentNode)->left   =               NULL;
	( * currentNode)->parent =  parentCurrentNode;

	return ERROR_OFF;
}


int graphDump (Node * head) {

	FILE * graphDumpFile = fopen ("graphDump.txt", "w");
	CHECK_ERROR(!graphDumpFile, "Problem with opening graphDump.txt.\n");

	int commandGraphDump = HEAD;
	fprintf (graphDumpFile, "digraph G {\n rankdir=L\n");
	graphDumpDrawNode (head, graphDumpFile, &commandGraphDump);
	fprintf (graphDumpFile, "}");
	fseek (graphDumpFile, STARTLETTERGRAPHDUMP, SEEK_SET);
	fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%c\"];\n",  
			 head, ORANGE, head->op);	

	fclose (graphDumpFile);

	return ERROR_OFF;
}


void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile, int * commandGraphDump) {

	if (currentNode != NULL) {

		if (( * commandGraphDump) != HEAD) {

			if (currentNode->type == OP) {

				if (( * commandGraphDump) == FIRSTLY)
				fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%c\"];\n",  
				currentNode, ORANGE, (currentNode->parent)->op);

				if ((currentNode->parent)->left == currentNode)
					fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%c\"];\n",
					currentNode, 			  GREEN,           currentNode->op);

				if ((currentNode->parent)->right == currentNode)
					fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%c\"];\n",  
					currentNode, 				RED,    	   currentNode->op);

				fprintf (graphDumpFile, "block%p -> block%p\n",
					currentNode->parent, 				   currentNode);
			}

			if (currentNode->type == NUM) {

				if (( * commandGraphDump) == FIRSTLY)
					fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%d\"];\n",  
					currentNode, ORANGE, (currentNode->parent)->number);

				if ((currentNode->parent)->left == currentNode)
					fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%d\"];\n",
					currentNode, 			  GREEN,           currentNode->number);

				if ((currentNode->parent)->right == currentNode)
					fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%d\"];\n",  
					currentNode, 				RED,    	   currentNode->number);

				fprintf (graphDumpFile, "block%p -> block%p\n",
					currentNode->parent, 				   currentNode);
			}
		}

		( * commandGraphDump)++                                                ;
		graphDumpDrawNode (currentNode->left, graphDumpFile,  commandGraphDump);
		graphDumpDrawNode (currentNode->right, graphDumpFile, commandGraphDump);
	}
}


int detectArgument (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode) {

	char charBuffer = '\0';
	int intBuffer   =   0 ;
	fscanf (dumpFile, "%c", &charBuffer);

	if (!isdigit (charBuffer)) {

		( * currentNode)->type = OP;
		( * currentNode)->op = charBuffer;
		return ERROR_OFF;
	}

	else {

		ungetc (charBuffer, dumpFile);
		fscanf (dumpFile, "%d", &intBuffer);
		( * currentNode)->type   = NUM;
		( * currentNode)->number = intBuffer;
	}


	return ERROR_OFF;
}


int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode) {

	char bracketBuffer = '\0';
	fscanf (dumpFile, "%c", &bracketBuffer);

	if (bracketBuffer == '(') {

		constructor (currentNode, 						parentCurrentNode);
		InitializeNode (&( * currentNode)->left,  dumpFile, * currentNode);
		detectArgument (currentNode, dumpFile, 			parentCurrentNode);
		InitializeNode (&( * currentNode)->right, dumpFile, * currentNode);
	}

	else {

		ungetc (bracketBuffer, dumpFile);
		return ERROR_OFF;
	}

	fscanf (dumpFile, "%c", &bracketBuffer);

	return ERROR_OFF;
}
