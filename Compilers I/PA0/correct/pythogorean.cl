class Main inherits IO{
	main() : Object { -- check if triplet is pythogorean
		let h : Int <- in_int() in	
			let p : Int <- in_int() in
				let b : Int <- in_int() in
				if h*h = p*p + b*b then -- simple pythogorus theorem
					out_string("Yes, they are pythogorean triplets !\n")
				else out_string("Oops, they are not pythogorean triplets !\n")
				fi	
	};
};