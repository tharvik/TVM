object Recursive {
	def main() : Unit = {
		println(new Func().run(new Func().run(new Func().run(20))));
	 }
}
class Func {
	def run(s : Int) : Int = {
		var i : Int;
		 if(0 < s) {
			i = new Func().run(( s - 1));
		 }
		else {
			i = 0;
		 }
		return i;
	 }
}

