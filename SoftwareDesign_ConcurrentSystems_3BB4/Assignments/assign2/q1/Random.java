public class Random{

	private int max;  //10, really
	private int last; //last number created

	public Random(int max){
		this.max  = max;
		this.last = (int) (System.currentTimeMillis() % max); //to get a digit within 0-9
	}

	public int nextInt(){ return (last * 32719 + 3) % max; } //returns the random number
}