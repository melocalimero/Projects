#You start from an empty string and process an ordered series of operations. Each operation can be:
#   add c: appends character c at the end of the string
#   del: removes last character from the string (if any)
    # swap a b: swaps character a with character b, meaning that all occurrences of character a are replaced
    # with character b, and all occurrences of character b are replaced with character a
    # rot x: applies rot-x to the current string

# Notes:
# each operation is only applied to the string as it is before the operation itself, and it does not influence
# the characters added after the operation is executed
#   The string below represents the alphabet of allowed characters:
#   abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789
# when performing rot operations, characters are rotated following the order in the alphabet, rotating to
# the right.
# For example, performing rot-6 on character y outputs character E (because E is 6 places to the
# right of y in the alphabet), and performing rot-3 on character 8 outputs character b (because after the
# end the alphabet restarts from a)

import sys
import pprint

#leggo l'input
input_data = sys.stdin.read().split()

iterator = iter(input_data)
message = []
final_message = ""
try:
    num_op = int(next(iterator))
    for _ in range(num_op):
        operation = next(iterator)
        if(operation == "del"):
            if(message):
                message.pop()
            continue
        
        if operation == "add" :
            ch = next(iterator)
            message.append(ch)
            continue

        if operation == "swap" :
            ch = next(iterator)
            ch1 = next(iterator)
            for i in range(len(message)):
                if(message[i] == ch):
                    message.remove(message[i])
                    message.insert(i, ch1)
                elif(message[i] == ch1):
                    message.remove(message[i])
                    message.insert(i, ch)
            continue
        
        #operation = rot
        shift = int(next(iterator))
        alph = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
        rotated_list = []
        for c in message:
            if(c in alph):
                c_index = alph.index(c)
                new_index = (c_index + shift)%len(alph)
                rotated_list.append(alph[new_index])
        message = rotated_list
        


    for c in message:
        final_message = final_message + c
    print (final_message.strip())
except StopIteration:
    pass