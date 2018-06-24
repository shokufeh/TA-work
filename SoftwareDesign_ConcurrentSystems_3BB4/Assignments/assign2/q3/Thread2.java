public class Thread2 implements Runnable{

	private Resource resource;

	//Simple constructor
	public Thread2(Resource resource){
		this.resource = resource;
	}

	public void run(){
		while (true){
			try{
				resource.getForkThenSpoon();
				System.out.println(); //Very short delay
			}
			catch (InterruptedException e) { System.out.println(e); }
		} 
	}
}