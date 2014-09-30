/*Compute the biggest integer smaller than the square root of the number inputed*/
object Prime {

	def main() : Unit = {
		/*true number : 46340.94999026239 Our program finds 464340 so OK*/
        var result : Int;
        result = new Sqrt().compute(2147483646); 
        println(result);
        if(result = 464340){
        	println("Ok");
        } else {
        	println("Ko");
        }
    }

}

class Sqrt {
	
	var estimate : Int;
	var counter : Int;
	def compute(number : Int) : Int = {
        estimate = number;
        counter = 0;
        /* 20 iterations should be enough for all integers*/
        while(counter < 20){
        	estimate = (estimate/2 + number/(estimate*2));
        	counter = counter + 1;
        }
        return estimate;
    }
}
