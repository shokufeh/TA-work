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

//   The program outputs the following stream: ( c.d )^{n}

import java.lang.Thread;
import java.util.concurrent.TimeUnit;

class CD extends Thread
{
	int n;
	Counter counter;
	
	public CD( )
	{
		this.n       = 0;
		this.counter = new Counter( 0 );
	}
	
	public CD( int n )
	{
		this.n       = n;
		this.counter = new Counter( 0 );
	}
	
	public CD( int n, Counter counter )
	{
		this.n       = n;
		this.counter = counter;
	}
	
	private void pause( long secs )
	{
		try{
			TimeUnit.SECONDS.sleep( secs );
		}
		catch( InterruptedException ie )
		{
			// DO NOTHING
		}
	}

	public void run()
	{
		int current = 0;
		
		for ( int i = 0; i < n; i++ )
		{
			System.out.print( "c " );
			this.pause( 1 );
			
			current = counter.value( );
			counter.set( ++current );
			
			System.out.print( "d " );
			this.pause( 1 );
			
			current = counter.value( );
			counter.set( ++current );
		}
	}
}