package counter;

public class Main {

	public static void main(String[] args) {
		Counter counter = new Counter();
		Thread t1 = new Increaser(counter);
		Thread t2 = new Decreaser(counter);
		
		t1.start();
		t2.start();
		
		try {
			t1.join();
			t2.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		System.out.println("counter = " + counter.getValue());
	}

}
