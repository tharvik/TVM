object Test {
	def main() : Unit = {
		println(new Util().showAll());
	}
}

class Random {

	/*	@brief : Initial seed 
	*/
	var initSeed : Int;
	/*	@brief : Current "seed"
	*/
	var seed : Int ;

	/*	@brief : Initiate the Seed 
		Undefined behavior if not called prior to use this object.
		We enforce seed to be positive. 
		@param : s : Int
		@return : Int (the seed)
	*/
	def init (s : Int ) : Random  = {
		seed = new Util().abs(s);
		initSeed = seed;
		return this ;
	}

	/*	@brief : Computes the next random number according to the seed.
		We use the formula next = previous *a  mod m. According to Lewis,
		Goodman and Miller, a = 16807 and m = 2147483647 which have been 
		validated as a minimum requirement. 
		@return : Int (next seed)
	*/
	def nextIntLCG () : Int = {
		var k : Int ;
		var rnd_seed : Int ;
	
		rnd_seed = seed ;
		k = rnd_seed / 127773 ;
		rnd_seed = 16807 * (rnd_seed - k * 127773) - k * 2836;
		if(rnd_seed < 0){
			rnd_seed = rnd_seed + 2147483647;
		}
		seed = rnd_seed ;
		return seed ;
	}	

	/* 	@brief : Returns the next Int between 0 and n
		n must be smaller than 2147483647 and we enforce it to be
		positive for convinience.
		@param : n : Int 
		@return : Int
	*/
	def nextInt(n : Int) : Int = {
		var res : Int ;
		var absn : Int;
		absn = new Util().abs(n);
		if(!(absn < 2147483647) && !(absn == 2147483647)){
			println("Error : n too big !") ;
			res =0-1; 
		}else{
			res = this.nextIntLCG();
			res = new Util().mod(res, n);

		}
		return res;
	}

	/*	@brief : Generates a random boolean. 
		@ return : Bool
	*/
	def nextBoolean() : Bool = {
		var res : Bool ;
		var x : Int ;
		x = this.nextInt(2);
		if(x == 1){
			res = true;
		}else{
			res = false;
		}
		return res;
	}

	/*	@brief : Computes a Stream of n random Ints
		if n > 100, we set n to 100 (arbitrary limit).
		@param : n : Int
		@return : Int[] 
	*/
	def streamLCG(n : Int) : Arr = {
		var size : Int;
		var res : Int[];
		var i  : Int ;
		size = new Util().abs(n);
		
		if(!(n < 100) && !(n == 100)){
			size = 100;
		}
		res = new Int[size];
		i = 0;
		while (i < size){
			res[i] = this.nextIntLCG();
			i = i + 1;
		}

		return new Arr().constr(res, size);
	}

	/* @brief : Generates a stream of n random integers < m. 
	   m has to be positive and smaller than or equal to 2147483647.
	   n has to be positive (or enforced otherwise) and smaller than
	   or equal to 100 (otherwise enforced). 
	   @param : n : Int
	   @param : m 
	   @return : Int[]
	*/
	def streamOfInt(n : Int, m : Int) : Arr = {
		var modul : Int ;
		var res : Arr;
		var tmp : Int[];
		var i : Int;
		modul = new Util().abs(m);

		
		if(!(modul < 2147483647) && !(modul == 2147483647)){
			modul = 2147483647;
		}
		res = this.streamLCG(n);
		tmp = res.getArr();
		i = 0;

		while (i < res.getSize()){
			tmp[i] = new Util().mod(tmp[i], modul);
			i = i + 1;
		}
		res = res.setArr(tmp);

		return res;
	}

}

class Util {
	
	/* 	@brief : Computes m mod n. 
		@param : m : Int , n : Int 
		@return : Int 
	*/
	def mod (m : Int, n : Int) : Int = {
		return m -(n* (m/n));
	}

	/*	@brief : Computes the absolute value of m
		@param : m : Int 
		@return : Int
	*/
	def abs (m : Int ) : Int =  {
		var res : Int ;
		
		if(!(m < 0)) {
			res = m;
		} else {
			res = 0-m ;
		}
		return res ;
	}
	/* Used for display
	*/
	def showAll() : Int = {
		println("");
		println("This program generates pseudo random values.");
		println("Using the original seed and the Lewis Goodman Miller formula.");
		println("It enables you to : take a random number, take a random boolean,");
		println("Take a stream of pseudo random numbers and a stream of random numbers");
		println("inferior to a given value. Here we show a stream of rand less than 50");
		println(new Random().init(6).streamOfInt(99, 50).arrToString());
		println("");
		return 1;
	}

}

/* @brief : class to represent an array and its size
*/
class Arr {
	var arr : Int [];
	var size : Int ; 

	/*	@brief : Constructor for the array. Values are in a
		size is in s. We simply assure that s is positive !
		@param : a : Int[]
		@param : s : Int
	*/
	def constr(a : Int[] , s : Int ) : Arr = {
		arr = a;
		size = new Util().abs(s);
		return this ;
	}

	/*	@brief : puts the array to a String. Values are separated by ;
		MUST be initialized !
		@return : String
	*/
	def arrToString() : String = {
		var res : String;
		var i : Int ;
		res = "";
		i = 0;
		while(i < size){
			res = res + arr[i] +"; ";
			i = i + 1;
		}
		return res;
	}

	/*	@brief : getter for arr
		@return : Int[]
	*/
	def getArr() : Int[] = {
		return arr;
	}
	/*	@brief : getter for size
		@return : Int
	*/
	def getSize() : Int = {
		return size;
	}

	def setArr(a : Int[]) : Arr = {
		arr = a ;
		return this;
	}
}