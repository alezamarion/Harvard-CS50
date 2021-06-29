# mario in python
from cs50 import get_int


def main():
    n = get_number()

    # loop
    for i in range(n):
        print(" "*(n-i-1), end="")
        print("#"*(i+1), end="")
        # print newline
        print()

# getting height


def get_number():
    n = 0
    while n < 1 or n > 8:
        n = get_int("Height: ")
    if n >= 1 or n <= 8:
        return n


main()