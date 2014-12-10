object Test {
    def main() : Unit = {
        if(new TestBattery().test()) { println("Ok"); } else { println("error"); }
    }
}

class A {
	def mySuperFunction(i: Int) : Bool = {
		println(i + 10);
		return true;
	}
	
	/* overload test
	def mySuperFunction(i: Int, i2: Int) : Bool = {
		return true;
	} */
}

class B {
	/* override test*/
	def mySuperFunction(i: Int) : Bool = {
		println(i + 100);
		return false;
	}
} 



class TestBattery {
	
	def test() : Bool = {
		var i : Bool;
		var t : Bool;
		i = this.sementic();
		if (!i) println("sementic error");
		t = this.print();
		t = this.and();
		t = this.or();
		t = this.override();
		
		//this is a comment
		
		return i;
		
		/* this is
			another comment */
		
	}
	
	def sementic() : Bool = {
		println(("foo" + 3) + " " + "foo3");
		println((3 + "foo") + " " + "3foo");
		println((3 + 3) + " " + 6);
		return (42 == 42) && !(new A() == new A()) && !("f" + "oo" == "fo" + "o");
	}
	
	def print() : Bool = {
		println(1);
		println(true);
		println("hello");
		return true;
	}
	
	def printFoo(): Bool = {
	  println("foo");
	  return false;
	}

	def printBar(): Bool = {
	  println("bar");
	  return true;
	}
	
	def and() : Bool = {
		println("and test : ");
		return this.printFoo() && this.printBar();
	}
	
	def or() : Bool = {
		println("or test : ");
		return this.printBar() || this.printFoo();
	}
	
	def override() : Bool = {
		var a: A;
		var b: B;
		a = new A();
		b = new B();
		
		println("override test : ");
		
		return a.mySuperFunction(1) && b.mySuperFunction(1); 
	}
}