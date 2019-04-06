class Main inherits IO{
	main() : Object { -- check if triplet forms avalid triange
		let a : Int <- in_int() in	
			let b : Int <- in_int() in
				let c : Int <- in_int() in
					if c < a+b then -- check first combo enequality
						if a < b+c then --check second combo enequality
							if 	b < a+c then --check third combo enequality
								out_string("Triangle !\n")
							else
								out_string("Not triangle !\n")
							fi
						else
							out_string("Not triangle !\n")
						fi
					else
						out_string("Not triangle !\n")
					fi
	};
};