/*
 * This program encodes/decodes messages with a key
 * using the ceasar cipher
 *
 * For more information: http://en.wikipedia.org/wiki/Caesar_cipher
 */

object ceasarCipher {
	def main(): Unit = {
		if(new ShowProgram().run()) {
			println("");
			println("Program complete..");
		}
	}
}

class ShowProgram {
	def run(): Bool = {
		var t: Text;
		var t2: CeasarCipher;
		var key: Int;
		
		/*
		 * Create the text t to encrypt with the key
		 */
		key = 0-42424242;
		t = new Text().init(25)
			.add(new Alphabet().f())
			.add(new Alphabet().r())
			.add(new Alphabet().e())
			.add(new Alphabet().d())
			.add(new Alphabet().SPACE())
			.add(new Alphabet().a())
			.add(new Alphabet().n())
			.add(new Alphabet().d())
			.add(new Alphabet().SPACE())
			.add(new Alphabet().f())
			.add(new Alphabet().l())
			.add(new Alphabet().o())
			.add(new Alphabet().r())
			.add(new Alphabet().i())
			.add(new Alphabet().a())
			.add(new Alphabet().n());
			
		println("The text to encrypt is: ");
		t = t.printText();
		println("");
			
		println("Pssst, the key is: "+key);
		println("");
		
		println("Let's encode using CeasarCipher(key, text)! ");
		t2 = new CeasarCipher().loadFromText(t).encode(key);
		t = t2.printText();
		println("");
		
		println("Let's decode now!");
		t2 = t2.decode(key);
		t = t2.printText();
		
		return true;
	}
}

class CeasarCipher extends Text {
	
	/*
	 * We construct CeasarCipher from a Text!
	 */
	def loadFromText(textToLoad: Text): CeasarCipher = {
		t = textToLoad.getIntArray();
		nextPos = textToLoad.getNextPos();
		
		return this;
	}
	
	def encode(key: Int): CeasarCipher = {
		var i: Int;
		var modKey: Int;
		var alphLength: Int;
		
		alphLength = new Alphabet().getLength();
		modKey = new Math().mod(key, alphLength);
		
		i = 0;
		while(i < nextPos) {
			/*
			 * With an Alphabet of length 27:
			 * 23 -> 26
			 * 24 -> 27
			 * 25 -> 1
			 * 26 -> 2
			 */
			t[i] = new Math().mod(t[i]+modKey-1+alphLength, alphLength) + 1;
			
			i = i+1;
		}
		
		return this;
	}
	
	def decode(key: Int): CeasarCipher = {
		return this.encode(0-key);
	}
}

class Text {
	var t: Int[];
	var nextPos: Int;
	
	/*
	 * maxSize: Text max length
	 */
	def init(maxSize: Int): Text = {
		t = new Int[maxSize];
		nextPos = 0;
		return this;
	}

	/*
	 * Add a char to the text (Use Alphabet Class for char <=> Int conversion)
	 */
	def add(c: Int): Text = {
		if(nextPos < t.length) {
			t[nextPos] = c;
			nextPos = nextPos + 1;
		}
		return this;
	}
	
	def printText(): Text = {
		var finalText: String;
		var i: Int;
		
		finalText = "";
		i = 0;
		
		while(i < nextPos) {
			finalText = finalText + new Alphabet().getChar(t[i]);
			i = i+1;
		}
		
		println(finalText);
		
		return this;
	}
	
	/*
	 * Getters / Setters
	 */
	def getIntArray(): Int[] = {
		return t;
	}
	def setIntArray(newT: Int[]): Text = {
		t = newT;
		return this;
	}
	def getNextPos(): Int = {
		return nextPos;
	}
	def setNextPos(newNextPos: Int): Text = {
		nextPos = newNextPos;
		return this;
	}
}

/*
 * This is the alphabet digitalizer.
 */
class Alphabet {
	def getLength(): Int = { return 27;}

	def a(): Int = { return 1;}
	def b(): Int = { return 2;}
	def c(): Int = { return 3;}
	def d(): Int = { return 4;}
	def e(): Int = { return 5;}
	def f(): Int = { return 6;}
	def g(): Int = { return 7;}
	def h(): Int = { return 8;}
	def i(): Int = { return 9;}
	def j(): Int = { return 10;}
	def k(): Int = { return 11;}
	def l(): Int = { return 12;}
	def m(): Int = { return 13;}
	def n(): Int = { return 14;}
	def o(): Int = { return 15;}
	def p(): Int = { return 16;}
	def q(): Int = { return 17;}
	def r(): Int = { return 18;}
	def s(): Int = { return 19;}
	def t(): Int = { return 20;}
	def u(): Int = { return 21;}
	def v(): Int = { return 22;}
	def w(): Int = { return 23;}
	def x(): Int = { return 24;}
	def y(): Int = { return 25;}
	def z(): Int = { return 26;}
	
	def SPACE(): Int = { return 27;}
	
	def getChar(c: Int): String = {
		var textChar: String;
		textChar = "?";
		
		if(c == 1) { textChar = "a"; }
		else if(c == 2) { textChar = "b"; }
		else if(c == 3) { textChar = "c"; }
		else if(c == 4) { textChar = "d"; }
		else if(c == 5) { textChar = "e"; }
		else if(c == 6) { textChar = "f"; }
		else if(c == 7) { textChar = "g"; }
		else if(c == 8) { textChar = "h"; }
		else if(c == 9) { textChar = "i"; }
		else if(c == 10) { textChar = "j"; }
		else if(c == 11) { textChar = "k"; }
		else if(c == 12) { textChar = "l"; }
		else if(c == 13) { textChar = "m"; }
		else if(c == 14) { textChar = "n"; }
		else if(c == 15) { textChar = "o"; }
		else if(c == 16) { textChar = "p"; }
		else if(c == 17) { textChar = "q"; }
		else if(c == 18) { textChar = "r"; }
		else if(c == 19) { textChar = "s"; }
		else if(c == 20) { textChar = "t"; }
		else if(c == 21) { textChar = "u"; }
		else if(c == 22) { textChar = "v"; }
		else if(c == 23) { textChar = "w"; }
		else if(c == 24) { textChar = "x"; }
		else if(c == 25) { textChar = "y"; }
		else if(c == 26) { textChar = "z"; }
		
		else if(c == 27) { textChar = " "; }
		
		return textChar;
	}
}

/*
 * Enable more advanced mathematical operations
 */
class Math {
	/*
	 * Computes the modulo of a regarding m: a mod m
	 */
	def mod(a: Int, m: Int): Int = {
		return a-(a/m)*m;
	}
}

