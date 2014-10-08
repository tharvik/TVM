object Euclid {
	def main() : Unit = {
		println(new EuclidPGDC().compute(25, 555));
	 }
}
class EuclidPGDC {
	def compute(a : Int, b : Int) : Int = {
		var r : Int;
		 println("PGDC of " + a + " and " + b + " : ");
		 while(!(b == 0)) {
			r = this.modulo(a, b);
			 a = b;
			 b = r;
		 }
		return a;
	 }
	def modulo(n : Int, mod : Int) : Int = {
		return(n -(( n / mod) * mod));
	 }
}

