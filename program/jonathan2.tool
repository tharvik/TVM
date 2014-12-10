/* Compiler construction
 * Jonathan MASUR
 * EL-MA3 
 * October 1st, 2013
 *
 * TOOL lacks floating point support. But who cares, it's not needed, is it ?
 *
 * Non-integer values can easily be handled with fixed point representation
 * This test program is a library that helps usage of fixed point numbers.
 * It's possible to do math operation on them, convert to/from int, and print on the screen
 *
 * The main advantages is that all calculations are exact and that there is no rounding problems
 * as on floating point.
 * The main disadvantage is that the precision is fixed (in our case at 1/64), and that the precision
 * is limited on small numbers.
 */

object jonathan2
{
	def main() : Unit =
	{
		println(new Main().start());
	}
}

class Main
{
	var num1 : Int;
	var num2 : Fixp;
	var num3 : Fixp;

	def start() : String =
	{
		num1 = 3;
		num2 = new Fixp().from_int(4);
		num3 = new Fixp().from_int(5);
		println("num1 = " + num1 + " num2 = " + num2.print() + " num3 = " + num3.print());
		println("");
		println("num2 = num1 + num2 = " + num1         + " + " + num2.print() + " = " + num2.add_int(num1).print());
		println("num3 = num2 + num3 = " + num2.print() + " + " + num3.print() + " = " + num3.add(num2).print());
		println("num3 = num1 / num3 = " + num1         + " / " + num3.print() + " = " + num3.rev_div_int(num1).print());
		println("num2 = num2 / num3 = " + num2.print() + " / " + num3.print() + " = " + num2.div(num3).print());
		println("num3 = num3 / num1 = " + num3.print() + " / " + num1         + " = " + num3.div_int(num1).print());
		println("num2 = num2 * num3 = " + num2.print() + " * " + num3.print() + " = " + num2.mult(num3).print());
		println("num3 = num3 * num1 = " + num3.print() + " * " + num1         + " = " + num3.mult_int(num1).print());
		println("floor(num2) = " + num2.floor());
		println("floor(num3) = " + num3.floor());
		println("ceil(num2) = " + num2.ceil());
		println("ceil(num3) = " + num3.ceil());
		println("int(num2) = " + num2.int());
		println("int(num3) = " + num3.int());
		return "";
	}
}

/* Fixed point helper class */
class Fixp
{
	var data : Int;		// The fixed point value is represented in an INT value, with the lower 6-bits being the fractional part

	// Stupid OOP...
	def data() : Int =
	{
		return data;
	}

	def from_int(int_value : Int) : Fixp =
	{
		data = int_value * 64;
		return this;
	}

	// Return integer equivalent (round low)
	def floor() : Int =
	{
		return data/64;
	}

	// Return integer equivalent (round high)
	def ceil() : Int =
	{
		return (data+63) / 64;
	}

	// Return integer equivalent (round to nearest neighboor)
	def int() : Int =
	{
		return (data+32) / 64;
	}

	/* Print the fixed point number in a string */
	def print() : String =
	{
		var int_part : Int;
		var frac_part : Int;
		var str : String;

		int_part = data / 64;
		frac_part = (data - int_part*64) * 15625;	// 0.015625 = 1/64
		// Enforce an added 0 for values which sould print in the form 0.0x and not 0.x
		if(!(frac_part == 0) && (frac_part < 100000))
			str = "0";
		else
			str = "";

		return int_part + "." + str + frac_part;
	}

	/* Add fixed point with integer */
	def add_int(b : Int) : Fixp =
	{
		data = data + b*64;
		return this;
	}

	/* Add two fixed point values */
	def add(b : Fixp) : Fixp =
	{
		data = data + b.data();
		return this;
	}

	/* Multiply with integer */
	def mult_int(m : Int) : Fixp =
	{
		data = data * m;
		return this;
	}

	/* Multiply two fixed point values */
	def mult(m : Fixp) : Fixp =
	{
		data = (data * m.data())/64;
		return this;
	}

	/* Divide by an integer */
	def div_int(d : Int) : Fixp =
	{
		data = data / d;
		return this;
	}

	/* Divide two fixed point values */
	def div(d : Fixp) : Fixp =
	{
		data = (data*64)/d.data();
		return this;
	}

	/* Divide an integer by a fixed point value */
	def rev_div_int(d : Int) : Fixp =
	{
		data = (d*64*64) / data;
		return this;
	}
}