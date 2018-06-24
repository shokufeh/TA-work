//Josh Mitchell, Erin Varey
//mitchjp3, vareye
//001422856, 001400605
public class Bank {
	
	/*
	 * This program no longer deadlocks because the AcctThread class has the ability to
	 * give up the locks it possesses.
	 */
	public static void main(String[] args) throws InterruptedException {
		//Create the 3 accounts that will transfer to each other
		Account act1 = new Account(100);
		Account act2 = new Account(650);
		Account act3 = new Account(25);

		//Create the 3 threads that will operate the accounts
		AcctThread th1 = new AcctThread(act1, act2, 1);
		AcctThread th2 = new AcctThread(act2, act1, 2);
		AcctThread th3 = new AcctThread(act2, act3, 3);

		//Print the initial balances of all 3 accounts
		System.out.println("Initial Account 1 balance: " + act1.getBalance());
		System.out.println("Initial Account 2 balance: " + act2.getBalance());
		System.out.println("Initial Account 3 balance: " + act3.getBalance());
		
		//Start each thread
		th1.start();
		th2.start();
		th3.start();
		
		//while at least 1 thread has not terminated, print the account balances every 100ms
		while (th1.isAlive() || th2.isAlive() || th3.isAlive()) {
			Thread.sleep(100);
			System.out.println("Account 1 balance: " + act1.getBalance());
			System.out.println("Account 2 balance: " + act2.getBalance());
			System.out.println("Account 3 balance: " + act3.getBalance());
		}
		//Once all 3 threads have terminated, print the final account balances.
		//This state will always be reached, as the program no longer deadlocks
		System.out.println("Final Account 1 balance: " + act1.getBalance());
		System.out.println("Final Account 2 balance: " + act2.getBalance());
		System.out.println("Final Account 3 balance: " + act3.getBalance());
	}
}