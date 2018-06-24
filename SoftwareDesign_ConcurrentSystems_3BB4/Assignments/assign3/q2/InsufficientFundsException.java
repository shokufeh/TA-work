//Josh Mitchell, Erin Varey
//mitchjp3, vareye
//001422856, 001400605
public class InsufficientFundsException extends Exception {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public InsufficientFundsException(){
		System.out.println("Can't over-draw account");
	}
}
