import java.util.Scanner;

public class Main{
	public static void main(String[] args) throws InterruptedException{
		Scanner scan = new Scanner(System.in);
		System.out.println("Enter the number of slots: ");
		int N = scan.nextInt(); //assuming that TA will only give integers as input
		scan.close();

		GenThread[] thread = new GenThread[N];

		while(true){
			Number num = new Number(); //clearing its content for every loop
			
			for(int i=0;i<N;i++){
				thread[i] = new GenThread(num);
				thread[i].start();
				Thread.sleep(4); //delay required for randomness otherwise a fast computer would produce the same digit multiple times
			}

			for(int i=0;i<N;i++) thread[i].join(); //waiting for all threads to execute before printing result

			System.out.println(num.getNumber());
			System.out.println();
			Thread.sleep(1000);
		}
	}
}