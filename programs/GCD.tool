object GCD {
	def main() : Unit = {
		println(new PGCD().computeGCD(7, 13));
	 }
}
class PGCD {
	def computeGCD(num1 : Int, num2 : Int) : Int = {
		var num_aux : Int;
		 var res : Int;
		 if(num1 < num2) res = this.computeGCD(num2, num1);
		 else {
			if(num2 == 0) res = num1;
			 else {
				num_aux = num1 -(( num1 / num2) * num2);
				 res = this.computeGCD(num2, num_aux);
			 }
		}
		return res;
	 }
}

