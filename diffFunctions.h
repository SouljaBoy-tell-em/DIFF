FUNCTIONS(log, 1,
{
	return NodeOpDiv (diff (dumpFile, node->right), NodeOpMul (node->right, (NodeOpFunc (num (10), FUNC_ln))));
}
		   PrintLog  (dumpFile, node);
)

FUNCTIONS(ln, 2, 
{
	return NodeOpDiv (diff (dumpFile, node->right), CopyUnderTheTree (node->right));
}
		   PrintLn   (dumpFile, node);
)

FUNCTIONS(sin, 3, 
{
	return NodeOpMul (NodeOpFunc (node->right, FUNC_cos), diff (dumpFile, node->right));
}, 
		   PrintSin  (dumpFile, node);
)

FUNCTIONS(cos, 4, 
{
	return NodeOpMul (NodeOpMul (NodeOpFunc (node->right, FUNC_sin), num (-1)), diff (dumpFile, node->right));
}
		   PrintCos  (dumpFile, node);
)

FUNCTIONS(tg, 5, 
{
	return NodeOpDiv (NodeOpSub 
		  			 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_sin)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_cos)) ), 
		   			  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sin)), diff (dumpFile,   NodeOpFunc (node->right, FUNC_cos)))),
		   			  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_cos)), num  				 	  					     (2)));
}
		   PrintTg   (dumpFile, node);
)

FUNCTIONS(ctg, 6, 
{
	return NodeOpDiv (NodeOpSub 
	      			 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_cos)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sin)) ), 
	       			  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_cos)), diff (dumpFile,   NodeOpFunc (node->right, FUNC_sin)))),
	       			  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sin)), num                                			     (2)));
}
		   PrintCtg  (dumpFile, node);
)

FUNCTIONS(sh, 7, 
{
	return NodeOpMul (NodeOpFunc (node->right, FUNC_ch), diff (dumpFile, node->right));
}
		   PrintSh   (dumpFile, node);
)

FUNCTIONS(ch, 8, 
{
	return NodeOpMul (NodeOpFunc (node->right, FUNC_sh), diff (dumpFile, node->right));
}
		   PrintCh   (dumpFile, node);
)

FUNCTIONS(th, 9, 
{
	return NodeOpDiv (NodeOpSub 
					 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_sh)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_ch)) ), 
					  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sh)), diff (dumpFile,   NodeOpFunc (node->right, FUNC_ch)))),
					  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_ch)), num  						   					   (2)));
}
		    PrintTh  (dumpFile, node);
)

FUNCTIONS(cth, 10, 
{
	return NodeOpDiv (NodeOpSub 
					 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_ch)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sh))), 
					  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_ch)), diff (dumpFile,  NodeOpFunc (node->right, FUNC_sh)))),
					  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sh)), num  											  (2)));

}
		   PrintCth  (dumpFile, node);
)
