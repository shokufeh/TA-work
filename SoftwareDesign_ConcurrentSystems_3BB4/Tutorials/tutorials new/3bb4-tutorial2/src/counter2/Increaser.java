package counter2;

public class Increaser extends Thread {
	private Counter counter;

	public Increaser(Counter c) {
		counter = c;
	}

	public void run() {
		synchronized (counter) {
			counter.increment();
		}
	}
}
