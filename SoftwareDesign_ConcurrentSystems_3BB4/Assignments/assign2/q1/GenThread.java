public class GenThread extends Thread{

	private Number num;

	public GenThread(Number n){ num = n; }

	public void run(){
		synchronized(num){ //whichever thread acquires this first gets the first slot available
			Random rand = new Random(10);
			num.concat(rand.nextInt());
		}
	}
}