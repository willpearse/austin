README Austin - MCMC sampling (...sort of...)
============================================
Will Pearse (wdpearse@umn.edu)

##Overview
This is intended to be a quick way of doing MCMC sampling, and I'm aiming it primarily at ecologists because I am one.
I have made very little attempt at making this code generally usable for other people - please do try, and please do email me, but I'd suggest you consider using something like Filzbach (http://research.microsoft.com/en-us/projects/filzbach/).

##Requirements
* GNU Scientific Library (for statistical distributions)
* C++11 or higher (for threading)

##Suggestions
* Write your own likelihood (DataModel class), prior (DataModel class), data loading (DataModel class constructor), and proposal (Chain class) functions. This way the program will do what you want to do.
* Look at the examples I give in the main.cpp file - these should show you how to get started
* I'm experimenting with this library, and I'm no expert. Use it at your own risk, and please (it's in the license!) contact me for a citation before using this in a publication.

##License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. The following restriction to the GNU GPL applies: contact the author of this program (wdpearse@umn.edu) for a suitable citation when using the code.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
