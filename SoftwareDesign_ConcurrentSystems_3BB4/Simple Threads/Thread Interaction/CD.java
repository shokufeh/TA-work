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

//   The program outputs the following stream: ( c.d )+

import java.lang.Thread;
import java.util.concurrent.TimeUnit;

class CD extends Thread
{
	CD( )
	{}
	
	CD( String name )
	{
		super(name);
	}
	
	public void pause( long secs )
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
		while ( true )
		{
				System.out.print( "c " );
				pause(1);
			
				System.out.print( "d " );
				pause(1);
		}
	}
}