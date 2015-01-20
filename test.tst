proto puts(cstring str) as int
proto gets(cstring str) as cstring
proto printf(cstring str) as int

function greet
	long ey = 4	
	printf("Hey %i %i %i %i", 1,2,3, ey)
end

greet()
greet()
greet()
puts("")
printf("Enter something... : ) ")
cstring inp = gets("          ")
puts("")
printf("You entered: %s", inp)
