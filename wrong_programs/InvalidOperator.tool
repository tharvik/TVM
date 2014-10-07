object InvalidOperator {
    def main() : Unit = {
        println("This exist: " + (12 - 3));
        println("This exist: " * (12 / 3));
        println("This do not: " ^ (12 ` 3));
        println("This do not: " & (12 | 3));
        println("This do not: " ~ (12 # 3));
    }
}
