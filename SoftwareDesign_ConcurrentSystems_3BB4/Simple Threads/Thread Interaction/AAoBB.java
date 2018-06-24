//  Copyright (C) 2016 Valentin Cassano <cassanv@mcmaster.ca>
//
//    This file is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     Information on the GNU General Public License can be found
//     here: https://www.gnu.org/licenses/gpl.html
//     or here: https://en.wikipedia.org/wiki/GNU_General_Public_License

//   The program outputs the following stream: ( a.a | b.b )+

import java.lang.Thread;
import java.util.concurrent.TimeUnit;
import java.util.Random;

class AAoBB extends Thread
{
	AAoBB( )
	{}
	
	AAoBB( String name )
	{
		super(name);
	}
	
	private void pause( long secs )
	{
		try
		{
			TimeUnit.SECONDS.sleep(secs);
		}
		catch( InterruptedException e)
		{
			// DO NOTHING
		};
	}
	
	private void threadSafePrint( String string )
	{
		synchronized (System.out)
		{
			System.out.print( string );
		}
	}
	
	public void run()
	{
		Random random = new Random();
		
		while ( true )
		{
			int choose = random.nextInt(2);
			
			if ( choose == 0 )
			{		
				System.out.print( "a " );
				pause(1);
			
				System.out.print( "a " );
				pause(1);
			}
			else
			{
				System.out.print( "b " );
				pause(1);
			
				System.out.print( "b " );
				pause(1);
			}
		}
	}
}