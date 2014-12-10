/* Compiler construction
 * Jonathan MASUR
 * EL-MA3 
 * October 1st, 2013
 *
 * Test program N°1 for the TOOL programming Language
 * This programs sorts an array of size 2^n using the cool merge-sort algorithm I've just seen in the algorithm course
 * This algorithm has the advantage to have a lower complexity than the classical bubble-sort, at the expanse of increased memory usage
 */

object jonathan1
{
	def main() : Unit =
	{
		println(new Main().start());
	}
}

class Main
{
	def start() : String =
	{
		var array : Int[];
		var i : Int;
		var prng : PseudoRandomNumberGenerator;
		var num : Int;
		var SIZE : Int;		// Really  a const...

		SIZE = 64;
		array = new Int[SIZE];
		prng = new PseudoRandomNumberGenerator().init();
		i = SIZE-1;
		while (!(i<0))	// while i>=0
		{
			num = prng.getInt(0, 99);
			array[i] = num;
			i = i-1;
		}

		println("");
		println("Content of unsorted array :");

		// Dummy affectation required by TOOL...
		println(this.printarray(array, SIZE));
		println("");
		println("Content of sorted array :");

		println(this.printarray(this.sort(array, SIZE), SIZE));
		println("");
		println("Finished !");

		// TOOL doesn't support void function so we have to use a dummy return
		return "";
	}

	def printarray(ar : Int[], size : Int) : String =
	{
		var i : Int;
		var print_str : String;

		i = 0;
		print_str = "";
		while(i < size-1)
		{
			print_str = print_str + ar[i] + ", ";
			i = i+1;
		}
		print_str = print_str + ar[i] + ".";
		return print_str;
	}

	// Merge-sort 2 arrays, ar1 and ar2 (should be same size)
	// Returns an array of size 2 * size with numbers sorted if ar1 and ar2 are already sorted
	def merge_sort(ar1 : Int[], ar2 : Int[], size : Int) : Int[] =
	{
		var index1 : Int;
		var index2 : Int;
		var dest_index : Int;
		var dest_array : Int[];

		index1 = 0;
		index2 = 0;
		dest_index = 0;
		dest_array = new Int[2*size];

		while(dest_index < 2*size)
		{	// If the number at the top of array 1 is greater than number at the top of index 2
			if(index1 == size)
			{	// Use number from top of array2, increment index2
				dest_array[dest_index] = ar2[index2];
				index2 = index2 + 1;
			}
			else if((index2 == size) || (ar1[index1] < ar2[index2]))
			{	// Use number from top of array1, increment index1
				dest_array[dest_index] = ar1[index1];
				index1 = index1 + 1;
			}
			else
			{	// Use number from top of array2, increment index2
				dest_array[dest_index] = ar2[index2];
				index2 = index2 + 1;
			}
			dest_index = dest_index + 1;
		}
		return dest_array;
	}

	// Sort an array of size "size" (currently size should be 2, 4, 8, 16, etc...) using the division sort algorithm
	def sort(ar : Int[], size : Int) : Int[] =
	{
		/* If the array is too small to be divided into smaller arrays ... */
		var subar1 : Int[];
		var subar2 : Int[];
		var i : Int;
		var dummy_return_value : Int[];

		if(size == 2)
		{
			if(ar[0] < ar[1])	/* Array is already sorted */
			//	return ar;
				dummy_return_value = ar;
			else
			{					/* Exchange both elements and return sorted array */
				i = ar[1];
				ar[1] = ar[0];
				ar[0] = i;
			//	return ar;
				dummy_return_value = ar;
			}
		}
		else	/* Divide the array into 2 equal parts and sort them */
		{
			i = size/2 - 1;
			subar1 = new Int[size/2];
			subar2 = new Int[size/2];
			while(!(i<0))	// while i>=0
			{
				subar1[i] = ar[i];
				subar2[i] = ar[i + size/2];
				i = i-1;
			}

			/* Sort both sub-arrays, merge them and return the result */
			dummy_return_value = this.merge_sort(this.sort(subar1, size/2), this.sort(subar2, size/2), size/2);
		}

		return dummy_return_value;
	}
}

/* Reused from Maze.tool */
class PseudoRandomNumberGenerator {
  var a : Int;
  var b : Int;

  def init() : PseudoRandomNumberGenerator = {
    a = 12345; // put whatever you like in here
    b = 67890; 
    return this;
  }

  def getInt(min : Int, max : Int) : Int = {
    var posInt : Int;

    posInt = this.nextInt();
    if(posInt < 0)
      posInt = 0 - posInt;

    return min + (this.mod(posInt, max - min));
  }

  def mod(i : Int, j : Int) : Int = { return i - (i / j * j); }

  def nextInt() : Int = {
    b = 36969 * ((b * 65536) / 65536) + (b / 65536);
    a = 18000 * ((a * 65536) / 65536) + (a / 65536);
    return (b * 65536) + a;
  }
}
