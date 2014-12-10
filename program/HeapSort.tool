object HeapSort {
    def main() : Unit = {
        println(new S().init(20).sort().printValues());
    }
}

class S {
    
	var number : Int[];
   	var size : Int;
		
	var useless : Int;

	def init(s: Int): S = {
	
		var i: Int;		
		var k: Int;	
	
		size = s;
		number = new Int[s];
	
		i = 0;
		k = 342;

		while(i < s){
		
			number[i] = k;
	
			k = this.nextInt(k);
				
			i = i+1;
		}
		
	
		return this;
	}

	def nextInt(i: Int): Int = {
		var r: Int;
		
		if(i - 2 * (i / 2) == 0){
			r = i/2;
		}else{
			r = 3*i+1;
		}
		
		return r;	
	}

	def printValues(): Int = {
		var i: Int;
		i = 0;
		println("----------------------------");
		while(i < size){
			println("Value[" + i + "]" + " = " + number[i]);
			i = i + 1;
		}
		println("----------------------------");
		
		return 0;
	}

	def swap_idx(a: Int, b: Int): Int = {
		var tmp : Int;
		tmp = number[b];
		number[b] = number[a];
		number[a] = tmp;
		
		return 0;

	}
	
	def max_idx_3(a: Int, b: Int, c: Int) : Int = {
		
		var ret: Int;
		if(!(number[a] < number[b])){
			if(!(number[a] < number[c])) {
				ret =  a;
			} else {
				ret = c;
			}
		}else{
			if(!(number[b] < number[c])){
				ret = b;
			}else{
				ret = c;
			}
		}
		return ret;
	}
	def max_idx_2(a: Int, b: Int): Int = {
		var ret: Int;
		if(!(number[a] < number[b]))	{ret = a;}
		else				{ret = b;}
		return ret;
	}

	def siftDown(start: Int, end: Int): Int = {
	
		var root: 	Int;
		var lchild: 	Int;	
		var rchild: 	Int;
		var max:	Int;
		var ret:	Bool;
		

		root 	= start;
		lchild 	= 2*root;
		ret	= false;

		rchild 	= lchild+1; 

		while(!(end < lchild) && !ret){

			if(!(end < rchild)){
				max = this.max_idx_3(root, lchild, rchild);
				if(max == root){
						ret = true;
				}else{
					if(max == lchild){
						useless = this.swap_idx(root, lchild);
						root = lchild;
					}else{
						useless = this.swap_idx(root, rchild);
						root = rchild;
					}
				}	
			}else{
				max = this.max_idx_2(root, lchild);
				if(max == root){
						ret = true;	
				}else{
						useless = this.swap_idx(root, lchild);
						root = lchild;
				}
			}
			
			lchild = 2*root;
			rchild = lchild + 1;	
			
		}

		return 0;
		
	}

	def heapify(): Int = {
		
		var start : Int;

		start = (size - 2)/2;
		while (!(start  < 0)) {
			useless = this.siftDown(start, size-1);
			start = start-1;
		}

		return 0;
	}

	def sort(): S = {

		var end: Int;
		useless = this.heapify();
			
		end = size - 1;


		while(0 < end){
			useless = this.swap_idx(end, 0);
			end = end-1;
			useless = this.siftDown(0, end);
		}

		return this;

	}


}




