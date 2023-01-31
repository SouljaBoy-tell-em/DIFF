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
	\\maketitle					   \n");
    fprintf (dumpFile, "\\section{}\n");
}


void closeFiles (FILE * dumpFile, FILE * graphDumpFile) {

	fclose (graphDumpFile);
	fclose (dumpFile);
}


int constructor (Node ** currentNode, Node * parentCurrentNode) {

	* currentNode = (Node * ) malloc   		            					  (sizeof (Node));
	CHECK_ERROR(!( * currentNode), 		   "Problem with allocating memory for currentNode.");

	( * currentNode)->right  =               NULL;
	( * currentNode)->left   =               NULL;
	( * currentNode)->parent =  parentCurrentNode;

	return ERROR_OFF;
}


int graphDump (Node * head, FILE * graphDumpFile) {

	fprintf (graphDumpFile, "digraph G {\n rankdir=L\n");
	graphDumpDrawNode (head, graphDumpFile);
	fprintf (graphDumpFile, "}");

	return ERROR_OFF;
}


Node * CopyUnderTheTree (Node * node) {

	if (node->left)
		node->left = CopyUnderTheTree (node->left);

	if (node->right)
		node->right = CopyUnderTheTree (node->right);

	Node * currentNode = (Node * ) malloc (sizeof (Node));
	CHECK_ERROR(! currentNode, "Problem with allocating memory for currentNode.");

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

	if (node->left) {

		currentNode->left = (Node * ) malloc (sizeof (Node));
		CHECK_ERROR(! currentNode->left, "Problem with allocating memory for currentNode->left.");
	}

	if (node->right) {

		currentNode->right = (Node * ) malloc (sizeof (Node));
		CHECK_ERROR(! currentNode->right, "Problem with allocating memory for currentNode->right.");
	}

	if (node->left)
		* (currentNode->left) = * (node->left);

	if (node->right)
		* (currentNode->right) = * (node->right);

	return currentNode;
}


void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile) {

	if (!currentNode)
		return;

	if (currentNode->left) {

		fprintf (graphDumpFile, "node%p->node%p[color=black];\n", currentNode, currentNode->left);
		graphDumpDrawNode (currentNode->left, graphDumpFile);
	}

	switch (currentNode->type) {

		case NUM:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%d\"];\n", currentNode, currentNode->num);
			break; 

		case VAR:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%c\"];\n", currentNode, currentNode->op);
			break;

		case OP:
			switch (currentNode->op) {

				case ADD:
					fprintf (graphDumpFile, "node%p[shape=record, label=\"%c\"];\n", currentNode, currentNode->op);
					break; 
				case SUB:
					fprintf (graphDumpFile, "node%p[shape=record, label=\"%c\"];\n", currentNode, currentNode->op);
					break; 	
				case MUL:
					fprintf (graphDumpFile, "node%p[shape=record, label=\"%c\"];\n", currentNode, currentNode->op);
					break; 
				case DIV:
					fprintf (graphDumpFile, "node%p[shape=record, label=\"%c\"];\n", currentNode, currentNode->op);
					break;
				case DEG:
					fprintf (graphDumpFile, "node%p[shape=record, label=\"%c\"];\n", currentNode, currentNode->op);
					break;

				default:
					break; 
			}

			#define FUNCTIONS(name, num, amountSigns, ...)													\
				case num:																					\
					fprintf (graphDumpFile, "node%p [shape=record, label=\""#name"\"];\n", currentNode);	\
					break;
			
			switch (currentNode->num) {

            		#include "diffFunctions.h"
            		#undef FUNCTIONS

            		default:
            			break;
            	}		

        default:
        	break;															
	}

	if (currentNode->right) {

		fprintf (graphDumpFile, "node%p->node%p[color=black];\n", currentNode, currentNode->right);
		graphDumpDrawNode (currentNode->right, graphDumpFile);
	}
}


int detectArgument (Node ** currentNode, FILE * dumpFile, Node * parentCurrentNode) {

	char opBuffer   		      = '\0';
	int  intBuffer                =   0 ;
	char funcBuffer [LENFUNCTION] = "\0";

	fscanf (dumpFile, "%c", &opBuffer);

	if (strchr ("*/+-^", opBuffer)) {

		( * currentNode)->type = OP;
		( * currentNode)->op = opBuffer;
		return ERROR_OFF;
	}

	else if (opBuffer == 'x') {

		( * currentNode)->type = VAR;
		( * currentNode)->op   = opBuffer;
		return ERROR_OFF;
	}

	else if (isalpha (opBuffer)) {

		int index = 0;
		while (isalpha (opBuffer)) {

			funcBuffer [index] = opBuffer;
			index++;
			fscanf (dumpFile, "%c", &opBuffer);
		}

		ungetc (opBuffer, dumpFile);

		/*

		#define FUNCTIONS(name, num, amountSigns, ...) \
			if (!strcmp (funcBuffer, #name)) {   	   \
				( * currentNode)->type =  OP;		   \
				( * currentNode)->num  = num;		   \
			}

		#include "diffFunctions.h"
		#undef FUNCTIONS

		*/

		if (!strcmp (funcBuffer, "log")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_log;
		}

		if (!strcmp (funcBuffer, "ln")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_ln;
		}

		if (!strcmp (funcBuffer, "sin")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_sin;
		}

		if (!strcmp (funcBuffer, "cos")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_cos;
		}

		if (!strcmp (funcBuffer, "tg")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_tg;
		}

		if (!strcmp (funcBuffer, "ctg")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_ctg;
		}

		if (!strcmp (funcBuffer, "sh")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_sh;
		}

		if (!strcmp (funcBuffer, "ch")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_ch;
		}

		if (!strcmp (funcBuffer, "th")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_sin;
		}

		if (!strcmp (funcBuffer, "cth")) {

			( * currentNode)->type =  OP;
			( * currentNode)->num  = FUNC_sin;
		}
	}


	else {

		ungetc (opBuffer, dumpFile);
		fscanf (dumpFile, "%d", &intBuffer);
		( * currentNode)->type = NUM;
		( * currentNode)->num  = intBuffer;
	}

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
						CHECK_ERROR(! currentNode, "Problem with allocating memory for currentNode.");

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


void GraphDiffNode (FILE * dumpFile, Node * node) {

    fprintf (dumpFile, " \\left(");
    GraphTreePrint (dumpFile, node);
    fprintf (dumpFile, " \\right)'");
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


void NodeDtor (Node * node) {

	if (node) {

		NodeDtor (node->left);
		NodeDtor (node->right);

		free (node);
	}
}


Node * NodeOpAdd (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = ADD;

	return NodeAdd (OP, NULL, currentNode, left, right);
}


Node * NodeOpSub (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = SUB;

	return NodeAdd (OP, NULL, currentNode, left, right);
}


Node * NodeOpMul (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = MUL;

	return NodeAdd (OP, NULL, currentNode, CopyUnderTheTree (left), 
										   CopyUnderTheTree (right));
}


Node * NodeOpDiv (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = DIV;

	return NodeAdd (OP, NULL, currentNode, CopyUnderTheTree (left), 
										   CopyUnderTheTree (right));
}


Node * NodeOpDegree (Node * left, Node * right) {

	Node currentNode =  {};
	currentNode.op   = DEG;

	return NodeAdd (OP, NULL, currentNode, CopyUnderTheTree (left), 
										   CopyUnderTheTree (right));
}


Node * NodeOpFunc (Node * node, enum func _FUNC) {
	
	Node currentNode =  {};
	currentNode.op   = _FUNC;

	return NodeAdd (OP, NULL, currentNode, NULL, CopyUnderTheTree (node));
}


Node * NodeAdd (Type type, Node * node, Node currentNode, Node * left, Node * right) {

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
	return NodeAdd (NUM, NULL, currentNode, NULL, NULL);
}


Node * print (FILE * dumpFile, Node * node) {

	Node * nodeDiff = NULL;
	int change = NO;

	checkIn (dumpFile);
	nodeDiff = diff (dumpFile, node);

	do {

		change = NO;
		simplificationDiffTree (&nodeDiff, &change);
	} while (change == YES);


	fprintf (dumpFile, " TOTAL: \n \\newline ");
    fprintf (dumpFile, " $$ ");
    GraphTreePrint (dumpFile, nodeDiff);
    fprintf (dumpFile, " $$ ");
    fprintf (dumpFile, "\\end{document}\n");

    FILE * treeDiffFile = fopen ("treeDiff.txt", "w");
    graphDump (nodeDiff, treeDiffFile);
    fclose (treeDiffFile);

    return nodeDiff;
}


Node * simplificationDiffTree (Node ** node, int * change) {

	Node * startNode = * node;

	if (( * node)->type == OP && ( * node)->op == ADD) {

		if ((( * node)->right)->type == NUM && (( * node)->left)->type == NUM) {

			* node = ( * node)->left;
			( * node)->num += (startNode->right)->num;
			free (( * node)->right);
		}
	}

	if (( * node)->type == OP && ( * node)->op == SUB) {

		if ((( * node)->right)->type == NUM && (( * node)->left)->type == NUM) {

			* node = ( * node)->left;
			( * node)->num -= (startNode->right)->num;

			free (( * node)->left);
		}
	}

	if (( * node)->type == OP && ( * node)->op == MUL) {

		if ((( * node)->right)->type == NUM && (( * node)->right)->num      == 0) {

			* node = ( * node)->right;
			( * node)->num = 0;

			free (( * node)->left);
		}

		else if ((( * node)->left)->type == NUM && (( * node)->left)->num   == 0) {

			* node = ( * node)->left;
			( * node)->num = 0;

			free (( * node)->right);
		}

		else if ((( * node)->right)->type == NUM && (( * node)->right)->num  == 1) {

			* node =  ( * node)->left;
		}

		else if ((( * node)->left)->type == NUM && (( * node)->left)->num    == 1) {

			* node = ( * node)->right;

			free (( * node)->left);
		}
	}

	if (( * node)->type == OP && ( * node)->op == DEG) {

		if ((( * node)->right)->type == NUM && (( * node)->right)->num      == 0) {

			( * node)->type = NUM;
			( * node)->num  =   1;

			free (( * node)->left );
			free (( * node)->right);
		}

		else if ((( * node)->left)->type == NUM && (( * node)->left)->num   == 0) {

			( * node)->type = NUM;
			( * node)->num  =   1;

			free (( * node)->left );
			free (( * node)->right);
		}

		else if ((( * node)->right)->type == NUM && (( * node)->right)->num == 1) {

			* node = ( * node)->left;

			free (( * node)->right);
		}

		else if ((( * node)->right)->type == NUM && (( * node)->right)->num == 1) {

			* node = ( * node)->right;

			free (( * node)->left);
		}

	}

	if (( * node)->left)
		simplificationDiffTree (&(( * node)->left), change);

	if (( * node)->right)
		simplificationDiffTree (&(( * node)->right), change);

	if ( * node != startNode)
		* change = YES;

	return * node;
}		