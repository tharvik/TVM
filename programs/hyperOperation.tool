object hyperOperation {
	def main() : Unit = {
		{
			println();
			println("n = 0: " + new H().HypOp(0, 3, 2));
			println("n = 1: " + new H().HypOp(1, 3, 2));
			println("n = 2: " + new H().HypOp(2, 3, 2));
			println("n = 3: " + new H().HypOp(3, 3, 2));
			println("n = 4: " + new H().HypOp(4, 3, 2));
			println("n = 5: StackOverflow? :__D");
			println();
			println("n = 0: " + new H().HypOp(0, 3, 0));
			println("n = 1: " + new H().HypOp(1, 3, 0));
			println("n = 2: " + new H().HypOp(2, 3, 0));
			println("n = 3: " + new H().HypOp(3, 3, 0));
			println();
			println("n = 0: " + new H().HypOp(0, 0, 4));
			println("n = 1: " + new H().HypOp(1, 0, 4));
			println("n = 2: " + new H().HypOp(2, 0, 4));
			println("n = 3: " + new H().HypOp(3, 0, 4));
			println();
			println("n = 0: " + new H().HypOp(0, 5, 4));
			println("n = 1: " + new H().HypOp(1, 5, 4));
			println("n = 2: " + new H().HypOp(2, 5, 4));
			println("n = 3: " + new H().HypOp(3, 5, 4));
		}
	}
}
class H {
	def HypOp(n : Int, a : Int, b : Int) : Int = {
		var nextNb : Int;
		if(n == 0) {
			nextNb = b + 1;
		}
		else if(n == 1 && b == 0) {
			nextNb = a;
		}
		else if(n == 2 && b == 0) {
			nextNb = 0;
		}
		else if(b == 0) {
			nextNb = 1;
		}
		else {
			nextNb = this.HypOp(n - 1, a, this.HypOp(n, a, b - 1));
		}
		return nextNb;
	}
}
