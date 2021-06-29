

from cs50 import SQL
from sys import argv, exit

# checking correct usage
if len(argv) != 2:
    print("Incorret Number of Arguments")
    exit(1)

db = SQL("sqlite:///students.db")

# querying database
house = argv[1]
rows = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", house)

# printing result
for row in rows:
    first, middle, last, birth = row["first"], row["middle"], row["last"], row["birth"]
    if middle != None:
        print(f"{first} {middle} {last}, born {birth}")
    else:
        print(f"{first} {last}, born {birth}")