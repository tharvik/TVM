object jonathan1 {
	def main() : Unit = {
		println(new Main().start());
	}
}
class Main {
	def start() : String = {
		var array : Int [ ];
		var i : Int;
		var prng : PseudoRandomNumberGenerator;
		var num : Int;
		var SIZE : Int;
		SIZE = 64;
		array = new Int [ SIZE ];
		prng = new PseudoRandomNumberGenerator().init();
		i = SIZE - 1;
		while(!(i < 0)) {
			num = prng.getInt(0, 99);
			array [ i ] = num;
			i = i - 1;
		}
		println("");
		println("Content of unsorted array :");
		println(this.printarray(array, SIZE));
		println("");
		println("Content of sorted array :");
		println(this.printarray(this.sort(array, SIZE), SIZE));
		println("");
		println("Finished !");
		return "";
	}
	def printarray(ar : Int [ ], size : Int) : String = {
		var i : Int;
		var print_str : String;
		i = 0;
		print_str = "";
		while(i < size - 1) {
			print_str = print_str + ar [ i ] + ", ";
			i = i + 1;
		}
		print_str = print_str + ar [ i ] + ".";
		return print_str;
	}
	def merge_sort(ar1 : Int [ ], ar2 : Int [ ], size : Int) : Int [ ] = {
		var index1 : Int;
		var index2 : Int;
		var dest_index : Int;
		var dest_array : Int [ ];
		index1 = 0;
		index2 = 0;
		dest_index = 0;
		dest_array = new Int [ 2 * size ];
		while(dest_index < 2 * size) {
			if(index1 == size) {
				dest_array [ dest_index ] = ar2 [ index2 ];
				index2 = index2 + 1;
			}
			else if(( index2 == size) ||(ar1 [ index1 ] < ar2 [ index2 ])) {
				dest_array [ dest_index ] = ar1 [ index1 ];
				index1 = index1 + 1;
			}
			else {
				dest_array [ dest_index ] = ar2 [ index2 ];
				index2 = index2 + 1;
			}
			dest_index = dest_index + 1;
		}
		return dest_array;
	}
	def sort(ar : Int [ ], size : Int) : Int [ ] = {
		var subar1 : Int [ ];
		var subar2 : Int [ ];
		var i : Int;
		var dummy_return_value : Int [ ];
		if(size == 2) {
			if(ar [ 0 ] < ar [ 1 ]) dummy_return_value = ar;
			else {
				i = ar [ 1 ];
				ar [ 1 ] = ar [ 0 ];
				ar [ 0 ] = i;
				dummy_return_value = ar;
			}
		}
		else {
			i = size / 2 - 1;
			subar1 = new Int [ size / 2 ];
			subar2 = new Int [ size / 2 ];
			while(!(i < 0)) {
				subar1 [ i ] = ar [ i ];
				subar2 [ i ] = ar [ i + size / 2 ];
				i = i - 1;
			}
			dummy_return_value = this.merge_sort(this.sort(subar1, size / 2), this.sort(subar2, size / 2), size / 2);
		}
		return dummy_return_value;
	}
}
class PseudoRandomNumberGenerator {
	var a : Int;
	var b : Int;
	def init() : PseudoRandomNumberGenerator = {
		a = 12345;
		b = 67890;
		return this;
	}
	def getInt(min : Int, max : Int) : Int = {
		var posInt : Int;
		posInt = this.nextInt();
		if(posInt < 0) posInt = 0 - posInt;
		return min +(this.mod(posInt, max - min));
	}
	def mod(i : Int, j : Int) : Int = {
		return i -(i / j * j);
	}
	def nextInt() : Int = {
		b = 36969 *(( b * 65536) / 65536) +(b / 65536);
		a = 18000 *(( a * 65536) / 65536) +(a / 65536);
		return(b * 65536) + a;
	}
}
