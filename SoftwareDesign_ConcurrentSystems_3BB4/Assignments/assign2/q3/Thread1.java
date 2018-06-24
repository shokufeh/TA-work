public class Thread1 implements Runnable{

    private Resource resource;

    //Simple constructor
    public Thread1(Resource resource){
        this.resource = resource;
    }

    public void run(){
        while (true){
            try{
            	resource.getSpoonThenFork();
            	System.out.println();
            }
            catch (InterruptedException e) { System.out.println(e); }
        } 
    }
}