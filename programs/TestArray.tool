object TestArray {
	def main() : Unit = {
		println(new Tst().start(3));
	 }
}
class Tst {
	var tab : Int [ ];
	 def start(nbr : Int) : Int = {
		tab = new Int [ nbr ];
		 tab [ 0 ] = 0;
		 tab [ 1 ] = 1;
		 tab [ 2 ] = 2;
		 return this.tabRead(tab);
	 }
	def tabRead(tab : Int [ ]) : Int = {
		var count : Int;
		 count = 0;
		 while(count < tab.length) {
			println(tab [ count ]);
			 count = count + 1;
		 }
		println("FIN");
		 return 0;
	 }
}

