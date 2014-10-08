object jonathan2 {
	def main() : Unit = {
		println(new Main().start());
	 }
}
class Main {
	var num1 : Int;
	 var num2 : Fixp;
	 var num3 : Fixp;
	 def start() : String = {
		num1 = 3;
		 num2 = new Fixp().from_int(4);
		 num3 = new Fixp().from_int(5);
		 println("num1 = " + num1 + " num2 = " + num2.print() + " num3 = " + num3.print());
		 println("");
		 println("num2 = num1 + num2 = " + num1 + " + " + num2.print() + " = " + num2.add_int(num1).print());
		 println("num3 = num2 + num3 = " + num2.print() + " + " + num3.print() + " = " + num3.add(num2).print());
		 println("num3 = num1 / num3 = " + num1 + " / " + num3.print() + " = " + num3.rev_div_int(num1).print());
		 println("num2 = num2 / num3 = " + num2.print() + " / " + num3.print() + " = " + num2.div(num3).print());
		 println("num3 = num3 / num1 = " + num3.print() + " / " + num1 + " = " + num3.div_int(num1).print());
		 println("num2 = num2 * num3 = " + num2.print() + " * " + num3.print() + " = " + num2.mult(num3).print());
		 println("num3 = num3 * num1 = " + num3.print() + " * " + num1 + " = " + num3.mult_int(num1).print());
		 println(STR(floor(num2) =)(46:17) + num2.floor());
		 println(STR(floor(num3) =)(47:17) + num3.floor());
		 println(STR(ceil(num2) =)(48:17) + num2.ceil());
		 println(STR(ceil(num3) =)(49:17) + num3.ceil());
		 println(STR(int(num2) =)(50:17) + num2.int());
		 println(STR(int(num3) =)(51:17) + num3.int());
		 return "";
	 }
}
class Fixp {
	var data : Int;
	 def data() : Int = {
		return data;
	 }
	def from_int(int_value : Int) : Fixp = {
		data = int_value * 64;
		 return this;
	 }
	def floor() : Int = {
		return data / 64;
	 }
	def ceil() : Int = {
		return(data + 63) / 64;
	 }
	def int() : Int = {
		return(data + 32) / 64;
	 }
	def print() : String = {
		var int_part : Int;
		 var frac_part : Int;
		 var str : String;
		 int_part = data / 64;
		 frac_part =(data - int_part * 64) * 15625;
		 if(!(frac_part == 0) AND(101:30)AND(101:30)(frac_part < 100000)) str = "0";
		 else str = "";
		 return int_part + "." + str + frac_part;
	 }
	def add_int(b : Int) : Fixp = {
		data = data + b * 64;
		 return this;
	 }
	def add(b : Fixp) : Fixp = {
		data = data + b.data();
		 return this;
	 }
	def mult_int(m : Int) : Fixp = {
		data = data * m;
		 return this;
	 }
	def mult(m : Fixp) : Fixp = {
		data =(data * m.data()) / 64;
		 return this;
	 }
	def div_int(d : Int) : Fixp = {
		data = data / d;
		 return this;
	 }
	def div(d : Fixp) : Fixp = {
		data =(data * 64) / d.data();
		 return this;
	 }
	def rev_div_int(d : Int) : Fixp = {
		data =(d * 64 * 64) / data;
		 return this;
	 }
}

