class A2I {

     c2i(char : String) : Int {
	if char = "0" then 0 else
	if char = "1" then 1 else
	if char = "2" then 2 else
        if char = "3" then 3 else
        if char = "4" then 4 else
        if char = "5" then 5 else
        if char = "6" then 6 else
        if char = "7" then 7 else
        if char = "8" then 8 else
        if char = "9" then 9 else
        { abort(); 0; }  -- the 0 is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

     i2c(i : Int) : String {
		if i = 0 then "0" else
		if i = 1 then "1" else
		if i = 2 then "2" else
		if i = 3 then "3" else
		if i = 4 then "4" else
		if i = 5 then "5" else
		if i = 6 then "6" else
		if i = 7 then "7" else
		if i = 8 then "8" else
		if i = 9 then "9" else
		{ abort(); ""; }  -- the "" is needed to satisfy the typchecker
	        fi fi fi fi fi fi fi fi fi fi
     };

    a2i(s : String) : Int {
        if s.length() = 0 then 0 else
			if s.substr(0,1) = "-" then ~a2i_aux(s.substr(1,s.length()-1)) else
		        if s.substr(0,1) = "+" then a2i_aux(s.substr(1,s.length()-1)) else
		           a2i_aux(s)
		        fi fi fi
     };

    a2i_aux(s : String) : Int {
		(let int : Int <- 0 in	
	           {	
	               (let j : Int <- s.length() in
		          (let i : Int <- 0 in
			    while i < j loop
				{
				    int <- int * 10 + c2i(s.substr(i,1));
				    i <- i + 1;
				}
			    pool
			  )
		       );
	              int;
		    }
	        )
     };

    i2a(i : Int) : String {
		if i = 0 then "0" else 
	        if 0 < i then i2a_aux(i) else
	          "-".concat(i2a_aux(i * ~1)) 
	        fi fi
    };

    i2a_aux(i : Int) : String {
        if i = 0 then "" else 
	    (let next : Int <- i / 10 in
		i2a_aux(next).concat(i2c(i - next * 10))
	    )
        fi
    };
};

class Node { -- Node class
	value : String; -- value stored
	next : Node; -- next element
	prev : Node;

	getValue() : String { value }; -- get stored value

	getNext() : Node { next }; -- get next node

	getPrev() : Node {prev};

	setValue(val : String) : Node {{ -- set stored value
		value <- val;
		self;
	}};

	setNext(nxt : Node) : Node {{ -- set next value
		next <- nxt;
		self;
	}};

	setPrev(pre : Node) : Node {{ -- set next value
		prev <- pre;
		self;
	}};
};

class Stack inherits IO{
	top : Node;

	push(x:String) : Object {
		if not x = "" then 
			if isvoid top then 
				top <- (new Node).setValue(x)
			else 
				let temp : Node <- top in {
					top.setNext((new Node).setValue(x));
					top <- top.getNext();
					top.setPrev(temp);
				}
			fi
		else 0 fi
	};

	pop() : String {
		if isvoid top then "" else 
			let temp : Node <- top in {
				top <- top.getPrev();
				if not isvoid top then {
					top.setNext(temp.getNext());
					temp.getValue();
				}
				else
					temp.getValue()
				fi;
			}
		fi
	};

	print() : Object {
		let temp : Node <- top in {
			while not isvoid temp loop {
				out_string((temp.getValue()).concat(" "));
				temp <- temp.getPrev();
			}
			pool;
			out_string("\n");
		}};

	evaluate() : Object {
	    let var : String <- pop() in 
	      if var = "" then 0 else 
	      	if var = "+" then
	      		let num1 : Int <- (new A2I).a2i(pop()) in
	      			let num2 : Int <- (new A2I).a2i(pop()) in
	      				push((new A2I).i2a(num1+num2))
	      	else
	      		if var = "s" then
	      			let num1 : String <- pop() in
	      				let num2 : String <- pop() in {
	      					push(num1);
	      					push(num2);
	      				}
	      		else push(var)
	      		fi
	      	fi
	      fi
	  };
};

class Main inherits IO{
	stack : Stack;
	query : String;
	main() : Object {
		let stack : Stack <- (new Stack) in {
			out_string("> ");
			query <- in_string();
			while not query = "x" loop {
				if query = "e" then stack.evaluate() else
				if query = "d" then stack.print() else stack.push(query)
				fi fi;
				out_string("> ");
				query <- in_string();
			}
			pool;
		}
	};
};