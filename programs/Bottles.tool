object Bottles {
	def main() : Unit = {
		if(new Wall().sing()) {
			println("INFO: Done");
		}
	}
}
class Wall {
	var bottles : Int;
	def sing() : Boolean = {
		var i : Int;
		var dummy : Int;
		dummy = this.placeBottles();
		i = 0;
		while(i < bottles + 1) {
			println(this.whatsOnTheWall());
			println(this.whatDoWeDoWithTheBottle());
			dummy = this.drink();
			println("");
		}
		return true;
	}
	def placeBottles() : Int = {
		bottles = 99;
		return bottles;
	}
	def drink() : Int = {
		bottles = bottles - 1;
		return bottles;
	}
	def whatsOnTheWall() : String = {
		var str : String;
		str = "";
		if(bottles == 0) str = "No more bottles of beer on the wall, no more bottles of beer.";
		if(bottles == 1) str = "1 bottle of beer on the wall, 1 bottle of beer.";
		if(1 < bottles) str = bottles + " bottles of beer on the wall, " + bottles + " bottles of beer.";
		return str;
	}
	def whatDoWeDoWithTheBottle() : String = {
		var str : String;
		str = "";
		if(bottles == 0) str = "Go to the store and buy some more, 99 bottles of beer on the wall.";
		if(bottles == 1) str = "Take one down and pass it around, no more bottles of beer on the wall.";
		if(bottles == 2) str = "Take one down and pass it around, 1 bottle of beer on the wall.";
		if(2 < bottles) str = "Take one down and pass it around, " +(bottles - 1) + " bottles of beer on the wall.";
		return str;
	}
}
