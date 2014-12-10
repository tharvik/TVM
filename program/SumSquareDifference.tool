object SumSquareDifference {
    def main() : Unit = {
        if(new SSD().compute(100)) { println("Ok"); } else { println("error"); }
    }
}

class SSD {

	def compute(n: Int) : Bool = {
        var i : Int;
		var sumSquares : Int;
		var squareSum : Int;
		i = 1;
		sumSquares = 0;
		squareSum = 0;
		while (i < n + 1) {
			sumSquares = sumSquares + i * i;
			squareSum = squareSum + i;
			i = i + 1;
		}
		squareSum = squareSum * squareSum;
		
		println("The difference between the sum of the squares of the first " + n + " natural numbers and the square of the sum is : " + (squareSum - sumSquares));
		return true;
	}	
}