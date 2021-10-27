def incrementWhileLoop (a):
	print ("While Loop increment values")
	print ("Condition while a != b = (a + 5)")
	b = a + 5
	while (a != b):
		print (a)
		a += 1

def decrementWhileLoop (a):
	print ("While Loop decrement values")
	print ("Condition while a != b = (a - 5)")
	b = a - 5
	while (a != b):
		print(a)
		a -= 1

print (" ")
print ("Function for increment and decrement loops")
inpt1 = int(input("Enter a number: "))
incrementWhileLoop(inpt1)
print (" ")
decrementWhileLoop(inpt1)
print (" ")