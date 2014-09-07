import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class TreeMain{

	public static void main(String[] args) throws FileNotFoundException{
		Scanner filescan = new Scanner(new File("input2.txt"));
		String in_order_T = "";
		String post_order_T = "";

		//read in in-order-traversal string
		if(filescan.hasNextLine())
			in_order_T = filescan.nextLine();
		//read in post-order-traversal string
		if(filescan.hasNextLine())
			post_order_T = filescan.nextLine();

		if(in_order_T.equals("") || post_order_T.equals("")){
			throw new IllegalStateException("invalid file: file"+ 
				" must be a txt file with the first line an inorder "
				+"traversal\nand the second line a postorder traversal");
		}

		//read values from IOT string into list
		List<Integer> in_order_L = readValues(in_order_T);
		//read values from POT string into list
		List<Integer> post_order_L = reverseList(readValues(post_order_T));

		//create binary tree & build with values
		BinaryTree<Integer> tree = new BinaryTree<Integer>(in_order_L, post_order_L);

		//print tree
		System.out.println("TREE__");
		tree.printTree();
		
		//print out breadth first search
		System.out.println("\nBREADTH FIRST TRAV__");
		System.out.println(tree.breadthfirst());
	}

		

	public static List<Integer> readValues(String values){
		Scanner valuescan = new Scanner(values);
		valuescan.useDelimiter(",");

		List<Integer> list = new ArrayList<Integer>();
		while(valuescan.hasNext()){
			if(valuescan.hasNextInt())
				list.add(valuescan.nextInt());
			else
				throw new IllegalStateException("Invalid input: "
					+"must be two lines of integers separated only by commas");
		}
		return list;
	}

	public static List<Integer> reverseList(List<Integer> list){
		List<Integer> new_list = new ArrayList<Integer>();
		for(int i = list.size()-1; i >= 0; i--)
			new_list.add(list.get(i));
		
		return new_list;
	}
}