object Bezout {
	def main() : Unit = {
		println(new Util().callAll());
	}
}
class Util {
	def callAll() : Int = {
		var x : Tuple;
		var y : Int;
		println("");
		x = new Util().solveModEq(5, 11, 12);
		println("");
		y = new Util().ReverseMe(8, 15);
		println("");
		x = new Util().BezoutMe(5, 12, 13);
		return 1;
	}
	def mod(m : Int, n : Int) : Int = {
		return m -(n *(m / n));
	}
	def solveModEq(a : Int, b : Int, d : Int) : Tuple = {
		var gcd : Int;
		var res : Tuple;
		println("Solving equation modulo  : ");
		println("---------------------------");
		println("Trying to solve : " + a + "x = " + b + " mod " + d);
		res = new Tuple().init(0, 0);
		gcd = this.gcd(a, d);
		if(this.mod(b, gcd) == 0) {
			res = this.BezoutMe(a, d, b);
			res = res.init(1, res.first());
			println("Solution is : " + a + "*" + res.second() + " = " + b + " mod " + d);
		}
		else {
			println("Error, provide equation doesn&#x27;t have a solution !");
			res = res.init(0, 0);
		}
		println("---------------------------");
		return res;
	}
	def ReverseMe(a : Int, m : Int) : Int = {
		var gcd : Int;
		var res : Int;
		println("Trying to find the inverse of " + a + " modulo " + m + " :");
		println("------------------------------------------------------");
		gcd = this.gcd(a, m);
		if(gcd == 1) {
			res = this.modInverse(a, m);
			println(a + + m + " = " + res);
		}
		else {
			res = 0;
			println("gcd(" + a + ", " + m + + gcd + " inverse doesn&#x27;t not exist !");
		}
		println("------------------------------------------------------");
		return res;
	}
	def modInverse(x : Int, m : Int) : Int = {
		var temp : Int;
		temp = this.EuclideAlgorithm(x, m).first();
		temp = temp + m;
		return this.mod(temp, m);
	}
	def BezoutMe(a : Int, b : Int, d : Int) : Tuple = {
		var res : Tuple;
		var gcd : Int;
		var k : Int;
		k = 0;
		println("----------------------------------------------");
		println("Trying to solve " + a + "*u  " + b + "*v = " + d);
		res = new Tuple().init(0, 0);
		gcd = this.gcd(a, b);
		if(this.mod(d, gcd) == 0) {
			res = this.EuclideAlgorithm(a, b);
			k = d / gcd;
			res = res.init(res.first() * d, res.second() * d);
			println("We have " + a + "*" + res.first() + "  " + b + "*" + res.second() + " = " + d);
			println("----------------------------------------------");
		}
		else {
			println("Error ! d is not the gcd of a and b");
		}
		return res;
	}
	def EuclideAlgorithm(a_ : Int, b_ : Int) : Tuple = {
		var a : Int;
		var b : Int;
		var u : Int;
		var v : Int;
		var q : Int;
		var r : Int;
		var s : Int;
		var t : Int;
		var temp : Int;
		u = 1;
		v = 0;
		s = 0;
		t = 1;
		a = a_;
		b = b_;
		while(!(b < 0) && !(b == 0)) {
			q = a / b;
			r = this.mod(a, b);
			a = b;
			b = r;
			temp = s;
			s = u -(q * s);
			u = temp;
			temp = t;
			t = v -(q * t);
			v = temp;
		}
		return new Tuple().init(u, v);
	}
	def gcd(m_ : Int, n_ : Int) : Int = {
		var t : Int;
		var r : Int;
		var result : Int;
		var m : Int;
		var n : Int;
		m = this.abs(m_);
		n = this.abs(n_);
		if(m < n) {
			t = m;
			m = n;
			n = t;
		}
		r = this.mod(m, n);
		if(r == 0) {
			result = n;
		}
		else {
			result = this.gcd(n, r);
		}
		return result;
	}
	def abs(v : Int) : Int = {
		var res : Int;
		if(!(v < 0)) {
			res = v;
		}
		else {
			res = 0 - v;
		}
		return res;
	}
}
class Tuple {
	var x : Int;
	var y : Int;
	def init(a : Int, b : Int) : Tuple = {
		x = a;
		y = b;
		return this;
	}
	def first() : Int = {
		return x;
	}
	def second() : Int = {
		return y;
	}
	def display() : Int = {
		println("x : " + x + " y : " + y);
		return 0;
	}
}
