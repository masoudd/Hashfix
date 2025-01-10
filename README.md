Hashfix
=======

This program takes a file, a hashlist and a url.

Checks the file against the hashlist and if a chunk of the file does not match the hash in the hashlist, it downloads that chunk from the url and writes it in the file.


Hashlist
=======

This program generates a hash list from a file.
a hash list is a text file. line N in this file is the hash of the Nth chunk of that file.
size of each chunk is defined in chunk_len variable.
