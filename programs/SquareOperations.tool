object SquareOperations {
	def main() : Unit = {
		if(new SO().isPerfectSquare(100)) {
			println("The square root is " + new SO().squareRoot(100));
		 }
		else {
			println("The number is not a perfect square");
		 }
	}
}
class SO {
	var i : Int;
	 var res : Boolean;
	 var low : Int;
	 var high : Int;
	 var mid : Int;
	 var temp : Int;
	 var temp1 : Int;
	 def isPerfectSquare(num : Int) : Boolean = {
		i = 1;
		 res = false;
		 while(0 < num) {
			num = num - i;
			 i = i + 2;
			 if(num == 0) {
				res = true;
			 }
			if(num < 0) {
				res = false;
			 }
		}
		return res;
	 }
	def squareRoot(num : Int) : Int = {
		low = 0;
		 high = num + 1;
		 while(1 < high - low) {
			temp1 = low + high;
			 mid = temp1 / 2;
			 temp = mid * mid;
			 if(!(num < temp)) {
				low = mid;
			 }
			else {
				high = mid;
			 }
		}
		return low;
	 }
}

