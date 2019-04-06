class Main inherits IO{ -- check if a number is even or odd
	main() : Object {
		let x : Int <- in_int() in	
				if (x - 2*(x/2)) = 0 then -- check if remainder is 0 or not
					out_string("Even Number\n")
				else 
					out_string("Odd Number\n")	
				fi		
	};
};