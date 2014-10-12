object RsaTest {
	def main() : Unit = {
		if(new RsaBox().start(33, 7, 3)) {
			println("End of Execution");
		}
	}
}
class RsaBox {
	var m : Int;
	var e : Int;
	var d : Int;
	def start(m_ : Int, e_ : Int, d_ : Int) : Boolean = {
		var cypher : Int;
		var cleartext : Int;
		var i : Int;
		var l : Int [ ];
		l = new Int [ 6 ];
		l [ 0 ] = 6;
		l [ 1 ] = 3;
		l [ 2 ] = 27;
		l [ 3 ] = 31;
		l [ 4 ] = 13;
		l [ 5 ] = 10;
		m = m_;
		e = e_;
		d = d_;
		i = 0;
		while(i < l.length) {
			cypher = this.encryptIterative(l [ i ]);
			cleartext = this.decryptIterative(cypher);
			println("Iterative : Given text : " + l [ i ] + ", Cypher : " + cypher + ", Cleartext : " + cleartext);
			println(l [ i ] == cleartext);
			i = i + 1;
		}
		i = 0;
		while(i < l.length) {
			cypher = this.encryptRecursive(l [ i ]);
			cleartext = this.decryptRecursive(cypher);
			println("Recursive : Given text : " + l [ i ] + ", Cypher : " + cypher + ", Cleartext : " + cleartext);
			println(l [ i ] == cleartext);
			i = i + 1;
		}
		return true;
	}
	def encryptIterative(text : Int) : Int = {
		return this.modpowIterative(text, e);
	}
	def decryptIterative(cypher : Int) : Int = {
		return this.modpowIterative(cypher, d);
	}
	def modpowIterative(t : Int, key : Int) : Int = {
		var ot : Int;
		ot = t;
		while(1 < key) {
			t = t * ot;
			t = this.getRest(t);
			key = key - 1;
		}
		return this.getRest(t);
	}
	def encryptRecursive(text : Int) : Int = {
		return this.modpowRecursive(text, e, text);
	}
	def decryptRecursive(cypher : Int) : Int = {
		return this.modpowRecursive(cypher, d, cypher);
	}
	def modpowRecursive(t : Int, key : Int, ot : Int) : Int = {
		if(1 < key) {
			t = t * ot;
			t = this.getRest(t);
			key = key - 1;
			t = this.modpowRecursive(t, key, ot);
		}
		return this.getRest(t);
	}
	def encryptBruteforce(text : Int) : Int = {
		return this.modpowBruteforce(text, e);
	}
	def decryptBruteforce(cypher : Int) : Int = {
		return this.modpowBruteforce(cypher, d);
	}
	def modpowBruteforce(t : Int, key : Int) : Int = {
		var ot : Int;
		ot = t;
		while(1 < key) {
			t = t * ot;
			key = key - 1;
		}
		return this.getRest(t);
	}
	def getRest(t : Int) : Int = {
		while(m < t) {
			t = t - m;
		}
		return t;
	}
}
