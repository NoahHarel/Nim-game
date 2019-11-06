# Base-Converter
HW1 in SW Project class. First project in C.  The program receives a number in base 𝑎 and converts it to base 𝑏.


The behavior of the main function is as follows:

1- The program receives an integer representing base 𝑎 (hint: simply use scanf with "%d"). The
program asks the user to input the base by printing the following message:
“Please enter the number's base:\n”

a. If the user enters a base 𝑎 ∉ [2,16] then the program prints the following error and
terminates: “Invalid input base\n”

2- Afterwards, the program receives an integer representing base 𝑏 (in the same manner). The
program asks the user to input the base by printing the following message: “Please enter
the desired base:\n”

a. If the user enters a base 𝑏 ∉ [2,16] then the program prints the following error and
terminates: “Invalid desired base\n”

3- The program will ask the user to enter a number in base 𝑎 by printing the following message:
“Please enter a number in base <a>:\n”
Where <a> is the value of the base 𝑎.
For example if 𝑎 = 3 then the message is: “Please enter a number in base 3:\n”

a. The user will input the number as a series of chars. If any character is invalid (i.e., a
character that is not in base 𝑎) then the program prints the following error and
terminates: “Invalid number!\n”
b. Use getchar() to read the user input for the number in base a.
4- Finally, the program converts the number from base 𝑎 to base 𝑏, prints the following
message: “The result is: <res>\n”, and terminate.
Where <res> is the number received in (section 3) printed in base 𝑏.


Assumptions and requirements:

• The input number ends when a whitespace or EOF character is received. Ignore any
characters beyond the input number (i.e., 4th input and beyond).

• For bases beyond 10, support both lowercase and uppercase letters.

• The input and output numbers do not exceed 1024 characters (including '\0'). You may also
assume all valid input numbers can be represented with an int.

• All input values are separated by any number and types of whitespace characters. You may
not assume anything regarding the validity of any of the 3 input values provided by the user,
they may even not be provided! Valid numbers are integers only, no floating numbers.

• If at any point there was an error while reading from the standard input then the program
prints the following error and terminates: “An error occurred!\n”

• Any further questions should be answered by using the supplied executable.

• Strange behavior may be observed due to using scanf("%d",…), and that is expected and OK!
For example, try executing with an input base of "7.3", "11x", etc
