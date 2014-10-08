object VampireNumbers {
	def main() : Unit = {
		println(new VN().Start());
	 }
}
class VN {
	var number : Int;
	 var first : Int;
	 var second : Int;
	 var n1 : Int;
	 var n2 : Int;
	 var p : Int [ ];
	 def Start() : Int = {
		var tmp : Boolean;
		 println("");
		 println("This program computes the vampire numbers consisting of 4 digits");
		 println("A vampire number is a number with even digits and is formed by");
		 println("multiplying two numbers which contain half the number of digits");
		 println("of the number it self as well as the digits must be in the original");
		 println("number");
		 println("");
		 println("--------------------------------------------------------------------");
		 println("");
		 println("");
		 number = 1000;
		 p = new Int [ 4 ];
		 n1 = 0;
		 n2 = 0;
		 while(number < 10000) {
			tmp = this.IsVampire();
			 number = number + 1;
		 }
		return 0;
	 }
	def IsVampire() : Boolean = {
		var i : Int;
		 var j : Int;
		 var k : Int;
		 var l : Int;
		 var res : Int;
		 var result : Boolean;
		 i = 0;
		 j = 0;
		 k = 0;
		 l = 0;
		 res = 0;
		 result = false;
		 n1 = number / 100;
		 n2 = this.ModulousCalc(number, 100);
		 p [ 0 ] = n1 / 10;
		 p [ 1 ] = this.ModulousCalc(n1, 10);
		 p [ 2 ] = n2 / 10;
		 p [ 3 ] = this.ModulousCalc(n2, 10);
		 while(i < 4) {
			res = 0;
			 j = 0;
			 while(j < 4) {
				if(!(i == j)) {
					k = 0;
					 while(k < 4) {
						if(!(( k == i) ||(k == j))) {
							l = 0;
							 while(l < 4) {
								if(!(( l == i) ||(l == j) ||(l == k))) {
									first =(p [ i ] * 10) + p [ j ];
									 second =(p [ k ] * 10) + p [ l ];
									 res = first * second;
									 if(res == number) {
										println(first + " * " + second + " = " + number);
										 result = true;
										 l = 4;
										 k = 4;
										 j = 4;
										 i = 4;
									 }
								}
								l = l + 1;
							 }
						}
						k = k + 1;
					 }
				}
				j = j + 1;
			 }
			i = i + 1;
		 }
		return result;
	 }
	def ModulousCalc(num : Int, div : Int) : Int = {
		var temp : Int;
		 var check : Int;
		 check = 0;
		 temp = num;
		 if(temp < div) {
			check = 1;
		 }
		while(check < 1) {
			temp = temp - div;
			 if(temp < div) {
				check = 1;
			 }
		}
		return temp;
	 }
}

