FUNCTIONS(log, 1, 3,
{
	return NodeOpDiv (diff (dumpFile, node->right), NodeOpMul (node->right, (NodeOpFunc (num (10), FUNC_ln))));
})

FUNCTIONS(ln, 2, 2, 
{
	return NodeOpDiv (diff (dumpFile, node->right), CopyUnderTheTree (node->right));
})

FUNCTIONS(sin, 3, 3, 
{
	return NodeOpMul (NodeOpFunc (node->right, FUNC_cos), diff (dumpFile, node->right));
})

FUNCTIONS(cos, 4, 3, 
{
	return NodeOpMul (NodeOpMul (NodeOpFunc (node->right, FUNC_sin), num (-1)), diff (dumpFile, node->right));
})

FUNCTIONS(tg, 5, 2, 
{
	return NodeOpDiv (NodeOpSub 
		  			 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_sin)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_cos)) ), 
		   			  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sin)), diff (dumpFile,   NodeOpFunc (node->right, FUNC_cos)))),
		   			  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_cos)), num  				 	  					     (2)));
})

FUNCTIONS(ctg, 6, 3, 
{
	return NodeOpDiv (NodeOpSub 
	      			 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_cos)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sin)) ), 
	       			  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_cos)), diff (dumpFile,   NodeOpFunc (node->right, FUNC_sin)))),
	       			  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sin)), num                                			     (2)));
})

FUNCTIONS(sh, 7, 2, 
{
	return NodeOpMul (NodeOpFunc (node->right, FUNC_ch), diff (dumpFile, node->right));
})

FUNCTIONS(ch, 8, 2, 
{
	return NodeOpMul (NodeOpFunc (node->right, FUNC_sh), diff (dumpFile, node->right));
})

FUNCTIONS(th, 9, 2, 
{
	return NodeOpDiv (NodeOpSub 
					 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_sh)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_ch)) ), 
					  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sh)), diff (dumpFile,   NodeOpFunc (node->right, FUNC_ch)))),
					  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_ch)), num  						   					   (2)));
})

FUNCTIONS(cth, 10, 3, 
{
	return NodeOpDiv (NodeOpSub 
					 (NodeOpMul    (diff (dumpFile,   NodeOpFunc (node->right, FUNC_ch)), CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sh))), 
					  NodeOpMul    (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_ch)), diff (dumpFile,  NodeOpFunc (node->right, FUNC_sh)))),
					  NodeOpDegree (CopyUnderTheTree (NodeOpFunc (node->right, FUNC_sh)), num  											  (2)));

})