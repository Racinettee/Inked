proto puts(cstring str) as int
proto gets(cstring str) as cstring
proto printf(cstring str) as int

cstring hello = "Hello"

function greet
	long ey = 4	
	printf("Hey %i %i %i %i", 1,2,3, ey)
end


greet()
puts("")
if(1 == 0)
	puts("HeyLow")
end
printf("Enter something... : ) ")
cstring inp = gets("          ")
puts("")
printf("You entered: %s", inp)

class heythere
	long heyboy
end

heythere boy
