object ArmstrongNumber {
    def main() : Unit = {
        println(new AN().isArmstrong(371));        
    }
}


class AN {
      
      def isArmstrong(num : Int) : Bool = {
	
        var numOd: Int;
        var temp1: Int;
        var digits: Int[];
		var i: Int;
        var temp2: Int;
        var Sum: Int;
        var tempNum: Int;
		var j: Int;
        var tempProd: Int;
		var ret: Bool;

       ret = false;
       i = 0;
       numOd = 0; 
       temp1 = num;
       Sum = 0;

       while(0 < temp1){
	
          temp1 = temp1 / 10;
          numOd = numOd + 1;
       }  
	digits= new Int[numOd];
	temp1 = num;
	temp2 = numOd;        

	while(i < temp2){
	  
	  digits[i] = this.mod(temp1,10);
	  temp1 = temp1 / 10;
	  i = i + 1;
	}
        
	temp2 = numOd;
	i = numOd;
	j = 0;
	tempProd = 1;	

	while(j < temp2) {
	      tempNum = digits[j];
          tempProd = 1;
	      i = numOd;

		while(0 < i){
			tempProd = tempProd * tempNum;
			i = i -1;

		}
        
		Sum = Sum + tempProd;		
		j = j + 1;

	
	}	

        if( Sum == num)
			ret = true;

		return ret;


		
     


      }

      def mod(m : Int, n : Int) : Int = {
        return m - (n * (m / n));
    }

}
