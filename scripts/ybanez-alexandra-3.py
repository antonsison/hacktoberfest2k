terms = int(input("How many terms? "))

num1, num2 = 0, 1
count = 0

if terms <= 0:
   print("Please enter a positive integer")
elif terms == 1:
   print("Fibonacci sequence up to",terms,":")
   print(num1)
else:
   print("Fibonacci sequence:")
   while count < terms:
       print(num1)
       nth = num1 + n2

       num1 = num2
       num2 = nth
       count += 1