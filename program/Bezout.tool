/*
In this file we provide methods to compute the Bezout Identity 
solving the equation au + bv = gcd(a,b)*k where u and v are unknown.
We also provide a method to compute the modulo inverse. 
Both of these calculus are made possible by the use of the Euclidian 
extended algorithm. 
Finally we provide a method to solve equations of the type ax = b mod d
where x is the unknown variable. Again, for this purpose we use the Euclidian
Algorithm.
*/

object Bezout {
	def main () : Unit = {
		
        println( new Util().callAll());
		//println("This programm solves Bezout identity : au + bv = gcd(a,b)*k !");
		//println(new Util().solveModEq(5,11,12).display());
		//println(new Util().ReverseMe(8, 15));
		
	}

   
}

class Util {

    /*Calls all the interresting methods with some values
    */
     def callAll() : Int = {
        var x : Tuple;
        var y : Int ;
        
        println("");
        x = new Util().solveModEq(5,11,12);
        
        println("");
        y = new Util().ReverseMe(8, 15);

        println("");
        x = new Util().BezoutMe(5,12,13);

        return 1;   
    }
	/* Method Computing the modulo of two numbers
	*/
	 def mod(m : Int, n : Int) : Int = {
        return m - (n * (m / n));
    }

    /* Solves ax = b mod d when solution exists. The condition
        for existance is that gcd(a, d) divides b.
    */
    def solveModEq(a : Int, b : Int, d : Int) : Tuple = {
        var gcd : Int;
        var res : Tuple ; 

        /* For display purposes
        */
        println("Solving equation modulo  : ");
        println("---------------------------");

        println("Trying to solve : "+a+"x = "+b+" mod "+d);
        /* first value is validity of res, i.e. 0 means
            the second value is not the result (an error occurred)
            1 means that the second value is the answer needed
        */
        res = new Tuple().init(0,0); 
        gcd = this.gcd(a, d);

        if(this.mod(b, gcd) == 0){
            res = this.BezoutMe(a, d, b);
            res = res.init(1, res.first());
            println("Solution is : "+a+"*"+res.second()+" = "+b+" mod "+d);
        }else{
            println("Error, provide equation doesn't have a solution !");
            res = res.init(0,0);
        }
        println("---------------------------");
        return res;
    }
    /* Method computing the inverse of a modulo m 
        if possible (gcd(a,m) = 1), returns default value 
        otherwise.
    */
    def ReverseMe(a : Int , m : Int ) : Int = {
    	var gcd : Int ; 
    	var res : Int ;

        /* Handle the display
        */
        println("Trying to find the inverse of "+a+" modulo "+m+" :");
        println("------------------------------------------------------");


    	gcd = this.gcd(a,m);
    	if(gcd == 1) {
    		//they are relatively prime we compute the inverse
    		res = this.modInverse(a, m);
            println(a+"^(-1) mod "+m+" = "+res);
    	}else {
    		//They are not relatively prime so error code !
    		res = 0;
            println("gcd("+a+", "+m+")="+gcd+" inverse doesn't not exist !");
    	}
        println("------------------------------------------------------");
    	return res;
    }

    /* Method Computing the modulo inverse of x modulo m,
        that is, if y is the inver of x mod m, yx ≃ 1 mod m.
        To compute this, we use the euclidian algo trying to solve
        xu + mv = 1 = gcd(x,m) and we take u + m (u ≃ u+ m mod m, just
        easier for display cause usually u is negative)
    */
    def modInverse (x: Int, m : Int ) : Int = {
    	var temp : Int ;
    	temp = this.EuclideAlgorithm(x,m).first();
    	temp = temp + m ;
    	return this.mod(temp,m);
    }

    /* Method to solve the bezout equation au + bv = d. 
        d has to be gcd (a,b). The euclidian algorithm actually
        works if gcd(a,b) divides d and so we implemented the more
        general result : au + bv = gcd(a,b)*k, k a natural number != 0
    */
    def BezoutMe(a : Int, b : Int, d : Int ) : Tuple = {
    	var res : Tuple ;
    	var gcd : Int ;
        var k : Int ;
        k = 0;

        println("----------------------------------------------");
        println("Trying to solve "+a+"*u + "+b+"*v = "+d);
    	res = new Tuple().init(0,0);
    	gcd = this.gcd(a,b);

    	if (this.mod(d, gcd)== 0){
    		res =  this.EuclideAlgorithm(a, b);
            k = d / gcd ;
            res = res.init(res.first() * d, res.second()* d);
            println("We have "+a+"*"+res.first()+" + "+b+"*"+res.second()+" = "+d);
            println("----------------------------------------------");
    	} else {
    		println("Error ! d is not the gcd of a and b");
    	}
    	return res ;
    	
    }

    /* Method using the Euclidian algorithm to compute 
    	the solutions for the given equation
    */
    def EuclideAlgorithm (a_ : Int, b_ : Int) : Tuple = {
    	var a : Int ;
    	var b : Int ;
    	var u : Int ;
    	var v : Int ; 
    	var q : Int ;
    	var r : Int ;
    	var s : Int ;
    	var t : Int ;
    	var temp : Int ;

    	u = 1 ; 
    	v = 0;
    	s = 0 ;
    	t = 1;
    	a = a_;
    	b = b_ ;

    	while ( ! (b < 0) && !(b == 0) ) {
    		q = a / b;
    		r = this.mod (a, b);
    		a = b ;
    		b = r;
    		temp = s;
    		s = u - (q *s);
    		u = temp;
    		temp = t;
    		t = v - (q*t);
    		v = temp;
    	}

    	return new Tuple().init(u, v);
    } 

    /* Compute the gcd of two positive integers
    	(taken from the examples)
    */
    def gcd(m_ : Int, n_ : Int) : Int = {
        var t : Int;
        var r : Int;
        var result : Int;
        var m : Int;
        var n : Int;
 
        m = this.abs(m_);
        n = this.abs(n_);
 
        if (m < n) {
            t = m;
            m = n;
            n = t;
        }
 
        r = this.mod(m,n); // m % n;
 
        if (r == 0) {
            result = n;
        } else {
            result = this.gcd(n, r);
        }
        return result;
    }

     def abs(v : Int) : Int = {
        var res : Int;
 
        if(!(v < 0)) {
            res = v;
        } else {
            res = 0 - v;
        }
        return res;
    }
}

class Tuple {
	var x : Int ; 
	var y : Int ; 

	def init (a : Int, b : Int) : Tuple = {
		x = a ;
		y = b;
		return this ;
	}
	def first() : Int = {
		return x;
	}
	def second () : Int = {
		return y;
	}
	def display () : Int = {
		println ("x : "+x+" y : "+y);
		return 0;
	}
}
