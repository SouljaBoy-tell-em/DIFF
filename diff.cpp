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

	HEAD,
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


const char * GREEN = "green";
const char * RED = "red";
const char * ORANGE = "orange";


#define EXP     		   2.71
#define LENFUNCTION           5
#define MAXLENTITLE 		100
#define STARTLETTERGRAPHDUMP 23


#define CHECK_ERROR(condition, message_error) 		      \
            do {                                          \
               if  (condition) {                          \
                   printf ("%s", message_error);          \
                   return ERROR_ON;                       \
               }                                          \
            } while(false)


#define FUNCTIONS(name, num, amountSigns, ...) \
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
int constructor (Node ** currentNode, Node * parentCurrentNode);
Node * CopyUnderTheTree (Node * node);
Node * diff (FILE * dumpFile, Node * node);
void GraphDiffNode (FILE * dumpFile, Node * node);
void diffNode(FILE * dumpFile, Node * node);
int graphDump (Node * head);
void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile, int * commandGraphDump);
void GraphNodeOpAdd (FILE * dumpFile, Node * node);
void GraphNodeOpSub (FILE * dumpFile, Node * node);
void GraphTreePrint (FILE * dumpFile, Node * node);
int InitializeNode (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode);
Node * nodeAdd (Type type, Node * node, Node currentNode, Node * left, Node * right);
Node * NodeOpAdd (Node * left, Node * right);
Node * NodeOpDegree (Node * left, Node * right);
Node * NodeOpDiv (Node * left, Node * right);
Node * NodeOpMul (Node * left, Node * right);
Node * NodeOpSub (Node * left, Node * right);
Node * NodeOpFunc (Node * node, enum func _FUNC);
Node * num (int num);
void print (FILE * dumpFile, Node * node);


int main (void) {

	FILE * graphDumpFile = fopen ("graphDump.txt", "w");
	FILE * dumpFile = fopen ("equation.txt", "r");
	Tree tree = {};
	InitializeNode (&(tree.head), dumpFile, NULL);
	//graphDump (tree.head);
	print (graphDumpFile, tree.head);

	return 0;
}


void checkIn (FILE * dumpFile) {

    fprintf (dumpFile, "\\documentclass[a4paper,12pt]{article}\n\
\\usepackage[a4paper,top=1.3cm,bottom=2cm,left=1.5cm,right=1.5cm,marginparwidth=0.75cm]{geometry}\n\
\\usepackage{cmap}																				 \n\
\\usepackage[warn]{mathtext} 																	 \n\
\\usepackage[T2A]{fontenc}																		 \n\
\\usepackage[utf8]{inputenc}																	 \n\
\\usepackage[english,russian]{babel}														     \n\
\\usepackage{physics}																			 \n\
\\usepackage{multirow}     																	     \n\
\\allowdisplaybreaks																			 \n\
																								 \n\
\\usepackage{float}																				 \n\
\\restylefloat{table}																			 \n\
																								 \n\
																								 \n\
																								 \n\
\\usepackage{graphicx}																			 \n\
																								 \n\
\\usepackage{wrapfig}																			 \n\
\\usepackage{tabularx}																			 \n\
																								 \n\
\\usepackage{hyperref}																			 \n\
\\usepackage[rgb]{xcolor}																		 \n\
\\hypersetup{																					 \n\
	colorlinks=true,urlcolor=blue																 \n\
}																								 \n\
																								 \n\
\\usepackage{pgfplots}																			 \n\
\\pgfplotsset{compat=1.9}																		 \n\
																								 \n\
																								 \n\
\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}		 									 \n\
\\usepackage{icomma}																			 \n\
																								 \n\
																								 \n\
\\mathtoolsset{showonlyrefs=true}																 \n\
																								 \n\
																								 \n\
\\usepackage{euscript}																			 \n\
\\usepackage{mathrsfs} 																			 \n\
																								 \n\
																							  	 \n\
\\DeclareMathOperator{\\sgn}{\\mathop{sgn}}														 \n\
																								 \n\
																								 \n\
\\newcommand*{\\hm}[1]{#1\\nobreak\\discretionary{}												 \n\
	{\\hbox{$\\mathsurround=0pt #1$}}{}}														 \n\
																								 \n\
\\date{\\today}																					 \n\
																								 \n\
\\usepackage{gensymb}																			 \n\
																								 \n\
\\title{Взятие производной}																		 \n\
\\begin{document}																				 \n\
\\maketitle																						 \n");

    fprintf (dumpFile, "\\section{}\n");
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


Node * CopyUnderTheTree (Node * node) {

	if (node->left)
		node->left = CopyUnderTheTree (node->left);

	if (node->right)
		node->right = CopyUnderTheTree (node->right);

	Node * currentNode = (Node * ) malloc (sizeof (Node));

	if (node->type == NUM) {	// TO DO: currentNode = node;

		currentNode->type = 	  NUM;
		currentNode->num  = node->num;
	}

	if (node->type == OP) {

		currentNode->type = 	  OP;
		currentNode->op   = node->op;
	}

	if (node->type == VAR) {

		currentNode->type =      VAR;
		currentNode->op   = node->op;
	}

	if (node->left)
		currentNode->left = (Node * ) malloc (sizeof (Node));

	if (node->right)
		currentNode->right = (Node * ) malloc (sizeof (Node));

	if (node->left)
		* (currentNode->left) = * (node->left);

	if (node->right)
		* (currentNode->right) = * (node->right);

	return currentNode;
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
					currentNode, ORANGE, (currentNode->parent)->num);

				if ((currentNode->parent)->left == currentNode)
					fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%d\"];\n",
					currentNode, 			  GREEN,           currentNode->num);

				if ((currentNode->parent)->right == currentNode)
					fprintf (graphDumpFile, "block%p [shape=record, color=\"%s\", label=\"%d\"];\n",  
					currentNode, 				RED,    	   currentNode->num);

				fprintf (graphDumpFile, "block%p -> block%p\n",
					currentNode->parent, 				   currentNode);
			}

			if (currentNode->type == VAR) {

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
		}

		( * commandGraphDump)++                                                ;
		graphDumpDrawNode (currentNode->left, graphDumpFile,  commandGraphDump);
		graphDumpDrawNode (currentNode->right, graphDumpFile, commandGraphDump);
	}
}


int detectArgument (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode) {

	char opBuffer   		                 =             '\0';
	int  intBuffer                           =               0 , 
		 pointerFilePlaceStart               = ftell (dumpFile), 
		 pointerFilePlaceFinish              =               0 ;
	bool isNumber                            =            false;

	if (fscanf (dumpFile, "%d", &intBuffer) == 1)
		isNumber = true;

	if (isNumber) {

		( * currentNode)->type =       NUM;
		( * currentNode)->num  = intBuffer;
		return ERROR_OFF;
	}

	pointerFilePlaceFinish = ftell (dumpFile);
	fseek (dumpFile, pointerFilePlaceStart - pointerFilePlaceFinish, SEEK_CUR);
	fscanf (dumpFile, "%c", &opBuffer);

	if (strchr ("*/+-^", opBuffer) && !isNumber) {

		( * currentNode)->type = OP;
		( * currentNode)->op = opBuffer;
		return ERROR_OFF;
	}

	if (opBuffer == 'x' && !isNumber) {

		( * currentNode)->type =       VAR;
		( * currentNode)->op   =  opBuffer;
		return ERROR_OFF;
	}


	pointerFilePlaceFinish = ftell (dumpFile);
	fseek (dumpFile, pointerFilePlaceStart - pointerFilePlaceFinish, SEEK_CUR);
	char funcCommandBuffer [LENFUNCTION] = "\0";
	fgets (funcCommandBuffer, 4, dumpFile);
	printf ("command: %s\n", funcCommandBuffer);

	if (!strcmp (funcCommandBuffer, "log")) {

		( * currentNode)->type =  OP;
		( * currentNode)->num  = FUNC_log;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "ln(")) {

		ungetc ('(', dumpFile);
		( * currentNode)->type = OP;
		( * currentNode)->num  = FUNC_ln;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "sin")) {

		( * currentNode)->type =  OP;
		( * currentNode)->num  = FUNC_sin;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "cos")) {

		( * currentNode)->type =  OP;
		( * currentNode)->num  = FUNC_cos;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "tg(")) {

		ungetc ('(', dumpFile);
		( * currentNode)->type = OP;
		( * currentNode)->num  = FUNC_tg;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "ctg")) {

		( * currentNode)->type =  OP;
		( * currentNode)->num  = FUNC_ctg;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "exp")) {

		( * currentNode)->type   =  OP;
		( * currentNode)->dbl    = EXP;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "sh(")) {

		ungetc ('(', dumpFile);
		( * currentNode)->type = OP;
		( * currentNode)->num  = FUNC_sh;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "ch(")) {

		ungetc ('(', dumpFile);
		( * currentNode)->type = OP;
		( * currentNode)->num  = FUNC_ch;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "th(")) {

		ungetc ('(', dumpFile);
		( * currentNode)->type = OP;
		( * currentNode)->num  = FUNC_th;
		return ERROR_OFF;
	}

	if (!strcmp (funcCommandBuffer, "cth")) {

		( * currentNode)->type =  OP;
		( * currentNode)->num  = FUNC_cth;
		return ERROR_OFF;
	}

	else {

		printf ("Function %s not found.\n", funcCommandBuffer);
		exit (EXIT_FAILURE);
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


Node * diff (FILE * dumpFile, Node * node) {

	switch (node->type) {

		case NUM:
			return num (0);
			break;

		case VAR: 
			return num (1);
			break;

		case OP:
			switch (node->op) {

				case ADD:
					return NodeOpAdd (diff (dumpFile, node->left), diff (dumpFile, node->right)); 
						// NodeOpAdd (dL, dR);
					break;

				case SUB:
					return NodeOpSub (diff (dumpFile, node->left), diff (dumpFile, node->right));
						// NodeOpSub (dL, dR);
					break;

				case MUL:
					return NodeOpAdd (NodeOpMul (diff (dumpFile, node->left), CopyUnderTheTree (node->right)), 
									  NodeOpMul (CopyUnderTheTree (node->left), diff (dumpFile, node->right)));
						// NodeOpAdd (NodeOpMul (dL, cR), NodeOpMul (cL, dR));
					break;

				case DEG:
					if ((node->left)->type == NUM) {

						if ((node->right)->type == NUM)
							return num (0);
							// (const) ^ const;

						else
							return NodeOpMul (NodeOpMul (NodeOpDegree (node->left, node->right), NodeOpFunc (node->left, FUNC_ln)), 
																				  			 		 diff (dumpFile, node->right));
							// (const) ^ f(x); NodeOpMul (NodeOpMul (NodeOpDegree (left, right), Ln (left)), dR));
					}

					else if ((node->right)->type == NUM) {

						Node * currentNode = (Node * ) malloc (sizeof (Node));
						currentNode->type = NUM;
						currentNode->num = (node->right)->num - 1;

						return NodeOpMul (NodeOpMul (NodeOpDegree (node->left, currentNode), CopyUnderTheTree (node->right)), 
																							 diff (dumpFile,    node->left));
						// (f(x)) ^ const; NodeOpMul (NodeOpMul (NodeOpDegree (left), cR), dL);
					}

					else
						return NodeOpMul (NodeOpDegree (node->left, node->right), diff (dumpFile, NodeOpMul (node->right, 
																					 NodeOpFunc (node->left, FUNC_ln))));
						// (f(x)) ^ g(x); NodeOpMul (NodeOpDegree (left, right), d (NodeOpMul (right, ln (left))));
					break;

				case DIV:
					return NodeOpDiv (NodeOpSub (NodeOpMul (diff (dumpFile, node->left), CopyUnderTheTree (node->right)), 
						   NodeOpMul 					  (CopyUnderTheTree (node->left), diff (dumpFile, node->right))), 
						   NodeOpDegree 									  (CopyUnderTheTree (node->right), num (2)));
						// (f(x)/g(x))' = (f'(x)g(x) - f(x)g'(x))/g(x)^2;
					break;

				default:
					break;
			}


			#define FUNCTIONS(name, num, amountSigns, ...) \
				case num:								   \
					__VA_ARGS__							   \
					break;	

			switch (node->num) {

				#include "diffFunctions.h"
				#undef FUNCTIONS

				default:
					printf ("So operation not found. Num command: %d\n", node->num);
					exit (EXIT_FAILURE);
					break;
			}

		default:
			printf ("So function not found.\n");
			exit (EXIT_FAILURE);
			break;

	}
}


Node * NodeOpAdd (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = ADD;

	return nodeAdd (OP, NULL, currentNode, left, right);
}


Node * NodeOpSub (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = SUB;

	return nodeAdd (OP, NULL, currentNode, left, right);
}


Node * NodeOpMul (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = MUL;

	return nodeAdd (OP, NULL, currentNode, CopyUnderTheTree (left), 
										   CopyUnderTheTree (right));
}


Node * NodeOpDiv (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = DIV;

	return nodeAdd (OP, NULL, currentNode, CopyUnderTheTree (left), 
										   CopyUnderTheTree (right));
}


Node * NodeOpDegree (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = DEG;

	return nodeAdd (OP, NULL, currentNode, CopyUnderTheTree (left), 
										   CopyUnderTheTree (right));
}


Node * NodeOpFunc (Node * node, enum func _FUNC) {
	
	Node currentNode =  {};
	currentNode.op   = _FUNC;

	return nodeAdd (OP, NULL, currentNode, NULL, CopyUnderTheTree (node));
}


void GraphNodeOpAdd(FILE * dumpFile, Node * node) {

    fprintf (dumpFile, "$$ ");
    GraphDiffNode (dumpFile, node);
    fprintf (dumpFile, " = ");
    GraphDiffNode (dumpFile, node->left);
    fprintf (dumpFile, " + ");
    GraphDiffNode (dumpFile, node->right);
    fprintf (dumpFile, "$$\n \\newline");
}

void GraphNodeOpSub (FILE * dumpFile, Node * node) {

	fprintf (dumpFile, "$$ ");
	GraphDiffNode (dumpFile, node);
	fprintf (dumpFile, " = ");
	GraphDiffNode (dumpFile, node->left);
	fprintf (dumpFile, " - ");
	GraphDiffNode (dumpFile, node->right);
	fprintf (dumpFile, "$$\n \\newline");
}


void GraphDiffNode (FILE * dumpFile, Node * node) {

    fprintf (dumpFile, " \\left(");
    GraphTreePrint (dumpFile, node);
    fprintf (dumpFile, " \\right)'");
}


void GraphTreePrint (FILE * dumpFile, Node * node) {

	switch (node->type) {

		case NUM:
			printf ("%d", node->num);
			fprintf (dumpFile, "%d", node->num);
			break;

		case VAR:
			printf ("%c", node->op);
			fprintf (dumpFile, "%c", node->op);
			break;

		case OP:
			switch (node->op) {

				case ADD: 
            		fprintf (dumpFile, "\\left(");
            		if (node->left) 
            			GraphTreePrint (dumpFile, node->left);
            		fprintf (dumpFile, "%c", node->op);
            		if (node->right) 
            			GraphTreePrint (dumpFile, node->right);
            		fprintf (dumpFile, "\\right)");
            		break;

            	case SUB:
            		fprintf (dumpFile, "\\left(");
            		if (node->left) 
            			GraphTreePrint (dumpFile, node->left);
            		fprintf (dumpFile, "%c", node->op);
            		if (node->right) 
            			GraphTreePrint (dumpFile, node->right);
            		fprintf (dumpFile, "\\right)");
            		break;

            	case MUL:
            		if (node->left)
            			GraphTreePrint (dumpFile, node->left);
            		fprintf (dumpFile, "\\cdot ");
            		if (node->right)
            			GraphTreePrint (dumpFile, node->right);
            		break;

            	case DIV:
            		fprintf (dumpFile, "\\frac{");
           		 	if (node->left) 
           		 		GraphTreePrint (dumpFile, node->left);
            		fprintf (dumpFile, "}{");
            		if (node->right) 
            			GraphTreePrint (dumpFile, node->right);
            		fprintf (dumpFile, "}");
            		break;

            	case DEG:
            		if (node->left) {
               	 		fprintf (dumpFile, " \\left( ");
                		GraphTreePrint (dumpFile, node->left);
                		fprintf (dumpFile, " \\right) ");
            		}
            		fprintf (dumpFile, "%c", node->op);
            		fprintf (dumpFile, "{");
            		if (node->right) 
            			GraphTreePrint (dumpFile, node->right);
            		fprintf(dumpFile, "}");
            		break;

            	default:
            		break;
            	}

            	#define FUNCTIONS(name, num, amountSigns, ...) 		\
            		case num:								  		\
            			fprintf (dumpFile, "%s(", #name);	  		\
            			if (node->left)								\
            				GraphTreePrint (dumpFile, node->left);	\
            			if (node->right)							\
            				GraphTreePrint (dumpFile, node->right); \
            			fprintf (dumpFile, ")");					\
            			break;

            	switch (node->num) {

            		#include "diffFunctions.h"
            		#undef FUNCTIONS

            		default:
            			break;

            	}

            	default:
					break;
			}
}


Node * nodeAdd (Type type, Node * node, Node currentNode, Node * left, Node * right) {

	node = (Node * ) malloc (sizeof (Node));

	* node      = currentNode;
	node->type  =        type;
	node->left  =        left;
	node->right =       right;

	return node;
}


Node * num (int num) {

	Node currentNode =  {};
	currentNode.num  = num;
	return nodeAdd (NUM, NULL, currentNode, NULL, NULL);
}


void print (FILE * dumpFile, Node * node) {

	Node * nodeDiff = NULL;

	checkIn (dumpFile);
	nodeDiff = diff (dumpFile, node);

	fprintf (dumpFile, " TOTAL: \n \\newline ");
    fprintf (dumpFile, " $$ ");
    GraphTreePrint (dumpFile, nodeDiff);
    fprintf (dumpFile, " $$ ");
    fprintf (dumpFile, "\\end{document}\n");
}
		




