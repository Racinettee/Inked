Inked
=====

a compiler

made following this tutorial: http://gnuu.org/2009/09/18/writing-your-own-toy-compiler/

modified for llvm 3

the syntax looks a little like ruby with some inspiration from c

an example:

  function puts(cstring str) as int end

  function greet() as int<br>
    puts("hello")<br>
	  puts("Friend")<br>
  end<br>

  greet()<br>
  greet()<br>
  greet()
