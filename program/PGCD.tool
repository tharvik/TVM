object PGCD {
	def main() : Unit = {
		if(new Euclide().compute(3821732, 8098320)){
			println("Ok");
		}else{
			println("Pas ok");
		}
	}
}

class Euclide {
	def compute(nbr1 : Int, nbr2 : Int) : Bool = {
		var maxNbr : Int;
		var minNbr : Int;
		var r1 : Int;
		
		if(nbr1 < nbr2){
			maxNbr = nbr2;
			minNbr = nbr1;
		}else{
			maxNbr = nbr1;
			minNbr = nbr2;
		}
		
		while(0 < minNbr){
			r1 = 1;
			
			while(((minNbr * r1) < maxNbr) || ((minNbr * r1) == maxNbr)){
				r1 = r1 + 1;
			}
			
			r1 = r1 - 1;
			
			r1 = maxNbr - (minNbr * r1);
			maxNbr = minNbr;
			minNbr = r1;
			
		}
		
		if(minNbr == 0){
			println("PGCD (" + nbr1 + ", " + nbr2 + "): " +  maxNbr);
		}else{
			println("Negative value not allowed (" + nbr1 + ", " + nbr2 + ")");
		}
		
		return true;
	}
}