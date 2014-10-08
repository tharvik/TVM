object Sudoku {
	def main() : Unit = {
		println(new Grid().init(2).solve().prettyPrint());
	 }
}
class Grid {
	var values : Int [ ];
	 var first : Int [ ];
	 def init(level : Int) : Grid = {
		values = new Int [ 81 ];
		 first = new Int [ 81 ];
		 if(level < 2) {
			values [ 0 ] = 6;
			 first [ 0 ] = 1;
			 values [ 1 ] = 0;
			 first [ 1 ] = 0;
			 values [ 2 ] = 0;
			 first [ 2 ] = 0;
			 values [ 3 ] = 1;
			 first [ 3 ] = 1;
			 values [ 4 ] = 0;
			 first [ 4 ] = 0;
			 values [ 5 ] = 8;
			 first [ 5 ] = 1;
			 values [ 6 ] = 2;
			 first [ 6 ] = 1;
			 values [ 7 ] = 0;
			 first [ 7 ] = 0;
			 values [ 8 ] = 3;
			 first [ 8 ] = 1;
			 values [ 9 ] = 0;
			 first [ 9 ] = 0;
			 values [ 10 ] = 2;
			 first [ 10 ] = 1;
			 values [ 11 ] = 0;
			 first [ 11 ] = 0;
			 values [ 12 ] = 0;
			 first [ 12 ] = 0;
			 values [ 13 ] = 4;
			 first [ 13 ] = 1;
			 values [ 14 ] = 0;
			 first [ 14 ] = 0;
			 values [ 15 ] = 0;
			 first [ 15 ] = 0;
			 values [ 16 ] = 9;
			 first [ 16 ] = 1;
			 values [ 17 ] = 0;
			 first [ 17 ] = 0;
			 values [ 18 ] = 8;
			 first [ 18 ] = 1;
			 values [ 19 ] = 0;
			 first [ 19 ] = 0;
			 values [ 20 ] = 3;
			 first [ 20 ] = 1;
			 values [ 21 ] = 0;
			 first [ 21 ] = 0;
			 values [ 22 ] = 0;
			 first [ 22 ] = 0;
			 values [ 23 ] = 5;
			 first [ 23 ] = 1;
			 values [ 24 ] = 4;
			 first [ 24 ] = 1;
			 values [ 25 ] = 0;
			 first [ 25 ] = 0;
			 values [ 26 ] = 0;
			 first [ 26 ] = 0;
			 values [ 27 ] = 5;
			 first [ 27 ] = 1;
			 values [ 28 ] = 0;
			 first [ 28 ] = 0;
			 values [ 29 ] = 4;
			 first [ 29 ] = 1;
			 values [ 30 ] = 6;
			 first [ 30 ] = 1;
			 values [ 31 ] = 0;
			 first [ 31 ] = 0;
			 values [ 32 ] = 7;
			 first [ 32 ] = 1;
			 values [ 33 ] = 0;
			 first [ 33 ] = 0;
			 values [ 34 ] = 0;
			 first [ 34 ] = 0;
			 values [ 35 ] = 9;
			 first [ 35 ] = 1;
			 values [ 36 ] = 0;
			 first [ 36 ] = 0;
			 values [ 37 ] = 3;
			 first [ 37 ] = 1;
			 values [ 38 ] = 0;
			 first [ 38 ] = 0;
			 values [ 39 ] = 0;
			 first [ 39 ] = 0;
			 values [ 40 ] = 0;
			 first [ 40 ] = 0;
			 values [ 41 ] = 0;
			 first [ 41 ] = 0;
			 values [ 42 ] = 0;
			 first [ 42 ] = 0;
			 values [ 43 ] = 5;
			 first [ 43 ] = 1;
			 values [ 44 ] = 0;
			 first [ 44 ] = 0;
			 values [ 45 ] = 7;
			 first [ 45 ] = 1;
			 values [ 46 ] = 0;
			 first [ 46 ] = 0;
			 values [ 47 ] = 0;
			 first [ 47 ] = 0;
			 values [ 48 ] = 8;
			 first [ 48 ] = 1;
			 values [ 49 ] = 0;
			 first [ 49 ] = 0;
			 values [ 50 ] = 3;
			 first [ 50 ] = 1;
			 values [ 51 ] = 1;
			 first [ 51 ] = 1;
			 values [ 52 ] = 0;
			 first [ 52 ] = 0;
			 values [ 53 ] = 2;
			 first [ 53 ] = 1;
			 values [ 54 ] = 0;
			 first [ 54 ] = 0;
			 values [ 55 ] = 0;
			 first [ 55 ] = 0;
			 values [ 56 ] = 1;
			 first [ 56 ] = 1;
			 values [ 57 ] = 7;
			 first [ 57 ] = 1;
			 values [ 58 ] = 0;
			 first [ 58 ] = 0;
			 values [ 59 ] = 0;
			 first [ 59 ] = 0;
			 values [ 60 ] = 9;
			 first [ 60 ] = 1;
			 values [ 61 ] = 0;
			 first [ 61 ] = 0;
			 values [ 62 ] = 6;
			 first [ 62 ] = 1;
			 values [ 63 ] = 0;
			 first [ 63 ] = 0;
			 values [ 64 ] = 8;
			 first [ 64 ] = 1;
			 values [ 65 ] = 0;
			 first [ 65 ] = 0;
			 values [ 66 ] = 0;
			 first [ 66 ] = 0;
			 values [ 67 ] = 3;
			 first [ 67 ] = 1;
			 values [ 68 ] = 0;
			 first [ 68 ] = 0;
			 values [ 69 ] = 0;
			 first [ 69 ] = 0;
			 values [ 70 ] = 2;
			 first [ 70 ] = 1;
			 values [ 71 ] = 0;
			 first [ 71 ] = 0;
			 values [ 72 ] = 3;
			 first [ 72 ] = 1;
			 values [ 73 ] = 0;
			 first [ 73 ] = 0;
			 values [ 74 ] = 2;
			 first [ 74 ] = 1;
			 values [ 75 ] = 9;
			 first [ 75 ] = 1;
			 values [ 76 ] = 0;
			 first [ 76 ] = 0;
			 values [ 77 ] = 4;
			 first [ 77 ] = 1;
			 values [ 78 ] = 0;
			 first [ 78 ] = 0;
			 values [ 79 ] = 0;
			 first [ 79 ] = 0;
			 values [ 80 ] = 5;
			 first [ 80 ] = 1;
		 }
		else if(level == 2) {
			values [ 0 ] = 0;
			 first [ 0 ] = 0;
			 values [ 1 ] = 9;
			 first [ 1 ] = 1;
			 values [ 2 ] = 0;
			 first [ 2 ] = 0;
			 values [ 3 ] = 0;
			 first [ 3 ] = 0;
			 values [ 4 ] = 0;
			 first [ 4 ] = 0;
			 values [ 5 ] = 0;
			 first [ 5 ] = 0;
			 values [ 6 ] = 0;
			 first [ 6 ] = 0;
			 values [ 7 ] = 0;
			 first [ 7 ] = 0;
			 values [ 8 ] = 8;
			 first [ 8 ] = 1;
			 values [ 9 ] = 2;
			 first [ 9 ] = 1;
			 values [ 10 ] = 0;
			 first [ 10 ] = 0;
			 values [ 11 ] = 0;
			 first [ 11 ] = 0;
			 values [ 12 ] = 0;
			 first [ 12 ] = 0;
			 values [ 13 ] = 0;
			 first [ 13 ] = 0;
			 values [ 14 ] = 0;
			 first [ 14 ] = 0;
			 values [ 15 ] = 1;
			 first [ 15 ] = 1;
			 values [ 16 ] = 9;
			 first [ 16 ] = 1;
			 values [ 17 ] = 0;
			 first [ 17 ] = 0;
			 values [ 18 ] = 4;
			 first [ 18 ] = 1;
			 values [ 19 ] = 0;
			 first [ 19 ] = 0;
			 values [ 20 ] = 0;
			 first [ 20 ] = 0;
			 values [ 21 ] = 2;
			 first [ 21 ] = 1;
			 values [ 22 ] = 0;
			 first [ 22 ] = 0;
			 values [ 23 ] = 1;
			 first [ 23 ] = 1;
			 values [ 24 ] = 0;
			 first [ 24 ] = 0;
			 values [ 25 ] = 0;
			 first [ 25 ] = 0;
			 values [ 26 ] = 0;
			 first [ 26 ] = 0;
			 values [ 27 ] = 0;
			 first [ 27 ] = 0;
			 values [ 28 ] = 3;
			 first [ 28 ] = 1;
			 values [ 29 ] = 0;
			 first [ 29 ] = 0;
			 values [ 30 ] = 0;
			 first [ 30 ] = 0;
			 values [ 31 ] = 0;
			 first [ 31 ] = 0;
			 values [ 32 ] = 6;
			 first [ 32 ] = 1;
			 values [ 33 ] = 0;
			 first [ 33 ] = 0;
			 values [ 34 ] = 8;
			 first [ 34 ] = 1;
			 values [ 35 ] = 7;
			 first [ 35 ] = 1;
			 values [ 36 ] = 0;
			 first [ 36 ] = 0;
			 values [ 37 ] = 0;
			 first [ 37 ] = 0;
			 values [ 38 ] = 0;
			 first [ 38 ] = 0;
			 values [ 39 ] = 7;
			 first [ 39 ] = 1;
			 values [ 40 ] = 0;
			 first [ 40 ] = 0;
			 values [ 41 ] = 9;
			 first [ 41 ] = 1;
			 values [ 42 ] = 0;
			 first [ 42 ] = 0;
			 values [ 43 ] = 0;
			 first [ 43 ] = 0;
			 values [ 44 ] = 0;
			 first [ 44 ] = 0;
			 values [ 45 ] = 7;
			 first [ 45 ] = 1;
			 values [ 46 ] = 1;
			 first [ 46 ] = 1;
			 values [ 47 ] = 0;
			 first [ 47 ] = 0;
			 values [ 48 ] = 8;
			 first [ 48 ] = 1;
			 values [ 49 ] = 0;
			 first [ 49 ] = 0;
			 values [ 50 ] = 0;
			 first [ 50 ] = 0;
			 values [ 51 ] = 0;
			 first [ 51 ] = 0;
			 values [ 52 ] = 3;
			 first [ 52 ] = 1;
			 values [ 53 ] = 0;
			 first [ 53 ] = 0;
			 values [ 54 ] = 0;
			 first [ 54 ] = 0;
			 values [ 55 ] = 0;
			 first [ 55 ] = 0;
			 values [ 56 ] = 0;
			 first [ 56 ] = 0;
			 values [ 57 ] = 5;
			 first [ 57 ] = 1;
			 values [ 58 ] = 0;
			 first [ 58 ] = 0;
			 values [ 59 ] = 3;
			 first [ 59 ] = 1;
			 values [ 60 ] = 0;
			 first [ 60 ] = 0;
			 values [ 61 ] = 0;
			 first [ 61 ] = 0;
			 values [ 62 ] = 6;
			 first [ 62 ] = 1;
			 values [ 63 ] = 0;
			 first [ ... (truncated)</code></pre>                 </div>     <div class="modal-footer">       <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>     </div>