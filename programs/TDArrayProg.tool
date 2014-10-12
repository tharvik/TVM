object TDArrayProg {
	def main() : Unit = {
		println(new TDArray().test());
	}
}
class TDArray {
	var col : Int;
	var row : Int;
	var mem : Int [ ];
	def init(row_ : Int, col_ : Int) : TDArray = {
		col = col_;
		row = row_;
		mem = new Int [ row * col ];
		return this;
	}
	def put(row_ : Int, col_ : Int, elem : Int) : Int = {
		mem [ col * row_ + col_ ] = elem;
		return 0;
	}
	def get(row_ : Int, col_ : Int) : Int = {
		return mem [ col * row_ + col_ ];
	}
	def printRowCol() : String = {
		println("Row : " + row);
		return "Col : " + col;
	}
	def test() : String = {
		var arr : TDArray;
		var i : Int;
		println("Testing TDArray class :");
		println("Initialise with 10 rows and 5 columns :");
		arr = new TDArray().init(10, 5);
		println(arr.printRowCol());
		i = arr.put(7, 2, 3);
		println("Put element 3 at row 7 column 2.");
		println("Getting element at row 7 column 2 : " + arr.get(7, 2));
		return "Test ended";
	}
}
