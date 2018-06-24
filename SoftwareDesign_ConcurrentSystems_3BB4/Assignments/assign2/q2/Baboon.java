public class Baboon implements Runnable{

	protected Rope rope;
	private boolean toRight;

	//The following two integers will represent the minimum and maximum time in ms that a baboon spends before going on the rope
	private int minReady = 500;
	private int maxReady = 1000;

	//The following two integers will represent the minimum and maximum time in ms that a baboon spends on the rope
	private int minCross = 2000;
	private int maxCross = 4000;

	private String direction = " "; //Holds the direction of each baboon


	//Class constructor
	public Baboon(Rope rope, boolean toRight){
		this.rope = rope;
		this.toRight = toRight;
	}

	//Returns true if the baboon is crossing to the right, and false if crossing to the left
	public boolean toRight(){
		return toRight;
	}


	public void run(){

		while (true){

			if(toRight()) direction = "right";
			else direction = "left";

			//Wait a random amount of ms in between the min and max defined above
			try{
				Thread.sleep((int) (minReady + (maxReady - minReady) * Math.random()));
			}
			catch(InterruptedException e){ e.printStackTrace(); }

			//Enter a queue of baboons to cross over
			try{
				rope.getReady(this); //Queue on the rope
			}
			catch(InterruptedException e){ e.printStackTrace(); }

			//The Baboons now cross over
			try{
				//Time taken by each baboon to cross over, still in between the min and max defined above
				Thread.sleep((int) (minCross + (maxCross - minCross) * Math.random()));
				System.out.println("A baboon is now crossing to the " + direction);
			}
			catch(InterruptedException e) { e.printStackTrace(); }

			rope.getOff(this); //Baboon got off the rope, another will go on

			if(!toRight()) direction = "right";
			else direction = "left";
			System.out.println("A baboon has arrived to the " + direction);
		}
	}
}