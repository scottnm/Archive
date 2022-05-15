import java.util.*;

public class BinaryTree<E extends Comparable<E>>{
	
	private BTNode<E> root;
	private int size;
	private List<E> order;

	BinaryTree(List<E> order){
		root = null;
		size = 0;
		this.order = order;
	}

	BinaryTree(List<E> order, List<E> list){
		this(order);
		for(E val: list){
			this.add(val);
		}
	}

	//add node
	/*
	* kick off method for the recursive add method
	*/
	public boolean add(E val){
		int init_size = size;
		root = addHelper(root, val);
		return size > init_size;
	}

	/*
	* recursively traverse the tree until you reach a null leaf to add
	* the new value to 
	* if the value is already present do nothing
	*/
	public BTNode<E> addHelper(BTNode<E> curr_node, E val){
		if(curr_node == null){
			size++;
			return new BTNode<E>(val);
		}

		int val_index = order.indexOf(val);
		int curr_index = order.indexOf(curr_node.data);

		int comparison = val_index > curr_index ? 1 : val_index < curr_index ? -1 : 0;

		if(comparison > 0)
			curr_node.right = addHelper(curr_node.right, val);
		else if(comparison < 0)
			curr_node.left = addHelper(curr_node.left, val);
		
		return curr_node;
	}

	//remove node
	/*
	* kick off method for the recursive add method
	*/
	public boolean remove(E val){
		int init_size = size;
		root = addHelper(root, val);
		return size > init_size;
	}

	/*
	* recursively traverse the tree until you reach a null leaf to add
	* the new value to 
	* if the value is already present do nothing
	*/
	public BTNode<E> removeHelper(BTNode<E> curr_node, E val){
		if(curr_node == null){
			return null;
		}

		int val_index = order.indexOf(val);
		int curr_index = order.indexOf(curr_node.data);

		int comparison = val_index > curr_index ? 1 : val_index < curr_index ? -1 : 0;

		if(comparison > 0)
			curr_node.right = removeHelper(curr_node.right, val);
		else if(comparison < 0)
			curr_node.left = removeHelper(curr_node.left, val);
		
		else{
			/*
			 * the node is either a leaf
			 * so just make that node a null
			 */
			if(curr_node.left == null && curr_node.right == null){
				size--;
				return null;
			}
			/*
			 * ..or only has one children
			 * so replace that node with its child
			 */
			else if(curr_node.left == null || curr_node.right == null){
				size--;
				if(curr_node.left == null)
					return curr_node.right;
				else
					return curr_node.left;
			}
			/* 
			 * interesting case with two children: 
			 * get the max element of the node to be removed's left subtree
			 * remove that max element's node from the left subtree
			 * change the currentNode to that max element
			 * decrementing the size is handled in call to removeHelper()
			 */
			else{
				E max_of_left_subtree = getMax(curr_node.left);
				curr_node.left = removeHelper(curr_node.left, max_of_left_subtree);
				curr_node.data = max_of_left_subtree;
			}
		}
		return curr_node;
	}

	//contains kick off method
	public boolean contains(E val){
		return contains(root, val);
	}

	//recursively traverses tree to find value
	private boolean contains(BTNode<E> curr_node, E val){
		if(curr_node == null)
			return false;

		int val_index = order.indexOf(val);
		int curr_index = order.indexOf(curr_node.data);

		int comparison = val_index > curr_index ? 1 : val_index < curr_index ? -1 : 0;

		if(comparison > 0)
			return contains(curr_node.right, val);
		else if(comparison < 0)
			return contains(curr_node.left, val);
		
		return true;

	}

	//getMax
	public E getMax(BTNode<E> start_node){
		BTNode<E> curr_node = start_node;
		while(curr_node.right != null)
			curr_node = curr_node.right;

		return curr_node.data;
	}

	public String breadthfirst(){
		StringBuilder traversal = new StringBuilder("");
		breadthHelper(traversal);
		return traversal.toString();
	}

	//breadthfirst traversal
	public void breadthHelper(StringBuilder traversal){
    	Queue<BTNode<E>> q = new LinkedList<BTNode<E>>();
    	q.clear();
    	q.add(root);
    	while(!q.isEmpty()){
    		BTNode<E> node = q.remove();
    		traversal.append(node.data.toString()+",");
    		if(node.left != null) q.add(node.left);
    		if(node.right != null) q.add(node.right);
    	}
    	if(traversal.charAt(traversal.length()-1)==',')
    		traversal.deleteCharAt(traversal.length()-1);

    }

	//reset
	public void reset(){
		root = null;
		size = 0;
	}

	//printtree
	public void printTree() {
		printTree(root, "");
	}

	private void printTree(BTNode<E> n, String spaces) {
		if(n != null){
			printTree(n.right, spaces + "  ");
			System.out.println(spaces + n.data);
			printTree(n.left, spaces + "  ");
		}
	}

	//Binary Tree Node class
	private static class BTNode<E extends Comparable<E>>{
		private BTNode<E> left;
		private BTNode<E> right;
		private E data;
		BTNode(){
			this(null);
		}

		BTNode(E init_data){
			this(null, init_data, null);
		}

		BTNode(BTNode<E> left, E init_data, BTNode<E> right){
			this.left = left;
			this.right = right;
			data = init_data;
		}
	}

}