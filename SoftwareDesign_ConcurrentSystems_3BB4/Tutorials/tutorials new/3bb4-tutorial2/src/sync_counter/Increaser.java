package sync_counter;

public class Increaser extends Thread {
	private SyncCounter counter;
	
	public Increaser (SyncCounter c) {
		counter = c;
	}
	
	public void run() {
		counter.increment();
	}
}
