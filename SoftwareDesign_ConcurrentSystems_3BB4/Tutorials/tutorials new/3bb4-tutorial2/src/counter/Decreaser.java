package counter;

public class Decreaser extends Thread {
	private Counter counter;
	
	public Decreaser(Counter c) {
		counter = c;
	}
	
	public void run() {
		counter.decrement();
	}
}
