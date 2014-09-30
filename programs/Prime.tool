object Prime {

	def main() : Unit = {
        println(new Primes().init(1000).printPrimes());
    }

}

class Primes {
	
	var primeNumbers : Int[];
	var size : Int;
	
	def init(number : Int) : Int[] = {
        var i : Int;
        var j : Int;
        var potentialPrime : Int;
        var intDiv : Int;
        
        size = number;
        potentialPrime = 2;
        primeNumbers = new Int[number];
        i = 0;
        
        while(i < size){
        	j = 1;
        	while(j < potentialPrime){
        		j = j + 1;
        		if(j == potentialPrime){
        			primeNumbers[i] = potentialPrime;
        			i = i + 1;	
        		}else if((potentialPrime / j) * j == potentialPrime){
        			j = potentialPrime;
        		}
        	}
        	potentialPrime = potentialPrime + 1;
        }
        return this;
    }
    
    def printPrimes() : String = {
    	var result : String;
    	var i : Int;
    	i = 1;
    	
    	result = primeNumbers[0];
    	
    	while(i < size){
    		result = result + ", " + primeNumbers[i];
    	    i = i + 1;
    	}
    	
    	return result;
    }
}
