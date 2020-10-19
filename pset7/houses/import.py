import csv
from sys import exit, argv
import cs50

# check argv
if len(argv) != 2:
    print("Usage: python import.py <input.csv>")
    exit(1)

# reset database
open("students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

# create tables
db.execute("CREATE TABLE students (id INTEGER PRIMARY KEY AUTOINCREMENT, first VARCHAR(255), middle VARCHAR(255), last VARCHAR(255), house VARCHAR(10), birth INTEGER);")

# read in from given csv file
with open(argv[1], "r") as fin:
    reader = csv.DictReader(fin)

    # extract infos from csv line
    for line in reader:
        name = line["name"].split()
        house = line["house"]
        birth = line["birth"]

        # if a person has no middle name, insert it as NULL
        if len(name) < 3:
            name.insert(1, None)

        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                   name[0], name[1], name[2], house, birth)
