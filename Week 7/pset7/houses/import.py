

from cs50 import SQL
from sys import argv, exit
import csv

# spliting names


def partition_name(full_name):
    names = full_name.split()
    return names if len(names) >= 3 else [names[0], None, names[1]]


# checking correct usage
if len(argv) != 2:
    print("Incorret Number of Arguments")
    exit(1)

db = SQL("sqlite:///students.db")

# importing csv file
imported_csv = argv[1]
with open(imported_csv) as csv_file:
    reader = csv.DictReader(csv_file)
    for row in reader:
        names = partition_name(row["name"])
        db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                   names[0], names[1], names[2], row["house"], row["birth"])