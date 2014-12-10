object Pascal {
	def main() : Unit = {
		if(new Triangle().compute(20)){ //temps devient long à partir de 20
			println("Ok");
		}else{
			println("pas OK");
		}
	}
}

class Triangle {
	def compute(maxHt : Int) : Bool = {
		var i : Int;
		var j : Int;
		var k : Int;
		var util : Util;
		var line : String;
		
		println("Recursive way");
		println("********************************");
		
		i = 0;
		j = 0;
		
		while(i < maxHt){
			k = 0;
			line = "";
			while(k < (maxHt-i)){
				k = k+1;
				line = line + " ";
			}
			while((j < i) || (i == j)){
				line = line + this.getComb(j, i) + " ";
				
				j = j + 1;
			}
			println(line);
			if(i < j){
				j = 0;
			}
			i = i + 1;
		}
		
		println("");
		println("Newton way");
		println("********************************");
		
		i = 0;
		j = 0;
		util = new Util();
		
		while((i < maxHt)){
			k = 0;
			line = "";
			while(k < (maxHt-i)){
				k = k+1;
				line = line + " ";
			}
			
			while((j < i) || (i == j)){
				line = line + (util.fact(i)/(util.fact(j)*util.fact(i-j))) + " ";
				j = j + 1;
			}
			println(line);
			if(i < j){
				j = 0;
			}
			i = i + 1;
		}
		
		return true;
	}
	
	def getComb(line : Int, row : Int) : Int = {
		var res : Int;
		if((line == 0) || (line == row)) {
			res = 1;
		} else {
			res = this.getComb(line, row-1) + this.getComb(line-1, row-1);
		}
		return res;
	}
}

class Util {
	def fact(nbr : Int) : Int = {
		var res : Int;
		
		if(nbr == 0){
			res = 1;
		}else{
			res = nbr * this.fact(nbr - 1);
		}
		return res;
		
	}
}