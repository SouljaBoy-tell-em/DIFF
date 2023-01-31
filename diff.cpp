#include "diffHeader.h"
#include "diff.h"


int main (void) {

	FILE * dumpFile = fopen ("equation.txt", "r");
	FILE * graphDumpFile = fopen ("graphDump.txt", "w");
	Tree tree = {};

	InitializeNode (&(tree.head), dumpFile, NULL);
	print (graphDumpFile, tree.head);
	closeFiles (dumpFile, graphDumpFile);

	return 0;
}
