//This class is created to simplify conversion processes and ease synchronization and resource sharing
public class Number{

	private String number;
	
	public Number(){ number = ""; } //creates an empty number
	
	public void concat(int digit){ number = number.concat(Integer.toString(digit)); } //appends parameter to already existing string
	
	public String getNumber(){ return number; } //getter
}