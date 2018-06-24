//Josh Mitchell, Erin Varey
//mitchjp3, vareye
//001422856, 001400605
public class Account {
	
	private double balance;
	
	// Contains the lockID value of the thread that currently holds the lock to this account.
	// If whoHasLock == 0, no thread currently holds the account's lock.
	private int whoHasLock = 0;	

	public Account() {
		this.balance = 0;
	}

	//Getter for whoHasLock
	public int getWhoHasLock(){
		return whoHasLock; 
	}

	public double getBalance(){
		return balance; 
	}

	/*
	 * Threads attempt to acquire the lock to this Account.
	 * If no thread currently holds the lock, the lock is given to the asking thread and return true.
	 * If a thread already holds the lock, return false.
	 */
	public synchronized boolean acquireLock(int lockID){
		if (whoHasLock == 0){
			whoHasLock = lockID;
			return true;
		}
		else
			return false;
	}

	/*
	 * Once a thread no longer needs the account, it releases the lock.
	 * If the calling thread is the holder of the lock, release the lock.
	 * If the calling thread is not the holder of the lock, do nothing.
	 */
	public synchronized void releaseLock(int lockID){
		if (whoHasLock == lockID)
			whoHasLock = 0; 
	}

	public Account(double amount) {
		this.balance = amount;
	}

	synchronized void withdraw(double amount) throws InsufficientFundsException { 
		if (balance > amount ) {
			balance -= amount;
		}
		else {
			throw new InsufficientFundsException();
		}
	}

	synchronized void deposit(double amount) {
		balance += amount;
	}

	synchronized void transferTo(Account to, double amount) throws InsufficientFundsException {

		if (balance > amount) {
			balance -= amount;			
			to.deposit(amount);
			System.out.println('\n' + "Howdy! Account " + whoHasLock + " has transferred" + '\n');
		}
		else {
			throw new InsufficientFundsException();
		}
	}
}