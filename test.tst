proto puts(cstring str) as int
proto gets(cstring str) as cstring

function greet() as long
	puts("hello")
	puts("Friend")
	cstring inp = gets("               ")
	puts("You entered:")
	puts(inp)
end

greet()
greet()
greet()
