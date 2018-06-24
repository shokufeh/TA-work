package sync_counter;

public class SyncCounter {
	private int value = 0;
	
	public int getValue() {
		return value;
	}
	
	public synchronized void increment() {
		//value++;
		int tmp = value;
		tmp = tmp + 1;
		value = tmp;
	}
	
	public synchronized void decrement() {
		//value--;
		int tmp = value;
		tmp = tmp - 1;
		value = tmp;
	}
}
