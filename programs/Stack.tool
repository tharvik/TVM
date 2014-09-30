object Stack {
	def main() : Unit = {
		if((new Stack_c()).go()) println("No errors");
		else println("Errors");
	}
}

class Stack_c {

	var array: Int[];
	var count: Int;
	var max: Int;

	def push(a: Int): Bool = {
		var ret: Bool;
		ret = false;
		if(count < max) {
			ret = true;

			array[count] = a;
			count = count + 1;
		}

		return ret;
	}

	def head(): Int = {
		return array[count - 1];
	}

	def pop(): Bool = {
		var ret: Bool;
		ret = false;
		if(0 < count) {
			ret = true;
			count = count - 1;
		}

		return ret;
	}

	def show(): Bool = {

		var i: Int;
		var str: String;

		if(count == 0)
			str = "(empty)";
		else {
			str = "[" + array[0];
			i = 1;
			while(i < count) {
				str = str + "," + array[i];
				i = i + 1;
			}
			str = str + "]";
		}

		println(str);

		return true;
	}

	def init(): Bool = {
		max = 20;
		array = new Int[max];
		count = 0;

		return true;
	}

	def go(): Bool = {
		var okay: Bool;

		okay = this.init();

		okay = this.push(0);
		okay = this.push(1);
		okay = this.push(2);
		okay = this.push(4);
		if(okay) okay = this.pop();
		if(okay) okay = this.show();

		return okay;
	}

	def set(arr: Int[]): Bool = {
		array = arr;
		return true;
	}

	def get(): Int[] = {
		return array;
	}

	def size(): Int = {
		return count;
	}

	def reverse(): Stack_c = {

		var a: Stack_c;
		var c: Int;
		var okay: Bool;

		a = new Stack_c();
		okay = a.init();
		c = count;

		while(0 < c) {
			okay = a.push(this.head());
			okay = this.pop();
			c = c - 1;
		}

		return a;
	}
}
