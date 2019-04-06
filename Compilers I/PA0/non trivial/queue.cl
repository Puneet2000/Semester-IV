class Node { -- Node class
	value : Int; -- value stored
	next : Node; -- next element

	getValue() : Int { value }; -- get stored value

	getNext() : Node { next }; -- get next node

	setValue(val : Int) : Node {{ -- set stored value
		value <- val;
		self;
	}};

	setNext(nxt : Node) : Node {{ -- set next value
		next <- nxt;
		self;
	}};
};

class Queue inherits IO { -- queue class
	front : Node; -- front and rear
	rear : Node;
	val : Int;

	push(val : Int) : Object {
		if isvoid rear then { -- push in empty queue
			front <- (new Node).setValue(val);
			rear <- front;
		}
		else { -- push in non empty queue , shift rear
			rear.setNext((new Node).setValue(val));
			rear <- rear.getNext();
		}
		fi
	};

	pop() : Object { -- pop function
		if isvoid front then -- queue is empty
			out_string("Queue empty\n")
		else {
			val <- front.getValue(); -- get the value , change rear and front and print value
			front <- front.getNext();
			if isvoid front then {
				rear <- front;
				out_int(val);
			}
			else out_int(val)
			fi;
			out_string("\n");
		}
		fi
	};
};

class Main inherits IO{
	queue : Queue;
	val : Int;
	query : String;
	main() : Object {
		let queue : Queue <- (new Queue) in 
		    while (true) loop { -- continue forever
		    	query <- in_string();
		    	if query = "push" then { -- push
		    		val <- in_int();
		    		queue.push(val);
		    	}
		    	else 
		    		if query = "pop" then  --pop
		    			queue.pop()
		    		else
		    			out_string("Invalid input\n")
		    		fi
		    	fi;
		    }
		    pool
	};
};