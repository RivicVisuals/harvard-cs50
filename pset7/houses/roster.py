from sys import argv, exit
import cs50

# check for correct usage
if len(argv) != 2:
    print("Usage: python roster.py <House>")
    exit(1)

house = argv[1]

# open students.db
db = cs50.SQL("sqlite:///students.db")

# select first, middle (only if exists), last name as well as birth
# filtered by House name
# ordered by last, then first name
students = db.execute(
    "SELECT first, (CASE WHEN middle NOT NULL THEN middle ELSE '' END) middle, last, birth FROM students WHERE house LIKE (?) ORDER BY last ASC, first ASC;", house)

for student in students:
    # compile results to a nice string
    name = [student["first"], student["last"]]
    if student["middle"]:
        name.insert(1, student["middle"])
    name = " ".join(name)
    year = student["birth"]

    print(f"{name}, born {year}")
