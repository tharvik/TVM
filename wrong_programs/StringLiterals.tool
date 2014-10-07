object StringLiterals {
    def main() : Unit = {
        println("Okay");
        println("");
        println("Should work too\n");
        /* fail in toolc println("This is wrong
		"); */
    }
}
