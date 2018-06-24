public class Resource{

	private Object rs1;
	private Object rs2;

	public Resource(Object rs1, Object rs2){
		this.rs1 = rs1;
		this.rs2 = rs2;
	}


	public void delay(){
		try{
			Thread.sleep(1000);
		}
		catch (InterruptedException e) { System.out.println(e); }
	}


	//The following methods provide a way to synchronize the use of rs1 and rs2 in the desired manned to prevent deadlock
	public synchronized void getSpoonThenFork() throws InterruptedException{
		delay();
		System.out.println("Thread 1 is using rs1");

		delay();
		System.out.println("Thread 1 is using rs1 and rs2");

		delay();
		System.out.println("Thread 1 released rs1");

		delay();
		System.out.println("Thread 1 released rs2");
	}


	public synchronized void getForkThenSpoon() throws InterruptedException{
		delay();
		System.out.println("Thread 2 is using rs2");

		delay();
		System.out.println("Thread 2 is using rs2 and rs1");

		delay();
		System.out.println("Thread 2 released rs2");

		delay();
		System.out.println("Thread 2 released rs1");
	}
}