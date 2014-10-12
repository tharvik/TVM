object CalculPGCD {
	def main() : Unit = {
		println("PGCD : " + new PGCD().Start(555, 666));
	}
}
class PGCD {
	var numberA : Int;
	var numberB : Int;
	def Start(a : Int, b : Int) : Int = {
		var aux : Int;
		numberA = a;
		numberB = b;
		aux = this.Print();
		aux = this.Calcul();
		return aux;
	}
	def Print() : Int = {
		println("Premier numero: " + numberA + ".");
		println("Deuxieme numero: " + numberB + ".");
		return 42;
	}
	def Calcul() : Int = {
		var aux1 : Int;
		var aux2 : Int;
		var reste : Int;
		var resultat : Int;
		var pgcd : Int;
		if(numberA < numberB) {
			aux1 = numberB;
			aux2 = numberA;
		}
		else {
			aux1 = numberA;
			aux2 = numberB;
		}
		while(0 < aux2) {
			resultat = aux1 / aux2;
			reste = aux1 -(aux2 * resultat);
			aux1 = aux2;
			aux2 = reste;
		}
		pgcd = aux1;
		return pgcd;
	}
}
