# Xml Generator
Future support includes: conversion from xml to javadocs.
Objective: Parse xml comments from source files and store them in text files as data sets. From these data sets, the user can convert to other documentation formats.

Limitations:
-If no comments are given for a function, the parser ignores that function and it is not implemented into the data set. 

Current Bugs:
If a function does not have a return type, such as a constructor, the function name is improperly parsed.
