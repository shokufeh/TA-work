//Josh Mitchell, Erin Varey
//mitchjp3, vareye
//001422856, 001400605

//Thread that takes 2 accounts and makes a transfer between them
public class AcctThreadBroken extends Thread{
	AccountBroken acct1;
	AccountBroken acct2;

	public AcctThreadBroken (AccountBroken account1, AccountBroken account2) {
		this.acct1 = account1;
		this.acct2 = account2;
	}

	public void run () {
		//delay 100ms
		try {
			Thread.sleep(1000);
		}
		catch (InterruptedException e) {
			e.printStackTrace();
		}
		//Make the transfer
		try {
			acct1.transferTo(acct2, 20);
		} catch (InsufficientFundsException e) {
			e.printStackTrace();
		}
		//if the transfer was successful, notify the user.
		System.out.println(this.getName() + " transfered successfully");
	}
}
