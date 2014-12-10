object Shapes {
	def main() : Unit = {
		println(new Shape().test());
	}
}

class Shape {
	def whyThisMethod(): String = {
		return "This method is just here to test if the call to parent's methods works.";
	}
	def draw(): String = {
		println("I am a random shape :");
		println("_/\_____ ");
		println("\       \");
		return "/____/\_|";
	}
	def test() : String = {
		println(new Shape().draw());
		println(new Quadrilateral().draw());
		println(new Diamond().draw());
		println(new Rectangle().draw());
		println(new Square().draw());
		println(new Triangle().draw());
		println(new Diamond().family());
		println(new Rectangle().family());
		println(new Square().family());
		return new Square().whyThisMethod();
	}
}

class Quadrilateral extends Shape {
	def draw() : String = {
		println("I am a quadrilateral :");
		println("    _________ ");
		println("   /         |");
		println("  /          |");
		return " /___________|";
	}
	def family() : String = {
		return "We are the quadrilaterals.";
	}
}

class Diamond extends Quadrilateral {
	def draw() : String = {
		println("I am a pretty diamond.");
		println("       /\       ");
		println("      /  \      ");
		println("     /    \     ");
		println("    /      \    ");
		println("   /        \   ");
		println("  /          \  ");
		println("  \          /  ");
		println("   \        /   ");
		println("    \      /    ");
		println("     \    /     ");
		println("      \  /      ");
		return "       \/       ";
	}
}

class Rectangle extends Quadrilateral {
	def draw() : String = {
		println("Magnificient rectangle :");
		println(" _________________ ");
		println("|                 |");
		println("|                 |");
		println("|                 |");
		return "|_________________|";
	}
	def family() : String = {
		return "We are the rectangles, a sub-family of the quadrilaterals.";
	}
}

class Square extends Rectangle {
	def draw() : String = {
		println("Beautiful square :");
		println(" __________ ");
		println("|          |");
		println("|          |");
		println("|          |");
		return "|__________|";
	}
}

class Triangle extends Shape {
	def draw() : String = {
		println("Funny Triangle :");
		println("       /\       ");
		println("      /  \      ");
		println("     /    \     ");
		println("    /      \    ");
		println("   /        \   ");
		return "  /__________\  " ;
	}
}