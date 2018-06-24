//Josh Mitchell, Erin Varey
//mitchjp3, vareye
//001422856, 001400605
public class BankBroken {
	
	/*
	 * This program can deadlock if multiple accounts try to transfer to each other concurrently.
	 * The transfer function requires a lock on both the sending and receiving account.
	 * If one account grabs one lock while the other account grabs the other, neither can make
	 * the transfer and the program doesn't terminate. 
	 */
	
	public static void main(String[] args) throws InterruptedException {
		//Create the 3 accounts that will try to transfer
		AccountBroken act1 = new AccountBroken(100);
		AccountBroken act2 = new AccountBroken(650);
		AccountBroken act3 = new AccountBroken(25);

		//Create the threads that will operate the accounts
		AcctThreadBroken th1 = new AcctThreadBroken(act1, act2);
		AcctThreadBroken th2 = new AcctThreadBroken(act2, act1);
		AcctThreadBroken th3 = new AcctThreadBroken(act2, act3);

		//Print the initial balances of all 3 accounts
		System.out.println("Initial Account 1 balance: " + act1.getBalance());
		System.out.println("Initial Account 2 balance: " + act2.getBalance());
		System.out.println("Initial Account 3 balance: " + act3.getBalance());
		
		//start all 3 threads
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
		//This state will not always be reached, as the program can deadlock
		System.out.println("Final Account 1 balance: " + act1.getBalance());
		System.out.println("Final Account 2 balance: " + act2.getBalance());
		System.out.println("Final Account 3 balance: " + act3.getBalance());
	}
}