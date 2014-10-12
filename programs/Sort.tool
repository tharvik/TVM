object Sort {
	def main() : Unit = {
		println(new S().init(10).printValues());
	}
}
class S {
	var number : Int [ ];
	var size : Int;
	def init(s : Int) : S = {
		var i : Int;
		var k : Int;
		size = s;
		number = new Int [ s ];
		i = 0;
		k = 342;
		while(i < s) {
			number [ i ] = k;
			k = this.nextInt(k);
			i = i + 1;
		}
		return this;
	}
	def nextInt(i : Int) : Int = {
		var r : Int;
		if(i - 2 *(i / 2) == 0) {
			r = i / 2;
		}
		else {
			r = 3 * i + 1;
		}
		return r;
	}
	def printValues() : Int = {
		var i : Int;
		i = 0;
		println("----------------------------");
		while(i < size) {
			println("Value[" + i + "]" + " = " + number [ i ]);
			i = i + 1;
		}
		println("----------------------------");
		return 0;
	}
}
