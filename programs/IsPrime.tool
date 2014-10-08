object IsPrime {
	def main() : Unit = {
		println(new Prime().start(120));
	 }
}
class Prime {
	def start(nbr : Int) : Int = {
		if(nbr == 0) {
			println("The number is NOT prime.");
		 }
		if(nbr == 1) {
			println("The number may be prime.");
		 }
		else {
			if(( this.testPrime(nbr)) == 1) {
				println("The number may be prime.");
			 }
			else {
				println("The number is NOT prime.");
			 }
		}
		return 0;
	 }
	def testPrime(nbr : Int) : Int = {
		var count : Int;
		 var res : Int;
		 res = 1;
		 count = 2;
		 while(count < nbr + 1) {
			if(this.canDivise(nbr, count) == 1) {
				res = 0;
			 }
			count = count + 1;
		 }
		return res;
	 }
	def canDivise(nbr : Int, count : Int) : Int = {
		var res : Int;
		 var tst : Int;
		 tst = 2;
		 res = 0;
		 while(tst < nbr) {
			if(( tst * count) == nbr) {
				res = 1;
			 }
			tst = tst + 1;
		 }
		return res;
	 }
}

