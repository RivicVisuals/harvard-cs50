from cs50 import get_string
import re

s = get_string("Text: ")

# regex tryouts on https://regexr.com/
# count letters, matches all all alphanumeric characters
letterCount = len(re.findall("\w", s))
# count words, matches all connected alphanumeric characters, ignoring hyphens
wordCount = len(re.findall("(\w(-|')?)+", s))
# count sentences
sentenceCount = len(re.findall("\.|\?|\!", s))

print(letterCount)
print(wordCount)
print(sentenceCount)

# calculate readability level
avgLetters = letterCount * 100 / wordCount
avgSentences = sentenceCount * 100 / wordCount
indexReadability = 0.0588 * avgLetters - 0.296 * avgSentences - 15.8
gradeLevel = round(indexReadability)

if gradeLevel < 1:
    print("Before Grade 1")
elif gradeLevel >= 16:
    print("Grade 16+")
else:
    print(f"Grade {gradeLevel}")