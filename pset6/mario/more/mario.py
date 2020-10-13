from cs50 import get_int


def main():
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            break

    draw(height)


# draws a pyramid with the given height
def draw(height):
    for i in range(height):
        # first print enough spaces to make a left aligned pyramid, concatenate two spaces and the right half
        print(" " * (height - i - 1) + "#" * (i + 1) + "  " + "#" * (i + 1))


main()