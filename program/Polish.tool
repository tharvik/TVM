object Polish{
	def main(): Unit = {
		println(new Expr().getNew().printAll());
	}	
}

/* Encoding :
 
	1 --> +
	2 --> -
	3 --> * 
	4 --> /
	
*/

class Expr{
	var top: Tree;

	def getNew(): Expr = {
		var left : Tree;
		var right : Tree;
		
		left = new Leaf().init(4);
		
		right = new Leaf().init(5);
		
		top = new Fork().init(left, right, 1 /* plus */);
		
		left = top;
		
		right = new Leaf().init(2);
		
		top = new Fork().init(left, right, 3 /* times */);
		
		right = new Leaf().init(34);
		
		left = new Leaf().init(1367);
		
		top = new Fork().init(new Fork().init(left, right, 4 /* divide */), top, 2 /* minus */);
		
		return this;
	}
	
	def printAll(): String = {
		println("Prefix : " + top.prePrint());
		println("Postfix : " + top.postPrint());
		println("Infix : " + top.inPrint());
		println("Result : " + top.eval());
		return "* End *";
	}

}

class Tree{
	
	def eval(): Int = {
		println("Error : called eval on abstract class");
		return 0;
	}
	
	
	def decode(op: Int): String = {
		var res: String;
		if(op == 1){
			res = "+";
		}else if(op == 2){
			res = "-";
		}else if(op == 3){
			res = "*";
		}else if(op == 4){
			res = "/";
		}else{
			println("Invalid code for operation");
			res = "error";
		}
		
		return res;
	}

	def prePrint(): String = {
		return "Default prePrint";
	}
	
	def postPrint(): String = {
		return "Default postPrint";
	}
	
	def inPrint(): String = {
		return "Default inPrint";
	}
}

class Fork extends Tree{
	var l: Tree;
	var r: Tree;
	var val: Int;
	
	def init(left: Tree, right: Tree, value: Int): Fork = {
		l = left;
		r = right;
		val = value;
		return this;
	}
	
	def eval(): Int = {
		var res: Int;
		if(val == 1){
			res = l.eval() + r.eval();
		}else if(val == 2){
			res = l.eval() - r.eval();
		}else if(val == 3){
			res = l.eval() * r.eval();
		}else if(val == 4){
			res = l.eval() / r.eval();
		}else{
			println("Illegal Operation");
			res = 0;
		}
		
		println("Eval Fork : " + res);
		
		return res;
	}
	
	def prePrint(): String = {
		return this.decode(val) + " " + l.prePrint() + " " + r.prePrint();
	}
	
	def postPrint(): String = {
		return l.postPrint() + " " + r.postPrint() + " " + this.decode(val);
	}
	
	def inPrint(): String = {
		return "(" + l.inPrint() + " " + this.decode(val) + " " + r.inPrint() + ")";
	}
}

class Leaf extends Tree{
	var val: Int;
	
	def init(value : Int): Leaf = {
		val = value;
		return this;
	}
	
	def eval(): Int = {
		println("Eval Leaf : " + val);
		return val;
	}
	
	def prePrint(): String = {
		return "" + val;
	}
	
	def postPrint(): String = {
		return "" + val;
	}
	
	def inPrint(): String = {
		return "" + val;
	}
		
}