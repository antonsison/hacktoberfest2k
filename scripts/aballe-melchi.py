def getStringCharLength(a):
	print ("The Character length of ", a)
	return len(a)

def toLowerCase(a):
	print ("Convert ", a ," to lower case")
	return a.lower()


print ("Get the character length of the given string")
inpt1 = input("Enter a string: ")
print ("Output: ", getStringCharLength(inpt1))
print (" ")
print ("Convert given string to lowercase")
inpt1 = input("Enter a string: ")
print ("Output: ", toLowerCase(inpt1))
print (" ")