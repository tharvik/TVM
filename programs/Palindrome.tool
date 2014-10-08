object Palindrome {
	def main() : Unit = {
		println(new Pal().isPalindromic(33311333, 8));
	 }
}
class Pal {
	def isPalindromic(number : Int, palinLen : Int) : Int = {
		var numberArray : Int [ ];
		 var i : Int;
		 var returnVal : Int;
		 numberArray = new Int [ palinLen ];
		 i = 0;
		 while(i < palinLen) {
			numberArray [ i ] = number -(number / 10) * 10;
			 number = number / 10;
			 i = i + 1;
		 }
		i = 0;
		 returnVal = 1;
		 while(i < palinLen) {
			if(numberArray [ i ] == numberArray [ palinLen - 1 - i ]) i = i;
			 else returnVal = 0;
			 i = i + 1;
		 }
		return returnVal;
	 }
}

