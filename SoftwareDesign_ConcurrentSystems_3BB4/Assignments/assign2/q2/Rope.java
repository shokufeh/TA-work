import java.util.Iterator;
import java.util.LinkedList;

public class Rope{

	private LinkedList ropeQueue = new LinkedList(); //Baboons waiting to get on the rope
	private LinkedList onRope = new LinkedList(); //Baboons on the rope

	private int maximum = 5; //Rope's maximum occupancy

	//Empty class constructor
	public Rope(){}


	//Places a baboon add the rope queue
	public void getReady(Baboon baboon) throws InterruptedException{
		Baboon nextToCross;
		
		//Synchronize the class to prevent another baboon to enter the queue first
		synchronized (this){
			ropeQueue.add(baboon);
			nextToCross = getOff(null); //Checks to see if they are next to cross
		}

		if (baboon != nextToCross){
			synchronized (baboon){
				baboon.wait(); //Put baboon to "sleep" if not next to cross
			}
		}
	}

	//Gets the baboons on the rope, off the rope
	public synchronized Baboon getOff(Baboon remove){
		
		//If a baboon reached the end of the rope, take off the list
		if(remove != null){
			removeBaboon(remove, onRope);
		}
		
		Baboon add = null; //Initializes the next baboon to cross
		
		//Returns null if there is no one to cross yet
		if(ropeQueue.size() == 0){
			add = null;
		}
		
		//Returns null if the rope is full
		else if(onRope.size() >= maximum){
			add = null;
			System.out.println("The rope is at its maximum occupancy");
		}
		
		//Otherwise, deal with the next baboon
		else{
			Baboon baboonDirection = (Baboon) onRope.peek();
			Baboon nextBaboon = (Baboon) ropeQueue.peek();
			boolean goingRight;

			//When a baboon wants to cross, check their direction
			if(baboonDirection != null){
				goingRight = baboonDirection.toRight();
			}
			
			//If none will cross, then give the lock to whichever side is next
			else{
				goingRight = nextBaboon.toRight();
			}
			
			//If there are baboons in the queue, add them to potentially cross
			if(nextBaboon != null){
				
				//If the next baboon is going to the right, and the rope is either empty or is going to the right
				if(nextBaboon.toRight() && (goingRight || onRope.size() == 0)){
					add = nextBaboon;
				}
				
				// Otherwise, if the next baboon is going to the left, and and the rope is either empty or is going to the left
				else if(!nextBaboon.toRight() && (!goingRight || onRope.size() == 0)){
					goingRight = false;
					add = nextBaboon;
				}
				
				// Otherwise, the next baboon cannot cross yet
				else add = null;
			}
		}

		//When a baboon is chosen to cross
		if(add != null){
			
			//Remove them from the waiting queue
			if (add.toRight()){
				removeBaboon(add, ropeQueue);

			}
			else removeBaboon(add, ropeQueue);
			
			//Put them on the rope
			onRope.add(add);
			
			//"Wake" them up
			synchronized (add){
				add.notify();
			}
		}
		// Return the person
		return add;
	}

	//Removes a baboon from a list if one exists
	private void removeBaboon(Baboon baboon, LinkedList list){
		Iterator i = list.iterator();
		while (i.hasNext()){
			if (i.next() == baboon){
				i.remove();
			}
		}
	}
}