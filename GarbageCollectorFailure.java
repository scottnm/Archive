/**
 * To compile: javac GarbageCollectorFailure.java
 * To run:     java -Xmx2m GarbageCollectorFailure
 * 
 * This file serves as an example of how the garbage collector in Java
 * helps us manage memory. In the first loop we allocate 10,000 objects
 * just fine, but because nothing holds a reference to those objects after
 * each iteration of the for loop the garbage collector is able to reclaim
 * the memory that those objects consume and not run out of memory. In the
 * second loop, we give each object a reference to the next object that we
 * allocate (kind of like a linked list). As well the first object is
 * allocated outside of the loop. This means that when the garbage
 * collector runs it can not reclaim any of the memory allocated for the
 * previous objects because they are still reachable through references
 * from some other variable (in this case, our "so" variable at the start
 * of the main function. Eventually our program crashes after running out
 * of memory after trying to create roughly 1,500 SimpleObjs.
 */

public class GarbageCollectorFailure
{
    public static void main(String[] args)
    {
        SimpleObj so = new SimpleObj();
        so.n = 1;

        SimpleObj currObj = so;
        int objectAllocationLimit = 10000;
        for (int i = 0; i < objectAllocationLimit; ++i)
        {
            SimpleObj nextObj = new SimpleObj();
        }

        System.out.printf("Program was able to allocate %d objs in"
                + " the first for loop\nbut...\n", objectAllocationLimit);

        while(true)
        {
            try
            {
                // create a new object
                SimpleObj nextObj = new SimpleObj();
                nextObj.n = currObj.n + 1;

                // give the current object a reference to the new object
                currObj.o = nextObj;

                // update our current reference to the new object
                currObj = nextObj;
            }
            catch (OutOfMemoryError e)
            {
                System.out.printf("only could allocate up to ele #%d"
                        + " in the second loop\n", currObj.n);
                break;
            }
        }
    }
}

class SimpleObj
{
    public SimpleObj()
    {
        arr = new int[1000];
    }

    public SimpleObj o;
    public int n;
    public int[] arr;
}

