object test {
	def main() : Unit = {
		println(new MyObject().Start(10));
	 }
}
class MyObject {
	var size : Int;
	 def Start(value : Int) : Int = {
		println(value);
		 size = 3;
		 return this.Next();
	 }
	def Next() : Int = {
		println(size);
		 return 0;
	 }
}

