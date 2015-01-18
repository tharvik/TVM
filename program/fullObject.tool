object asd_main {
    def main(): Unit = {
        if(new asd_class().asd(999999)) {
            println("INFO: Done");
        }
    }
}

class asd_class {
    def asd(i: Int): Bool = {
    	var a: asd_class;
    	while(0 < i) {
		i = i - 1;

		a = new asd_class();
	}
	return true;
    }
}
