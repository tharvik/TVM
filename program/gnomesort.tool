object QuickSort {
    def main() : Unit = {
        println(new QS().Start(10));
    }
}

// This class contains the array of integers and
// methods to initialize, print and sort the array
// using GnomeSort
class QS {
    var number : Int[];
    var size : Int;

    // Invoke the Initialization, Sort and Printing
    // Methods
    def Start(sz : Int) : Int = {
        var aux01 : Int;
        aux01 = this.Init(sz);
        aux01 = this.Print();
        println(9999);
        aux01 = size - 1 ;
        aux01 = this.Sort();
        aux01 = this.Print();
        return 9999;
    }

    // Sort array of integers using Quicksort method
    def Sort() : Int = {
        var pos : Int;
        var temp : Int;
        pos = 1;
        while (pos < size) {
            if (number[pos-1] < number[pos] || number[pos] == number[pos-1]) {
                pos = pos + 1;
            } else {
                temp = number[pos];
                number[pos] = number[pos-1];
                number[pos - 1] = temp;
                if (1 < pos) {
                    pos = pos - 1;
                }
            }
        }
        return 1;
    }

    def Print() : Int = {
        var j : Int;

        j = 0 ;
        while (j < (size)) {
            println(number[j]);
            j = j + 1 ;
        }
        return 0 ;
    }

    // Initialize array of integers
    def Init(sz : Int) : Int = {
        size = sz ;
        number = new Int[sz] ;

        number[0] = 20 ;
        number[1] = 7  ; 
        number[2] = 12 ;
        number[3] = 18 ;
        number[4] = 2  ; 
        number[5] = 11 ;
        number[6] = 6  ; 
        number[7] = 9  ; 
        number[8] = 19 ; 
        number[9] = 5  ;

        return 0 ;  
    }
}
