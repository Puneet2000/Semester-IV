class Node { -- node class
	value : Int; -- value stored
	left : Node; -- left children
	right : Node; -- right children

	leftNode() : Node { left }; -- left node access

	rightNode() : Node { right }; -- right node access

	getVal() : Int { value }; -- get stored value

	setRightNode(r : Node) : Node {{ -- change the right node
		right <- r;
		self;
	}};

	setLeftNode(l : Node) : Node {{ -- change the left node
		left <- l;
		self;
	}};

	setValue(val : Int) : Node {{ -- change the value
		value <- val;
		self;
	}};
};
class Main inherits IO{
	root : Node; -- root node
	n : Int;
	i : Int <- 0;
	val : Int;
	main() : Object {{
		n <- in_int();
	    while (i<n) loop { -- insert n integers
	    	val <- in_int();
	    	root <- insert(root,val);
	    	i <- i+1 ;
	    }
	    pool;
	    inorder(root); -- find inorder traversal
	    out_string("\n");
	}};

	insert(node : Node , val : Int) : Node { -- insert in BST
		if isvoid node then  -- return newly allocated node
			(new Node).setValue(val)
		else {
			if node.getVal() < val then -- go to right subtree
				node.setRightNode(insert(node.rightNode(),val))
			else
				node.setLeftNode(insert(node.leftNode(),val)) -- go to left subtree
			fi;
		}
		fi
	};

	inorder(node: Node) : Object {
		if not isvoid node then { -- Print inorder LVR 
			inorder(node.leftNode());
			out_int(node.getVal());
			out_string(" ");
			inorder(node.rightNode());
		}
		else 0 fi
	};
};