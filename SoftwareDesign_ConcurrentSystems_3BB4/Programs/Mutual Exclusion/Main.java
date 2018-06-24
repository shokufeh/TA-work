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

//   The program outputs ( a.a | b.b )+ || ( c.d )+

import java.lang.Integer;

class Main
{
	public static void main( String args[] )
    {
    	Counter counter   = new Counter( 0 );
    	
    	AAoLambda one = new AAoLambda( 2, counter );
    	CD        two = new CD       ( 2, counter );
    	
    	one.start( );
    	two.start( );
    	
    	try
    	{
			one.join( );
			two.join( );
		}
		catch ( InterruptedException ie )
		{
			// DO NOTHING
		}
    	
    	System.out.println( counter.value( ) );
    }
}
