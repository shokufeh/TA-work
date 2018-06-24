public class Main{

	public static void main(String[] args){

		//Since there are two different types of baboons. True will represent baboons crossing to the right, and false will represent baboons crossing to the left
		boolean toRight = true;

		Rope rope = new Rope();

		//This creates 6 threads (baboons) on each side to simulate the problem
		for (int i=0;i<6;i++){
			new Thread(new Baboon(rope, !toRight)).start();
			new Thread(new Baboon(rope, toRight)).start();
		}
	}
}
