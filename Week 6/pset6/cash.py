
# cash in python

from cs50 import get_float


# getting change
while True:
    x = get_float("Change owed: ")
    if x > 0:
        break

# rouding it
x = round(x * 100)

# initializing number of coins
coins = 0

# getting number of coins
while x > 0:
    if x >= 25:
        x -= 25
        coins += 1
    elif x >= 10:
        x -= 10
        coins += 1
    elif x >= 5:
        x -= 5
        coins += 1
    elif x >= 1:
        x -= 1
        coins += 1

# printing result
print(coins)
