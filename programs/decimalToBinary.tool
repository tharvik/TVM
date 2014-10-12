object decimalToBinary {
	def main() : Unit = {
		{
			println("Testing with -3 : " + new Converter().decToBin(0 - 3));
			println("Testing with 0 : " + new Converter().decToBin(0));
			println("Testing with 1 : " + new Converter().decToBin(1));
			println("Testing with 15 : " + new Converter().decToBin(15));
			println("Testing with 64 : " + new Converter().decToBin(64));
			println("Testing with 9999 : " + new Converter().decToBin(9999));
			println("Testing with 12938432 : " + new Converter().decToBin(12938432));
		}
	}
}
class Converter {
	def decToBin(dec : Int) : String = {
		var decimal : Int;
		var res : Int;
		var binary : String;
		var powersOfTwo : Int;
		var powerResult : Int;
		var numToSpace : Int;
		decimal = dec;
		binary = "";
		numToSpace = 4;
		if(decimal < 0) {
			binary = "-1";
		}
		else {
			if(decimal < 2) {
				binary = binary + decimal;
			}
			else {
				powersOfTwo = this.findGreatestPowerOfTwo(decimal);
				binary = this.addBeginningZeros(powersOfTwo + 1);
				println("powersOfTwo	decimal	powerResult	binary");
				while(0 - 1 < powersOfTwo) {
					powerResult = this.pow(2, powersOfTwo);
					println(powersOfTwo + "	" + decimal + "	" + powerResult + "	" + binary);
					if(powerResult < decimal || powerResult == decimal) {
						binary = binary + "1";
						if(0 < decimal) {
							decimal = decimal - this.pow(2, powersOfTwo);
						}
					}
					else {
						binary = binary + "0";
					}
					res = powersOfTwo / numToSpace;
					if(powersOfTwo == res * numToSpace) {
						binary = binary + " ";
					}
					powersOfTwo = powersOfTwo - 1;
				}
			}
		}
		return binary;
	}
	def findGreatestPowerOfTwo(decimal : Int) : Int = {
		var power : Int;
		power = 1;
		while(this.pow(2, power) < decimal) {
			power = power + 1;
		}
		if(( decimal == this.pow(2, power)) == false) {
			power = power - 1;
		}
		return power;
	}
	def pow(num : Int, power : Int) : Int = {
		var result : Int;
		result = num;
		if(power == 0) {
			result = 1;
		}
		while(1 < power) {
			result = result * num;
			power = power - 1;
		}
		return result;
	}
	def addBeginningZeros(numBits : Int) : String = {
		var result : String;
		var count : Int;
		result = "";
		count = numBits -(numBits / 4) * 4;
		if(0 < count) {
			count = 4 - count;
		}
		while(0 < count) {
			result = result + 0;
			count = count - 1;
		}
		return result;
	}
}
