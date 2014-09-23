object bool {
	def main() : Unit = {
		println(new bool_c().go());
	}
}

class bool_c {

	def go() : Bool = {
		var b: Bool;

		b = false;
		println(b);

		b = true;
		println(b);

		b = ! b;
		println(b);

		return b;
	}
}
