# readability in python


from cs50 import get_string

# getting string from user
texto = get_string("Text: ")

letters = 0
words = 1
sentences = 0

# finding numbers of letters, words and sentences
for caractere in texto:
    if (caractere >= 'a' and caractere <= 'z') or (caractere >= 'A' and caractere <= 'Z'):
        letters += 1
    elif caractere == " ":
        words += 1
    elif (caractere == "!") or (caractere == ".") or (caractere == "?"):
        sentences += 1

# calculating index
L = float((letters * 100) / words)

S = float((sentences * 100) / words)

index = float(round(0.0588 * L - 0.296 * S - 15.8))

# printing results
if index < 1:
    print("Before Grade 1")
elif index > 15:
    print("Grade 16+")
else:
    print("Grade", int(index))
