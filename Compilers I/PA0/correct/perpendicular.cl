class Main inherits IO{ -- Check if two vectors are perpendicular
	main() : Object { 
		let x1 : Int <- in_int() in	 -- take x coordinate of first vector
			let y1 : Int <- in_int() in -- take y coordinate of first vector
				let x2 : Int <- in_int() in  -- take x coordinate of second vector
					let y2 : Int <- in_int() in -- take x coordinate of second vector
						if x1*x2 + y1*y2 = 0 then -- find dot product
							out_string("Yes, they are perpendicular vectors !\n")
						else 
							out_string("Oops, they are not perpendicular vectors !\n")
						fi	
	};
};