from cs50 import get_string
from sys import exit
import re
import math


# get cc number from user
number = get_string("Number: ")

# check for non-numerical characters
if re.search("\D", number):
    print("INVALID")
    exit(0)


# count digits courtesy of https://stackoverflow.com/a/2189827
digits = int(math.log10(int(number))) + 1


# check for length, checksum, starting digits
if digits > 13 or digits < 16:
    checksum = 0

    # use a reversed loop to start at the rightmost digit
    for n in reversed(range(digits)):

        # reverse index
        if (digits - n) % 2 == 0:
            # add digit * 2 or the sum of the product, which is going to be product - 9 for [0-9]
            checksum += int(number[n]) * 2 if int(number[n]) < 5 else int(number[n]) * 2 - 9
        else:
            checksum += int(number[n])

    if checksum % 10 == 0:
        if re.search("^3(4|7)", number):
            print("AMEX")
            exit(0)
        elif re.search("^4", number):
            print("VISA")
            exit(0)
        elif re.search("^5([1-5])", number):
            print("MASTERCARD")
            exit(0)

# error catcher
print("INVALID")
exit(0)