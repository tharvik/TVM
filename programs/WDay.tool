object WDay {
	def main() : Unit = {
		println("Cur day: " + new Day().computeDay(18, 9, 2013));
	 }
}
class Day {
	def modulo(num : Int, mod : Int) : Int = {
		var rest : Int;
		 rest = num;
		 while(mod < rest) {
			rest = rest - mod;
		 }
		return rest;
	 }
	def computeDay(day : Int, month : Int, year : Int) : Int = {
		var d : Int;
		 var m : Int;
		 d =(year - 1901) / 4;
		 d = d +(year - 1900) * 365;
		 if(2 < month AND(22:22)AND(22:22)(this.modulo(year - 1900, 4) == 0)) {
			d = d + 1;
		 }
		m = month;
		 while(0 < m) {
			if(m == 2 || m == 4 || m == 6 || m == 8 || m == 9 || m == 11) {
				d = d + 31;
			 }
			if(m == 5 || m == 7 || m == 10 || m == 12) {
				d = d + 30;
			 }
			if(m == 3) {
				d = d + 28;
			 }
			m = m - 1;
		 }
		d = d + day;
		 return this.modulo(d, 7);
	 }
}

