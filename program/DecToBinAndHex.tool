object Pi {
    def main() : Unit = {
        println(new Test().Print(42));
    }
}

class Test {
  var binaryRes: Int[];
  var number: Int;
  var universalAnswer: Int;
  
  def Print(nbr: Int): String = {
    number = nbr;
    universalAnswer = this.ToBinary();
    return "Done!";
  }
  
  def ToBinary(): Int = {
    var powerOfTwo: Int;
    var i: Int;
    var j: Int;
    var remainder: Int;
    var binaryResInverse: Int[];
    
    universalAnswer = 42;
    powerOfTwo = 1;
    i = 0;
    j = 0;
    remainder = number;
    
    while(powerOfTwo < number) {
      powerOfTwo = powerOfTwo*2;
      j = j+1;
    }
    
    binaryResInverse = new Int[j];
    binaryRes = new Int[j];
    
    while(0 < remainder) {
      powerOfTwo = powerOfTwo/2;
      if(powerOfTwo<remainder || powerOfTwo == remainder){
    	  binaryResInverse[i] = 1;
    	  remainder = remainder - powerOfTwo;
      }
      else{
    	  binaryResInverse[i] = 0;
      }
      i = i+1;
    }
    
    universalAnswer = this.Inverse(binaryResInverse);
    return this.PrintBinary(binaryRes);
    
  }
  
  def Inverse(binaryTab: Int[]): Int = {
      var i: Int;
      var tabLength: Int;
      i = 0;
      tabLength = binaryTab.length;
      
      while(i < tabLength) {
        binaryRes[i] = binaryTab[tabLength-i-1];
        i = i+1;
      }
      return 42;
    }
  
  def ToHexa(binaryTab: Int[]): Int = {
    var i: Int;
    var j: Int;
    var powerOfTwo: Int;
    var hexNumber: Int;
    var hexRes: String;
    i = 0;
    j = 0;
    powerOfTwo = 1;
    hexNumber = 0;
    hexRes = "";
    
    while(i < binaryTab.length) {
      hexNumber = hexNumber + powerOfTwo*binaryTab[i];
      powerOfTwo = powerOfTwo*2;
      if(j == 3) {
        hexRes = hexRes + this.getHexRep(hexNumber);
        hexNumber = 0;
        powerOfTwo = 1;
        j = 0;
      }
      else {
        j = j+1;
      }
      i = i+1;
    }
    hexRes = hexRes + this.getHexRep(hexNumber);
    
    println("Hexadecimal representation of " + number + ":");
    println("0x" + hexRes);
    
    return 42;
  }
  
  def getHexRep(decimalNumber: Int): String = {
    var hexRes: String;
    hexRes = "";
    if(9<decimalNumber) {
          if(decimalNumber == 10) {
            hexRes = hexRes + "A";
          }
          if(decimalNumber == 11) {
            hexRes = hexRes + "B";
          }
          if(decimalNumber == 12) {
            hexRes = hexRes + "C";
          }
          if(decimalNumber == 13) {
            hexRes = hexRes + "D";
          }
          if(decimalNumber == 14) {
            hexRes = hexRes + "E";
          }
          if(decimalNumber == 15) {
            hexRes = hexRes + "F";
          }
        }
    else {
      hexRes = hexRes + decimalNumber;
    }
    
    return hexRes;
  }
  
  // Print the binary representation of a numbre in the inverse order
  def PrintBinary(binaryTab: Int[]): Int = {
    var i: Int;
    var binaryString: String;
    
    i = binaryTab.length-1;
    binaryString = "";
    
    while((0-1) < i) {
      binaryString = binaryString + binaryTab[i];
      i = i -1;
    }
    
    println("Binary representation of " + number + ":");
    println(binaryString);
    universalAnswer = this.ToHexa(binaryRes);
    return 42;
  }
}