object BinaryTree {
	def main() : Unit = {
		println(new BTComputer().compute());
	 }
}
class BTComputer {
	var theTree : BT;
	 var temp : Boolean;
	 def compute() : String = {
		theTree = new BT();
		 temp = theTree.setUpBT();
		 temp = theTree.addNode(50, "Boss");
		 temp = theTree.addNode(25, "Vice President");
		 temp = theTree.addNode(15, "Office Manager");
		 temp = theTree.addNode(30, "Secretary");
		 temp = theTree.addNode(75, "Sales Manager");
		 temp = theTree.addNode(85, "Salesman 1");
		 temp = theTree.inOrderTraverseTree(theTree.getRoot());
		 println("Node with the key 75");
		 println(theTree.findNode(75).toString());
		 return " *OK* ";
	 }
}
class Null extends Node {
}
class BT {
	var root : Node;
	 var null : Null;
	 var temp : Boolean;
	 def getRoot() : Node = {
		return root;
	 }
	def setUpBT() : Boolean = {
		null = new Null();
		 root = null;
		 temp = false;
		 return true;
	 }
	def addNode(key : Int, name : String) : Boolean = {
		var newNode : Node;
		 var parent : Node;
		 var focusNode : Node;
		 var stop : Boolean;
		 stop = true;
		 newNode = new Node();
		 temp = newNode.setNode(key, name, null);
		 if(root == null) {
			root = newNode;
		 }
		else {
			focusNode = root;
			 while(stop) {
				parent = focusNode;
				 if(key < focusNode.getKey()) {
					focusNode = focusNode.getLeftChild();
					 if(focusNode == null) {
						temp = parent.setLeftChild(newNode);
						 stop = false;
					 }
				}
				else {
					focusNode = focusNode.getRightChild();
					 if(focusNode == null) {
						temp = parent.setRightChild(newNode);
						 stop = false;
					 }
				}
			}
		}
		return true;
	 }
	def inOrderTraverseTree(focusNode : Node) : Boolean = {
		if(!(focusNode == null)) {
			temp = this.inOrderTraverseTree(focusNode.getLeftChild());
			 println(focusNode.toString());
			 temp = this.inOrderTraverseTree(focusNode.getRightChild());
		 }
		return true;
	 }
	def preorderTraverseTree(focusNode : Node) : Boolean = {
		if(!(focusNode == null)) {
			println(focusNode.toString());
			 temp = this.preorderTraverseTree(focusNode.getLeftChild());
			 temp = this.preorderTraverseTree(focusNode.getRightChild());
		 }
		return true;
	 }
	def postOrderTraverseTree(focusNode : Node) : Boolean = {
		if(!(focusNode == null)) {
			temp = this.postOrderTraverseTree(focusNode.getLeftChild());
			 temp = this.postOrderTraverseTree(focusNode.getRightChild());
			 println(focusNode.toString());
		 }
		return true;
	 }
	def findNode(key : Int) : Node = {
		var focusNode : Node;
		 var ret : Node;
		 focusNode = root;
		 while(!(focusNode.getKey() == key)) {
			if(key < focusNode.getKey()) {
				focusNode = focusNode.getLeftChild();
			 }
			else {
				focusNode = focusNode.getRightChild();
			 }
			if(focusNode == null) ret = null;
		 }
		ret = focusNode;
		 return ret;
	 }
}
class Node {
	var null : Null;
	 var key : Int;
	 var name : String;
	 var leftChild : Node;
	 var rightChild : Node;
	 def setNode(key1 : Int, name1 : String, null1 : Null) : Boolean = {
		key = key1;
		 name = name1;
		 null = null1;
		 leftChild = null1;
		 rightChild = null1;
		 return true;
	 }
	def getKey() : Int = {
		return key;
	 }
	def getLeftChild() : Node = {
		return leftChild;
	 }
	def getRightChild() : Node = {
		return rightChild;
	 }
	def setLeftChild(child : Node) : Boolean = {
		leftChild = child;
		 return true;
	 }
	def setRightChild(child : Node) : Boolean = {
		rightChild = child;
		 return true;
	 }
	def toString() : String = {
		var resultat : String;
		 resultat = name + " has the key " + key;
		 return resultat;
	 }
}

