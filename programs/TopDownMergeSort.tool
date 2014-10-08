object TopDownMergeSort {
	def main() : Unit = {
		println(new MergeSort().Start(10, 0, 9));
	 }
}
class MergeSort {
	var numbers : Int [ ];
	 var helper : Int [ ];
	 var size : Int;
	 var aux01 : Int;
	 def Start(a : Int, low : Int, high : Int) : Int = {
		aux01 = this.Init(a);
		 aux01 = this.Print();
		 println(9999);
		 aux01 = this.SplitMerge(low, high);
		 aux01 = this.Print();
		 return 9999;
	 }
	def SplitMerge(low : Int, high : Int) : Int = {
		var middle : Int;
		 if(low < high) {
			middle = low +(( high - low) / 2);
			 aux01 = this.SplitMerge(low, middle);
			 aux01 = this.SplitMerge(middle + 1, high);
			 aux01 = this.Merge(low, middle, high);
		 }
		return 0;
	 }
	def Merge(low : Int, middle : Int, high : Int) : Int = {
		var i0 : Int;
		 var i1 : Int;
		 var i2 : Int;
		 var k : Int;
		 k = low;
		 while(k < high + 1) {
			helper [ k ] = numbers [ k ];
			 k = k + 1;
		 }
		i0 = low;
		 i1 = middle + 1;
		 i2 = low;
		 while(i0 < middle + 1 AND(50:32)AND(50:32) i1 < high + 1) {
			if(helper [ i0 ] < helper [ i1 ] + 1) {
				numbers [ i2 ] = helper [ i0 ];
				 i0 = i0 + 1;
			 }
			else {
				numbers [ i2 ] = helper [ i1 ];
				 i1 = i1 + 1;
			 }
			i2 = i2 + 1;
		 }
		while(i0 < middle + 1) {
			numbers [ i2 ] = helper [ i0 ];
			 i2 = i2 + 1;
			 i0 = i0 + 1;
		 }
		return 0;
	 }
	def Print() : Int = {
		var j : Int;
		 j = 0;
		 while(j <(size)) {
			println(numbers [ j ]);
			 j = j + 1;
		 }
		return 0;
	 }
	def Init(sz : Int) : Int = {
		size = sz;
		 numbers = new Int [ sz ];
		 helper = new Int [ sz ];
		 numbers [ 0 ] = 20;
		 numbers [ 1 ] = 7;
		 numbers [ 2 ] = 12;
		 numbers [ 3 ] = 18;
		 numbers [ 4 ] = 2;
		 numbers [ 5 ] = 11;
		 numbers [ 6 ] = 6;
		 numbers [ 7 ] = 9;
		 numbers [ 8 ] = 19;
		 numbers [ 9 ] = 5;
		 return 0;
	 }
}

