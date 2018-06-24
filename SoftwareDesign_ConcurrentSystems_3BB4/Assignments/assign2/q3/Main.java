public class Main{

	public static void main(String[] args){

		final Object rs1 = new Object();
		final Object rs2 = new Object();

		Resource resource = new Resource(rs1, rs2);  

		new Thread(new Thread1(resource)).start();
		new Thread(new Thread2(resource)).start();

	}
}