object asd_main {
    def main(): Unit = {
        println(3 + 3);
        if(new asd_class().asd(66) == 67) {
            println("INFO: Done");
        }
    }
}

class asd_class {
    var asd: Int[];
    def sing(): Int = {
        asd[15] = 16;
	return 17;
    }

    def asd(a: Int): Int = {
    	a = 3;
	return 67;
    }
}
