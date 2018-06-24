//Josh Mitchell, Erin Varey
//mitchjp3, vareye
//001422856, 001400605

//Thread that takes 2 accounts and makes a transfer between them
public class AcctThread extends Thread{
	Account acct1;
	Account acct2;
	int lockID;	//A unique value given to each thread, to identify "who" holds the lock on an account

	public AcctThread (Account account1, Account account2, int lockID) {
		this.acct1 = account1;
		this.acct2 = account2;
		this.lockID = lockID;
	}

	public void run () {
		//delay 100ms
		try {
			Thread.sleep(100);
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
		/*This prevents the deadlock.
		 * If the acquisition of a lock is unsuccessful, acquireLock() returns false.
		 * If either account's lock can't be acquired, then any locks the thread holds are released.
		 * Only if both locks can be acquired by this thread, does the transfer proceed.
		 */
		while (!acct1.acquireLock(lockID) || !acct2.acquireLock(lockID)){ 
			acct1.releaseLock(lockID);
			acct2.releaseLock(lockID);		
		}
		
		//Since both locks are acquired, proceed with the transfer
		try {
			acct1.transferTo(acct2, 20);
		} catch (InsufficientFundsException e) {
			e.printStackTrace();
		}

		//Release both locks so other threads may use the accounts
		acct1.releaseLock(lockID);
		acct2.releaseLock(lockID);
	}
}