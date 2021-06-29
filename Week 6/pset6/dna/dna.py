
# DNA matching program

import sys
import csv
import re


def main():
    # verifying number of arguments
    if len(sys.argv) != 3:
        print("Missing Database or Sequence")
        sys.exit()

    # names of each argument
    database = sys.argv[1]
    sequence = sys.argv[2]

    # opening the database
    with open(database, "r") as csv_file:
        reader = csv.DictReader(csv_file)
        db = list(reader)

    # open sequence and removing new line 
    with open(sequence, "r") as txt_file:
        sq = txt_file.readline().rstrip("\n")
    
    # counting each sequence
    AGATC = count("AGATC", sq)
    TTTTTTCT = count("TTTTTTCT", sq)
    AATG = count("AATG", sq)
    TCTAG = count("TCTAG", sq)
    GATA = count("GATA", sq)
    TATC = count("TATC", sq)
    GAAA = count("GAAA", sq)
    TCTG = count("TCTG", sq)

    # finding matches
    if database == "databases/small.csv":
        for i in range(len(db)):
            if all([db[i]["AGATC"] == str(AGATC), db[i]["AATG"] == str(AATG), db[i]["TATC"] == str(TATC)]):
                name = db[i]["name"]
                break
            else:
                name = "No match"
    else:
        for i in range(len(db)):
            if all([db[i]["AGATC"] == str(AGATC), db[i]["TTTTTTCT"] == str(TTTTTTCT), db[i]["TCTAG"] == str(TCTAG), db[i]["AATG"] == str(AATG),
                    db[i]["GATA"] == str(GATA), db[i]["TATC"] == str(TATC), db[i]["GAAA"] == str(GAAA), db[i]["TCTG"] == str(TCTG)]):
                name = db[i]["name"]
                break
            else:
                name = "No match"
    print(name)


# Count the number of STR

def count(c, s):
    p = rf'({c})\1*'
    pattern = re.compile(p)
    match = [match for match in pattern.finditer(s)]
    max = 0
    for i in range(len(match)):
        if match[i].group().count(c) > max:
            max = match[i].group().count(c)
    return max


main()
