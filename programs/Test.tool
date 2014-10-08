object Test {
	def main() : Unit = {
		if(new TestBattery().test()) {
			println("Ok");
		 }
		else {
			println("error");
		 }
	}
}
class A {
	def mySuperFunction(i : Int) : Boolean = {
		println(i + 10);
		 return true;
	 }
}
class B {
	def mySuperFunction(i : Int) : Boolean = {
		println(i + 100);
		 return false;
	 }
}
class TestBattery {
	def test() : Boolean = {
		var i : Boolean;
		 var t : Boolean;
		 i = this.sementic();
		 if(! i) println("sementic error");
		 t = this.print();
		 t = this.and();
		 t = this.or();
		 t = this.override();
		 return i;
	 }
	def sementic() : Boolean = {
		println(( "foo" + 3) + " " + "foo3");
		 println(( 3 + "foo") + " " + "3foo");
		 println(( 3 + 3) + " " + 6);
		 return(42 == 42) AND(54:27)AND(54:27) !(new A() == new A()) AND(54:52)AND(54:52) !("f" + "oo" == "fo" + "o");
	 }
	def print() : Boolean = {
		println(1);
		 println(true);
		 println("hello");
		 return true;
	 }
	def printFoo() : Boolean = {
		println("foo");
		 return false;
	 }
	def printBar() : Boolean = {
		println("bar");
		 return true;
	 }
	def and() : Boolean = {
		println("and test : ");
		 return this.printFoo() AND(76:32)AND(76:32) this.printBar();
	 }
	def or() : Boolean = {
		println("or test : ");
		 return this.printBar() || this.printFoo();
	 }
	def override() : Boolean = {
		var a : A;
		 var b : B;
		 a = new A();
		 b = new B();
		 println("override test : ");
		 return a.mySuperFunction(1) AND(92:37)AND(92:37) b.mySuperFunction(1);
	 }
}

