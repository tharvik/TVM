/*
A quine is a program that writes itself
Limitations: 
-Double-quote are replaced by single-quote
-No break line
*/
object Quine {
	def main(): Unit = {
		println((new Q()).Start());
	}
}

class Q {
	var b: String;
	var c: String;
	var d: String;
	def Start(): String = {
		c = "''";
		d = ";return b+c+b+c+d;}}";
		b = "/*A quine is a program that writes itselfLimitations: -Double-quote are replaced by single-quote-No break line*/object Quine {def main(): Unit = {println((new Q()).Start());}}class Q {var b: String;var c: String;var d: String;def Start(): String = {c = '''''';d = '';return b+c+b+c+d;}}''b = ";
		return b+c+b+c+d;
	}
}