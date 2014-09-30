/*Compute the biggest integer smaller than the square root of the number inputed*/
object Sqrt {

    def main(): Unit = {
        /*true number : 46340.94999026239 Our program finds 46340 so OK*/
        println(new Sqrt_c().compute(2147483646));
        if (new Sqrt_c().compute(2147483646) == 46340) {
            println("OK");
        } else {
            println("KO");
        }
    }

}

class Sqrt_c {

    var estimate: Int;
    var counter: Int;
    
    def compute(number: Int): Int = {
        estimate = number;
        counter = 0;
        
        /* 20 iterations should be enough for all integers*/
        while (counter < 20) {
            estimate = (estimate / 2 + number / (estimate * 2));
            counter = counter + 1;
        }
        
        return estimate;
    }
}
