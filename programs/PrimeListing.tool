object PrimeListing {
	def main() : Unit = {
		if(!(new PrimeGenerator().next(200, 20))) {
			println("End !");
		 }
	}
}
class PrimeGenerator {
	def next(nl : Int, nb : Int) : Boolean = {
		var half : Int;
		 var ret : Boolean;
		 half = nl / 2;
		 if(!(half * 2 == nl) AND(22:31)AND(22:31) this.test(nl, half, 3)) {
			println(nl);
			 nb = nb - 1;
		 }
		if(0 < nb) {
			ret = this.next(nl + 1, nb);
		 }
		return true;
	 }
	def test(nl : Int, half : Int, start : Int) : Boolean = {
		var ret : Boolean;
		 ret = true;
		 if(( nl / start) * start == nl) {
			ret = false;
		 }
		else {
			if(start + 2 < half) {
				ret = this.test(nl, half, start + 2);
			 }
		}
		return ret;
	 }
}

