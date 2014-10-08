object QuickSort {
	def main() : Unit = {
		println(new QS().Start(10));
	 }
}
class QS {
	var number : Int [ ];
	 var size : Int;
	 def Start(sz : Int) : Int = {
		var aux01 : Int;
		 aux01 = this.Init(sz);
		 aux01 = this.Print();
		 println(9999);
		 aux01 = size - 1;
		 aux01 = this.Sort(0, aux01);
		 aux01 = this.Print();
		 return 9999;
	 }
	def Sort(left : Int, right : Int) : Int = {
		var v : Int;
		 var i : Int;
		 var j : Int;
		 var nt : Int;
		 var t : Int;
		 var cont01 : Boolean;
		 var cont02 : Boolean;
		 var aux03 : Int;
		 t = 0;
		 if(left < right) {
			v = number [ right ];
			 i = left - 1;
			 j = right;
			 cont01 = true;
			 while(cont01) {
				cont02 = true;
				 while(cont02) {
					i = i + 1;
					 aux03 = number [ i ];
					 if(!(aux03 < v)) cont02 = false;
					 else cont02 = true;
				 }
				cont02 = true;
				 while(cont02) {
					j = j - 1;
					 aux03 = number [ j ];
					 if(!(v < aux03)) cont02 = false;
					 else cont02 = true;
				 }
				t = number [ i ];
				 number [ i ] = number [ j ];
				 number [ j ] = t;
				 if(j <(i + 1)) cont01 = false;
				 else cont01 = true;
			 }
			number [ j ] = number [ i ];
			 number [ i ] = number [ right ];
			 number [ right ] = t;
			 nt = this.Sort(left, i - 1);
			 nt = this.Sort(i + 1, right);
		 }
		else nt = 0;
		 return 0;
	 }
	def Print() : Int = {
		var j : Int;
		 j = 0;
		 while(j <(size)) {
			println(number [ j ]);
			 j = j + 1;
		 }
		return 0;
	 }
	def Init(sz : Int) : Int = {
		size = sz;
		 number = new Int [ sz ];
		 number [ 0 ] = 20;
		 number [ 1 ] = 7;
		 number [ 2 ] = 12;
		 number [ 3 ] = 18;
		 number [ 4 ] = 2;
		 number [ 5 ] = 11;
		 number [ 6 ] = 6;
		 number [ 7 ] = 9;
		 number [ 8 ] = 19;
		 number [ 9 ] = 5;
		 return 0;
	 }
}

