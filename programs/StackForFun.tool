object StackForFun {
	def main() : Unit = {
		if(new Stack().test() == 0) {
			println("OK");
		}
	}
}
class Stack {
	var arrStack : Int [ ];
	var nextFreeSlot : Int;
	var size : Int;
	def init(size_ : Int) : Stack = {
		nextFreeSlot = 0;
		arrStack = new Int [ size_ ];
		size = size_;
		return this;
	}
	def push(el : Int) : Int = {
		if(nextFreeSlot == size) {
			println("Stack full. Cannot push.");
		}
		else {
			arrStack [ nextFreeSlot ] = el;
			nextFreeSlot = nextFreeSlot + 1;
		}
		return 0;
	}
	def pop() : Int = {
		if(0 < nextFreeSlot) nextFreeSlot = nextFreeSlot - 1;
		else println("Stack empty. Cannot pop.");
		return arrStack [ nextFreeSlot ];
	}
	def test() : Int = {
		var stack : Stack;
		var trashBin : Int;
		stack = new Stack().init(100);
		trashBin = stack.push(35);
		trashBin = stack.push(36);
		trashBin = stack.push(37);
		trashBin = stack.pop();
		if(trashBin == 37) {
			println("You are a good stack. You return 37 as expected.");
		}
		else {
			println("What a bad stack ! Returned: " + trashBin);
		}
		trashBin = stack.pop();
		if(trashBin == 36) {
			println("You are a good stack. You return 36 as expected.");
		}
		else {
			println("What a bad stack ! Returned: " + trashBin);
		}
		trashBin = stack.pop();
		if(trashBin == 35) {
			println("You are a good stack. You return 35 as expected.");
		}
		else {
			println("What a bad stack ! Returned: " + trashBin);
		}
		return 0;
	}
}
