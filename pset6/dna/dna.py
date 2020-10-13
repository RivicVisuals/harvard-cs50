from sys import argv, exit
import re
import csv

# check for argv
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(0)


# open sequence.txt
with open(argv[2], "r") as sequence:
    seq = sequence.read()

# open data.csv
# hold content in memory
data = open(argv[1], "r")
db = csv.DictReader(data)


# extract sequences from header in db, skip first one as it is the name
sequenceCount = {}
for s in db.fieldnames[1:]:
    # matches all sequences of s as one
    matches = re.findall("(?:" + s + ")+", seq)
    # gets length of longest match by dividing character count by length of s
    if len(matches) > 0:
        sequenceCount[s] = int(len(max(matches)) / len(s))


# compare counts of sequences to data.csv
for row in db:
    # print matches
    # convert to lists before comparing; cut off first value from row, as it is the name
    rowValues = list(row.values())[1:]
    if list(map(int, rowValues)) == list(sequenceCount.values()):
        print(row["name"])
        exit(0)


print("No match")
data.close()
exit(0)