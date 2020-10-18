import csv
from sys import exit, argv

# check argv
if len(argv) != 2:
  print("Usage: python import.py <input.csv>")
  exit(1)

with open(argv[1], "r") as fin:
  reader = csv.DictReader(fin)

  for line in reader:
    print(line)
    