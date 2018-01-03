# Xml Generator
Future support includes: conversion from xml to javadocs.
Objective: Parse xml comments from source files and store them in text files as data sets. From these data sets, the user can convert to other documentation formats.

Runtime is o(n) where n is the number of lines in the source file.

Changelog: 
	- Fixed issue where the file would be opened twice and would cause every other function's comments to be skipped and not stored.
	- Added support for return types, function names, return summaries, parameter names and descriptions.
	- Added a print function that prints the contents of the function_structure.
	- Removed unused code.
	- Changed search algorithm to be more efficient.
	
Issues: First function's comments not stored properly.
logical issue with parseCommentBlocks function.
Return type is a problem with the function names.

Parameter summary for multiple parameters.

Paramater Description:

///<param name  = "patt"> The string pattern which we wish to remove from the beginning of the file.



TO do: support for constructors.
	Remove the /// from strings.
	Tagging from function declaration. Need to get content from between the () to determine # of parameters and their types.
	block comment ignorance.
