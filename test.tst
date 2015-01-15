proto puts(cstring str) as int
proto gets(cstring str) as cstring
proto printf(cstring str) as int

function greet(int param)
	puts("attempting to assign param")
	int x = param
	printf("%i", x)
	puts("hello")
end

greet(2)
greet(4)
greet(6)


