object VectorImpl {
	def main() : Unit = {
		if(new TestVector().test()) {
			println("Ok");
		 }
		else {
			println("error");
		 }
	}
}
class Vector {
	var array : Int [ ];
	 var pos : Int;
	 def construct() : Int = {
		pos = 0;
		 array = new Int [ 2 ];
		 return 0;
	 }
	def resize(n : Int) : Int = {
		var oldArray : Int [ ];
		 var i : Int;
		 oldArray = array;
		 i = 0;
		 array = new Int [ n ];
		 while(( i < oldArray.length) AND(24:38)AND(24:38)(i < n)) {
			array [ i ] = oldArray [ i ];
			 i = i + 1;
		 }
		return 0;
	 }
	def at(index : Int) : Int = {
		return array [ index ];
	 }
	def set(index : Int, value : Int) : Int = {
		array [ index ] = value;
		 return value;
	 }
	def push_back(value : Int) : Int = {
		var oldPos : Int;
		 var lol : Int;
		 if(array.length <(pos + 1)) {
			oldPos = pos;
			 lol = this.resize(array.length * 2);
			 pos = oldPos;
		 }
		lol = this.set(pos, value);
		 pos = pos + 1;
		 return pos;
	 }
}
class TestVector {
	def test() : Boolean = {
		var v : Vector;
		 var i : Int;
		 i = 0;
		 v = new Vector();
		 i = v.construct();
		 i = 0;
		 while(i < 10) {
			i = v.push_back(i);
		 }
		i = 0;
		 while(i < 10) {
			println(v.at(i));
			 i = i + 1;
		 }
		return true;
	 }
}

