package sync_counter;

public class Decreaser extends Thread {
	private SyncCounter counter;
	
	public Decreaser(SyncCounter c) {
		counter = c;
	}
	
	public void run() {
		counter.decrement();
	}
}
