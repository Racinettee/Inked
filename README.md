Inked
=====

A compiler. It is made with flex, bison, and llvm 3.x.

It is original based off of this tutorial:
http://gnuu.org/2009/09/18/writing-your-own-toy-compiler/

The syntax takes inspiration from a few languages.

Here is a current working example:

proto puts(cstring str) as int<br>
proto gets(cstring str) as cstring<br>
proto printf(cstring str) as int<br>

cstring hello = "Hello"<br>

function greet<br>
&nbsp;&nbsp;long ey = 4	<br>
&nbsp;&nbsp;printf("Hey %i %i %i %i", 1,2,3, ey)<br>
end<br>

greet()<br>
puts("")<br>
if(1 == 0)<br>
&nbsp;&nbsp;puts("HeyLow")<br>
end<br>
printf("Enter something... : ) ")<br>
cstring inp = gets("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")<br>
puts("")<br>
printf("You entered: %s", inp)<br>

class heythere<br>
&nbsp;&nbsp;long heyboy<br>
end<br>

heythere boy
