object Boolean {
    def main(): Unit = {
        println(new Bool_c().go());
    }
}

class Bool_c {

    def go(): Bool = {
        var b: Bool;

        b = false;
        println(b);

        b = true;
        println(b);

        b = !b;
        println(b);

        return b;
    }
}
