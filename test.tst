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

long b = 1
if(b == 0)
	puts("HeyLow")
else
	puts("HeyNo")
end
printf("Enter something... : ) ")
cstring inp = gets("          ")
puts("")
printf("You entered: %s", inp)

class heythere
	long heyboy
	int boyman
end

heythere boy
