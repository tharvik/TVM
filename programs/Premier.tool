object Premier {
	def main() : Unit = {
		if(new NPremier().isPremier(4)) {
			println("Est premier");
		}
		else {
			println("N est pas premier");
		}
	}
}
class NPremier {
	def modulo(num : Int, mod : Int) : Int = {
		var rest : Int;
		rest = num;
		while(mod < rest) {
			rest = rest - mod;
		}
		return rest;
	}
	def isPremier(num : Int) : Boolean = {
		var limite : Int;
		var etat : Int;
		var div : Boolean;
		limite = num - 1;
		etat = 2;
		div = true;
		while(( etat < limite) && ! div) {
			if(this.modulo(num, etat) == 0) {
				div = false;
			}
			else {
				etat = etat + 1;
			}
		}
		return div;
	}
}
