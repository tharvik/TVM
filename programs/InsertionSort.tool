object InsertionSort {
	def main() : Unit = {
		println(new ISort().start());
	 }
}
class ISort {
	var tab : Int [ ];
	 var i : Int;
	 var unused : Int;
	 var valueToInsert : Int;
	 var holePos : Int;
	 def start() : Int = {
		tab = new Int [ 5 ];
		 tab [ 0 ] = 3;
		 tab [ 1 ] = 1;
		 tab [ 2 ] = 4;
		 tab [ 3 ] = 5;
		 tab [ 4 ] = 0;
		 i = 0;
		 while(i < tab.length) {
			println(tab [ i ]);
			 i = i + 1;
		 }
		println("-----------------");
		 unused = this.sort();
		 i = 0;
		 while(i < tab.length) {
			println(tab [ i ]);
			 i = i + 1;
		 }
		return 0;
	 }
	def sort() : Int = {
		i = 1;
		 while(i < tab.length) {
			valueToInsert = tab [ i ];
			 holePos = i;
			 while(0 < holePos AND(57:31)AND(57:31) valueToInsert < tab [ holePos - 1 ]) {
				tab [ holePos ] = tab [ holePos - 1 ];
				 holePos = holePos - 1;
			 }
			tab [ holePos ] = valueToInsert;
			 i = i + 1;
		 }
		return 0;
	 }
}

