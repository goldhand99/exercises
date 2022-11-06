



def get_final_line(arg): 

    with open(arg) as f:
        index = 0
        lines = f.readlines()
        lineCount = len(lines)
        for line in lines:
            index += 1 
            if index == lineCount:
                return line.format()


# Write a wordcount function that mimics the wc Unix command. The function will
# take a filename as input and will print four lines of output: Number of characters
# (including whitespace), Number of words (separated by whitespace), Number of
# # lines and the number of unique words (case sensitive)

from ast import Not
from genericpath import isdir, isfile
import os

def wordcount(arg): 
    fd = os.stat(arg)
    number_char = fd.st_size
    number_lines = 0

    with open(arg) as f: 
        lines = f.readlines()
        number_lines = len(lines)
        number_words = 0
        dict_unique_words = {}
        print(lines)
        for line in lines: 
           number_words = number_words + len(line.split())
           for word in line.split(): 
               dict_unique_words[word] = 0

    print("done, %d,%d,%d,%d",number_char,number_lines,number_words,len(dict_unique_words.keys()))



# find_longest_word : takes a filename as an argument and returns the
# longest word found in the file.
# b. find_all_longest_words : takes a directory name and returns a dict in which
# the keys are filenames and the values are the longest words from each file.



def find_longest_word(arg): 
    words_dict = {}
    with open(arg) as f:
        lines = f.readlines()
        for line in lines: 
            words = line.split()
            for word in words:
                words_dict[len(word)] = word
    sorted_keys = sorted(words_dict.keys(),reverse=True)
    return words_dict[sorted_keys[0]]
    
import os 
class Words: 
    words = {}
    def init(self):
        self.words = {}



def find_all_longest_word(arg,words): 
    
    length = 0
    print(isdir(arg))
    if isdir(arg):
        directories = os.listdir(arg)
        length = len(directories)
    if length:
        for file in directories:
            find_all_longest_word(file,words)
    elif isfile(arg):
        words[arg] = find_longest_word(arg)


words_word = Words()
words = words_word.words

find_all_longest_word('txtcase',words)





# 1. Write a function, csv_to_csv, that takes two filenames as arguments:
# a. the first is a csv-style file to read from
# b. the second is the name of a file in which to write the outputs
# 2. Change the separator of the csv file


def csv_to_csv (arg1, arg2): 
    words = []
    with open(arg1,arg2) as (f,f1): 
        lines = f.readlines()

        index = 0

        if lines.len() - 4 >=0:
           for line in lines: 
                lines += 4
                index += 4
                f1.write(line[0])
                f1.write(line[1])
                f1.write(line[2])
                f1.write(line[3])
                if lines.len() - index < 4: 
                    break
        while index < lines.len():
            for line in lines: 
                f1.write(line) 
                index += 1


        for line in lines:
             line.replace(',',"")
             f1.write(line)
                
    

