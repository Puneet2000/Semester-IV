(*
	(*
		This is test case 1 for PA2
	*)
*)

(*
	Unwanted  token.
	*)
*)

ClaSs Node { -- node class
	str : String <- "44444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444""Hello
	str2 : String <- "4444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444""Hello
	str3 : String <- "This is String
					number three"
	str4 : String <- "\0" -- should be 0
	str5 : String <- "This is\0 string number 5" -- converted to 0
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

	str6 : String <- "This is \n\b\f\t"
	str7 : String <- "#@>=][&^%"
	str8 : String <- "Test string \\0 number 7"
	str9 : String <- "\h\e\l\l\o\t\p\un\e\e\t"
	str9 : String <- "Hello \?\\"
	a @ Int <- #$ -- unwanted symbols
	b > c%4;

	b : Bool <- True
	c : Bool <- trUE
	f : Bool <- fALSE
	g : Bool <- FAlse

	num : Int <- 00000
	num2 : Int <- 9000c
	num3 : Int <- #0900
	setValue(val : Int) : Node {{ -- change the value
		value <- val;
		SELF;
	}};
};
cLASS Main inHERIts IO{
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

	insert(node : Node , val $ Int) : Node { -- insert in BST
		if isvoid node then  -- return newly allocated node
			(new Node).setValue(val)
		else {
			if node.getVal() <= val then -- go to right subtree
				node.setRightNode(insert(node.rightNode(),val))
			else
				node.setLeftNode(insert(node.leftNode(),val)) -- go to left subtree
			fi;
		}
		fi
	};

	inorder[node: Node] : Object {
		if not isvoid node then { -- Print inorder LVR 
			inorder(node.leftNode());
			out_int(node.getVal());
			out_string(" ");
			inorder(node.rightNode());
		}
		else 0 FI
(*
	(*

*)	};
};