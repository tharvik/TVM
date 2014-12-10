object InsertionSort {
    def main() : Unit = {
        println(new ISort().start());      // we give here the number's size to be test   
    }
}

class ISort {
	var tab : Int[];
	var i: Int;
	var unused: Int;
	var valueToInsert: Int;
	var holePos: Int;


	def start() : Int = {

		tab = new Int[5];
		tab[0] = 3;
		tab[1] = 1;
		tab[2] = 4;
		tab[3] = 5;
		tab[4] = 0;

		i = 0;
		
		while(i < tab.length) {
			println(tab[i]);
			i = i + 1;
		}

		println("-----------------");

		unused = this.sort();

		i = 0;
		
		while(i < tab.length) {
			println(tab[i]);
			i = i + 1;
		}

		

		return 0;

	}

	def sort() : Int = {

		i = 1;

		while(i < tab.length) {

			valueToInsert = tab[i];
			holePos = i;

			while(0 < holePos && valueToInsert < tab[holePos - 1]) {

				tab[holePos] = tab[holePos - 1];
				holePos = holePos - 1;

			}

			tab[holePos] = valueToInsert;
			i = i + 1;

		}

		return 0;

	}


// The values in A[i] are checked in-order, starting at the second one
/*for i ← 1 to i ← length(A)
  {
    // at the start of the iteration, A[0..i-1] are in sorted order
    // this iteration will insert A[i] into that sorted order
    // save A[i], the value that will be inserted into the array on this iteration
    valueToInsert ← A[i]
    // now mark position i as the hole; A[i]=A[holePos] is now empty
    holePos ← i
    // keep moving the hole down until the valueToInsert is larger than 
    // what's just below the hole or the hole has reached the beginning of the array
    while holePos > 0 and valueToInsert < A[holePos - 1]
      { //value to insert doesn't belong where the hole currently is, so shift 
        A[holePos] ← A[holePos - 1] //shift the larger value up
        holePos ← holePos - 1       //move the hole position down
      }
    // hole is in the right position, so put valueToInsert into the hole
    A[holePos] ← valueToInsert
    // A[0..i] are now in sorted order
  }
*/


}
