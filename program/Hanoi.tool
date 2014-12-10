object Hanoi {
    def main() : Unit = {
        if(new HanoiPresenter().play()) { println("Ok"); } else { println("error"); }
    }
}

class HanoiPresenter {

    var towerA: Tower;
    var towerB: Tower;
    var towerC: Tower;

    var steps: Int;

    def play() : Bool = {

        var N: Int;

        N = 8;
        steps = 0;

        towerA = new Tower().init(this.generateRings(N));
        towerB = new Tower().init(new LeafRing());
        towerC = new Tower().init(new LeafRing());

        if (! this.printAll()) { println("Error !!!"); }

        if (! this.mvRing(N, towerA, towerC, towerB)) { println("Error !!!"); }        

        println("Done in " + steps + " steps.");

		return true;
    }

    def mvRing2(a: Tower, b: Tower): Bool = {
        var v: Bool;
        v = b.push(a.pop());
        steps = steps + 1;
        if (! this.printAll()) { println("Error !!!"); }
        return v;
    }
    
    def mvRing(nb: Int, a: Tower, b: Tower, t: Tower): Bool = {
        var e: Bool;
        if (nb == 1) {
            e = this.mvRing2(a, b);
        } else {
            e = this.mvRing(nb-1, a, t, b);
            e = this.mvRing2(a, b);
            e = this.mvRing(nb-1, t, b, a);
        }
        return e;
    }

    def generateRings(n: Int): Ring = {
        var i: Int;
        var r: Ring;
        r = new LeafRing();
        
        i = 0;
        while (i < n) {
            i = i + 1;
            r = new Ring().init(i, r);
        }

        return r;
    }

    def printAll(): Bool = {
        println("A = " + towerA.print());
        println("B = " + towerB.print());
        println("C = " + towerC.print());
        println("---------------------");
        return true;
    }
}

class Ring {

    var name: Int;
    var next: Ring;

    def isLeaf(): Bool = {
        return false;
    }

    def init(i: Int, n: Ring) : Ring = {
        name = i;
        next = n;
        return this;
    }

    def print(): String = {
        var str: String;
        if (this.isLeaf()) str = "";
        else str = next.print() + " " + name;
        return str;
    }

    def push(r: Ring): Ring = {
        next = r;
        return this;
    }

    def pop(): Ring = {
        var tmp: Ring;
        tmp = next;
        next = new LeafRing();
        return tmp;
    }

}

class LeafRing extends Ring {
    def isLeaf(): Bool = {
        return true;
    }

    def print(): String = {
        return "";
    }
}

class Tower {

    var top: Ring;

    def init(r: Ring): Tower = {
        top = r;
        return this;
    }


    def push(r: Ring): Bool = {
        top = r.push(top);
        return true;
    }

    def pop(): Ring = {
        var tmp: Ring;
        tmp = top;
        top = tmp.pop();
        return tmp;
    }

    def print(): String = {
        return top.print();
    }

}
