//Josh Mitchell, Erin Varey
//mitchjp3, vareye
//001422856, 001400605
public class AccountBroken extends Thread {
	double balance;

	public AccountBroken() {
		this.balance = 0;
	}

	public AccountBroken(double amount) {
		this.balance = amount;
	}

	public double getBalance(){
		return balance; 
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

	synchronized void transferTo(AccountBroken to, double amount) throws InsufficientFundsException {

		if (balance > amount) {
			balance -= amount;			
			to.deposit(amount);
		}
		else {
			throw new InsufficientFundsException();
		}
	}
}
