object PalindromeNumber {
	def main() : Unit = {
		println(new Pal().Start(5));
	 }
}
class Pal {
	var phrase : Int [ ];
	 var inverse : Int [ ];
	 var tmp1 : Int;
	 def Start(num : Int) : Int = {
		tmp1 = this.InitA(num);
		 println("");
		 println("nombre original");
		 tmp1 = this.tabRead(phrase);
		 println("");
		 println("nombre inversé");
		 tmp1 = this.doReverse(phrase, num);
		 tmp1 = this.tabRead(inverse);
		 println("");
		 tmp1 = this.checkPal(phrase, inverse);
		 println("");
		 return 0;
	 }
	def InitA(size : Int) : Int = {
		phrase = new Int [ size ];
		 phrase [ 0 ] = 1;
		 phrase [ 1 ] = 2;
		 phrase [ 2 ] = 3;
		 phrase [ 3 ] = 4;
		 phrase [ 4 ] = 9;
		 return 0;
	 }
	def doReverse(tab : Int [ ], size : Int) : Int = {
		var count : Int;
		 var countEnd : Int;
		 inverse = new Int [ size ];
		 count = 0;
		 countEnd = tab.length - 1;
		 while(count < tab.length) {
			inverse [ count ] = tab [ countEnd ];
			 count = count + 1;
			 countEnd = countEnd - 1;
		 }
		return 0;
	 }
	def tabRead(tab : Int [ ]) : Int = {
		var count : Int;
		 count = 0;
		 while(count < tab.length) {
			println(tab [ count ]);
			 count = count + 1;
		 }
		return 0;
	 }
	def checkPal(tab1 : Int [ ], tab2 : Int [ ]) : Int = {
		var count : Int;
		 var sumTest : Int;
		 count = 0;
		 sumTest = 0;
		 while(count < tab1.length) {
			if(!(tab1 [ count ] == tab2 [ count ])) {
				sumTest = sumTest + 1;
			 }
			count = count + 1;
		 }
		if(sumTest == 0) {
			println("Ce nombre est un palindrome !");
			 println("");
		 }
		else {
			println("Ce nombre ne fait pas un palindrome !");
			 println("");
		 }
		return 0;
	 }
}

