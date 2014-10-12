object Main {
	def main() : Unit = {
		if(new MyObject().init()) {
			println("Finish");
		}
	}
}
class MyArray {
	var array : Int [ ];
	var size : Int;
	def init(firstElement : Int) : MyArray = {
		array = new Int [ 1 ];
		size = 1;
		array [ 0 ] = firstElement;
		return this;
	}
	def add(element : Int) : MyArray = {
		var tmp : Int [ ];
		var i : Int;
		tmp = array;
		array = new Int [ size + 1 ];
		i = 0;
		while(i < size) {
			array [ i ] = tmp [ i ];
			i = i + 1;
		}
		array [ size ] = element;
		size = size + 1;
		return this;
	}
	def print() : String = {
		var str : String;
		var i : Int;
		i = 0;
		str = "array : ";
		while(i < size) {
			println(i);
			str = str + array [ i ];
			if(i < size - 1) {
				str = str + ", ";
			}
			i = i + 1;
		}
		return str;
	}
}
class MyObject {
	def init() : Boolean = {
		println(new MyArray().init(42).add(3).print());
		return true;
	}
}
