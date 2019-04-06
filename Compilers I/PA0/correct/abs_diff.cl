class Main inherits IO{ -- find the absolute difference of two integers
	main() : Object { let x : Int <- in_int() in	-- take first input
			let y : Int <- in_int() in              -- take second input
				if (x-y) < 0 then                   -- check the difference
					out_int(y-x)                    -- output same if positive otherwise negate it.
				else
					out_int(x-y)
				fi	
	};
};