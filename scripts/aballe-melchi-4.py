def getTupleLength (tpl):
	print ("Get tuple length")
	print ("Tuple contains ", tpl)
	return len(tpl)

def getTupleMaxValue (tpl):
	print ("Get max value in tuple")
	print ("Tuple contains ", tpl)
	return max(tpl)


print ("Bind a new tuple")
a = []
tpllen = int(input("Length of Tuple: "))
for i in range(tpllen):
	inpt1 = input("Enter Value: ")
	a.append(inpt1)
print(" ")
a = tuple(a)

print("Get the Length of the given tuple")
print("Output: ", getTupleLength(a))
print(" ")
print("Get the maximum value in tuple")
print("Output: ", getTupleMaxValue(a))
print(" ")