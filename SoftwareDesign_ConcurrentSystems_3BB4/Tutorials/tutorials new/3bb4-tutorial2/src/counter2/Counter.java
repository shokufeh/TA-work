package counter2;

public class Counter {
	private int value = 0;
	
	public int getValue() {
		return value;
	}
	
	public void increment() {
		//value++;
		int tmp = value;
		tmp = tmp + 1;
		value = tmp;
	}
	
	public void decrement() {
		//value--;
		int tmp = value;
		tmp = tmp - 1;
		value = tmp;
	}
}
