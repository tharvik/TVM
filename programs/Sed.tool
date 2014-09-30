object Sed {

	def main() : Unit = {
		println((new Sed_c()).parse());
	}

}

class Sed_c {

	def parse_1(array: Stack, cmd: Stack) : Stack = {
		var out: Stack;
		var okay: Bool;
		var trigger: Int;
		var replace: Int;

		out = new Stack();
		okay = out.init();

		trigger = 99;
		replace = 99;

		if(cmd.head() == 1) okay = cmd.pop();
		if(okay && cmd.head() == 0) okay = cmd.pop();
		if(okay) {trigger = cmd.head(); okay = cmd.pop();}
		if(okay && cmd.head() == 0) okay = cmd.pop();
		if(okay) {replace = cmd.head(); okay = cmd.pop();}
		if(okay && cmd.head() == 0) okay = cmd.pop();


		if(okay) {

			while(0 < array.size()) {

				if(trigger == array.head())
					okay = out.push(replace);
				else
					okay = out.push(array.head());

				okay = array.pop();
			}
		}

		return out.reverse();
	}

	def parse() : Int = {
		var array: Stack;
		var cmd: Stack;
		var okay: Bool;

		array = new Stack();
		cmd = new Stack();

		okay = array.init();
		okay = cmd.init();

		okay = cmd.push(0);
		okay = cmd.push(5);
		okay = cmd.push(0);
		okay = cmd.push(1);
		okay = cmd.push(0);
		okay = cmd.push(1);
		// 1 0 1 0 3 0 ~= s/1/3/

		okay = array.push(0);
		okay = array.push(0);
		okay = array.push(0);
		okay = array.push(1);
		okay = array.push(2);
		okay = array.push(0);

		okay = array.show();
		if(cmd.head() == 1)
			array = this.parse_1(array, cmd);

		okay = array.show();

		return 0;
	}

}

// copy of Stack.Tool
class Stack {

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

	def reverse(): Stack = {

		var a: Stack;
		var c: Int;
		var okay: Bool;

		a = new Stack();
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
