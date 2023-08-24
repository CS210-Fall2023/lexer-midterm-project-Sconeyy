# Lexer (Midterm Project)
## Intro
Modern translation systems often use lexical analysis to divide input into meaningful units. Once these meaningful units (or lexemes or tokens) have been derived, other components within the translation system are used to determine the relationships among the lexemes. Lexical analyzers (or lexers) are commonly used in compilers, interpreters, and other translation systems that you have often used. The act of lexical analysis is also known as scanning. For this assignment, you are to build a lexer that will successfully scan through a set of programs expressed in the CCX programming language. You have never used CCX, and that is just fine: scanning through CCX programs won't require intimate knowledge of CCX. 
* Your lexer shall be written in the C programming language.
* Your lexer will be compiled and tested using GitHub codespace (default configuration), so you should test your lexer in this environment.
* Your program must run from the command line and accept command line arguments.
* You may not use any lexer generators or other automated tools to complete this assignment. All algorithms, structures, and techniques employed in your lexer must be of your own making.
  
 A sample CCX program is below. This program simply prints the string "Hello, world" to the screen and then prints the arguments to the program if any were provided. 
 ````c
/* 
* Hello world with args. 
*/ 
procedure main(argc: integer; argv: string_vector_type) is 
begin printf("Hello, world\n"); 
loop 
argc := argc - 1; 
exit when (argc = 0);
 printf("arg[%d]: %s\n", argc, argv[argc]); 
end loop; 
end main;
````
The goal of lexical analysis is to break programs like the one above into lexemes that are eventually used by other components within the translation system to determine things like whether the program is legal and what the program does. Although the requirements and constraints imposed upon lexical analysis may vary considerably between different translation systems, the requirements for most lexers (and for this assignment) are very simple. Your lexer shall 
* open a file provided in the command line
* discover the lexemes found in the file
* classify each lexeme
* print out each lexeme and its classification to the output file named with the same name as the input file with “.lexer” as the last part of the name.

  
Your lexer shall classify each lexeme found in a given source file into one of 8 categories. These categories are: **comment**, **string**, **keyword**, **character literal**, **numeric literal**, **operator**, **identifier**, and **UNK**. The details concerning each of these categories are specified later in this document, but for now, let's look at the output for a CCX lexer that meets the requirements of this assignment. The following is the output produced by a lexer when scanning the source code presented above. 
* each lexeme and its classification are printed on a separate line.
* a single space appears between each lexeme and its classification
* the classification appears in parentheses.
````
/* 
* Hello world with args. 
*/ (comment) 
procedure (keyword) 
main (identifier) 
( (operator) 
argc (identifier) 
: (operator) 
integer (keyword) 
; (operator) 
argv (identifier) 
: (operator) 
string vector type (identifier) 
) (operator) 
is (keyword) 
begin (keyword) 
printf (identifier) 
( (operator) 
"Hello, world\n" (string) 
) (operator) 
; (operator) 
loop (keyword)
argc (identifier) 
:= (operator) 
argc (identifier) 
- (operator) 
1 (numeric literal) 
; (operator) 
exit (keyword) 
when (keyword) 
( (operator) 
argc (identifier) 
= (operator) 
0 (numeric literal) 
) (operator) 
; (operator) 
printf (identifier) 
( (operator) 
"arg[%d]: %s\n" (string) 
, (operator) 
argc (identifier) 
, (operator) 
argv (identifier) 
[ (operator) 
argc (identifier) 
] (operator) 
) (operator) 
; (operator) 
end (keyword) 
loop (keyword) 
; (operator) 
end (keyword) 
main (identifier) 
; (operator)
````
Please examine this output closely. Each lexeme must be printed on a separate line, and a single space must appear between the lexeme and its classification. The lexeme itself must start in the first column on a given line. The classification of a lexeme must appear in parentheses. No “blank” or “empty” lines can appear in the output unless they are part of a multi-line comment. Each lexeme must be printed precisely as it appears in the source file. Do not bracket the lexeme in quotes or any other characters.

## The Lexeme Categories 
As mentioned, your lexer shall classify each lexeme encountered into one of 8 categories. The details of each category follow. 
### comment 
Comments in CCX begin with /* and end with */ (C-style comments). Comments can span multiple lines. Everything encountered between (and including) the /* and */ delimiters is considered part of the comment lexeme. 
### string
Strings in CCX are literals delimited by double-quotes "like this". The double quotes are part of the lexeme. When you print a lexeme that has been classified as a string, you must print the double quotes. You may assume that your lexer will never encounter a string that is more than 256 characters long. You may assume that a string inside the string is not allowed.
### keyword 
CCX contains many keywords. Keywords are sometimes called reserved words. Keywords (like all of CCX) are case-sensitive, and may not be used as identifiers in legal programs. It is not the job of the lexer to determine whether a keyword is misused; the lexer simply classifies a particular lexeme as being a keyword. The following is the list of CCX keywords that your lexer must recognize: 
````
accessor and array begin bool case character constant else elsif end exit function
if in integer interface is loop module mutator natural null of or other out
positive procedure range return struct subtype then type when while
````
### character literal 
Character literals in CCX are literals in single quotes like this: 'x'. CCX allows character escape sequences in character literals, such as '\020' but your lexer need not support this. 
### operator 
CCX contains many operators. Some operators consist of a single character, whereas others contain multiple characters. The following is a list of the operators that your lexer must recognize. Each operator is enclosed in double quotes for the purpose of disambiguation, but these double quotes are not part of the operator: 
````
“.”   “<”   “>”   “(“   “)”   “+”   “-“   “*”   “/”   “|”   “&”   “;”   “,”   “:”
“[“   “]”  “=”   “:=”   “..”   “<<”   “>>”   “<>”   “<=”   “>=”   “**”   “!=”   “=>”
````
> **Warning:** With operators, some are prefixes of others. For example "=" is a
         prefix for "=>" and ":" is a prefix for "=". Ensure your algorithm
         finds the longest one.


### numeric literal 
CCX allows numeric literals in multiple forms. Your lexer will recognize a simplified subset of CCX numeric literals. Each numeric literal encountered by your lexer will start with a decimal digit and will contain only the following: 
 * decimal digits (0 through 9)
 * the special characters ‘.’ (only one) and ‘#’(can have multiple, like 3#5#)
 #### Example:
````
1..3 
````
shoud be classified as
````
 1 (numeric)
 .. (operator)
 3 (numeric)
````
 ````
 1.2
````
should be classified as 
````
1.2 (numeric)
````
any other character encountered will denote that the numeric literal has ended and a new lexeme has begun. 

### identifier 
Identifiers are used in programs to name entities such as variables. Every programming language has its own rules as to what constitutes a legal identifier. In CCX an identifier can be composed of letters, digits, and underscores, but must start with a letter. You may assume that your lexer will never encounter an identifier that is more than 256 characters long. 

### UNK 
This special category is set aside for lexemes that your lexer cannot classify and is intended to assist you in building and debugging your lexer. This category is composed of all lexemes that do not fit in any of the other specified categories. Your lexer will only be tested against legal CCX programs, so if the logic in your lexer is correct, you should never encounter an UNK lexeme. If, however, your lexer does encounter a lexeme that does not fit the requirements of any of the other categories, your lexer must print the offending lexeme, along with its category name in parenthesis.

## Grading 
Your lexer will be built using GitHub codespace (default). Your lexer will be tested against the following input files:
````
helloWorld.ccx
list.cci
list.ccx
complex.cci
complex.ccx
date.cci
date.ccx
natural.cci
natural.ccx
trie.cci
trie.ccx
widget.cci
widget.ccx
````
Each of these source files and the result produced by a correct lexer (.ANS) when scanning the file is provided in the repository. The output of your lexer will be compared with the correct result for each file. You MUST test your output using the command
````
diff date.cci.lexer date.cci.ANS
````
### Automated Testing
To simplify testing **test.sh** script is provided. In order to use the script, your executable should be named *lexer* and located in the same folder as **test.sh**. The input file (for example, **date.cci**), output file (for example, **date.cci.lexer**) and answer file (for example, **date.cci.ANS**) must be  in the files folder. To run the script, use the following command
````
./test.sh
````
The expected output is
````
files/complex.cci.ANS vs. files/complex.cci.lexer
You passed the test

files/date.cci.ANS vs. files/date.cci.lexer
You passed the test

files/list.cci.ANS vs. files/list.cci.lexer
You passed the test

files/natural.cci.ANS vs. files/natural.cci.lexer
You passed the test

(continue...)
````
You will need to submit a snapshot of your output from running **test.sh** script to demonstrate that you tested your program against all files.

### Additional requirements
You are required to do your own work for this assignment. Failure to comply will result in a score of 0 for this homework. Your program may not ask the user for the name of a file to scan, the number of files to scan, or anything of this sort. If your lexer does not process command-line arguments for any reason, at least 10% of the total possible points on this assignment will be deducted from your score.
You must use three file structures.
Your program will be evaluated for neatness and clarity as well as correctness. You must document your program using comments. You must use a consistent programming style which indicates that you are in control of your thoughts and the program which is being used to actualize them. Sloppy, ambiguous, convoluted, intentionally vague, undocumented, or insufficiently documented programs will be considered substandard and will be marked as such.

### Hints
Build your lexer incrementally. For example, start by reading a file and printing its context to the screen. Then build a lexer that recognizes just comments and prints the other words (group of characters separated by white space) with UNK state. Then add an ability to recognize the keywords. Once it has been tested, add the ability to recognize CCX operators. Continue in this fashion until your lexer is complete. Notice that different lexemes might be connected to each other without space, like
````
printf("Hello, world\n");
````
Don’t forget to separate them:
`````
printf (identifier)
( (operator) 
"Hello, world\n" (string) 
) (operator) 
; (operator)
`````

## Single-Developer Workflow
- [ ]  Edit your code
* Use Visual Studio Code or any text editor to write the code for the problem
- [ ] Test your code
* Compiling and executing your code to check for syntax, logic, and run-time errors.
* It is also important to identify some test cases.
- [ ] Commit your code
* Two things about committing:
1. You should commit somewhat frequently. At a minimum, commit each time you complete a question or small problem. Although I would suggest breaking your code down into subproblems. Test that it works and commit.
2. Leave informative commit messages. "Added stuff" will not help you if you are looking at your commit history in a year. A message like "Added initial version of hello-world function" will be more useful.
* Two things to do in the terminal during a commit
````
git add -A
git commit -m "YOUR COMMIT MESSAGE"
````
* `git add` is a command that tells git which files you want to record the changes to when you make your commit. For example, I made changes to `file1` and `file2` since my last commit, I can choose to only commit (take a snapshot of) the changes I made to `file1`.
* `git add -A` says to add all of the files that have changed since the last commit.
* If I just want to add `file1`, I would instead type `git add file1`.
- [ ] Push your code
      
At some point, you'll want the updated version of the assignment stored in your local computer, push back to GitHub, either so that the instructor can help with your code, or so that it can be graded. You can do this by using the command `git push`. When you press enter, you will be prompted for a username and password. This is your GitHub username and the password is the personal access token generated. (NOTE: while typing the personal access token, you will not see any letter in the terminal, this is normal, just continue typing). This should be the only time you will have to write the username and password after the `git push`, unless you are using a different computer or the personal access token expires.
````
$ git push
```` 

## Submitting Assignments
- [ ] Submit the URL of your repository and a Screenshot of the output after running the provided script on Canvas.
- [ ] It is VERY IMPORTANT that besides pushing the code to GitHub that you still submit the GitHub URL of this lab assignment in Canvas. This allows the instructor to be notified of your final submission.

> **_NOTE: By submitting your code to be graded, you are stating that your submission does not violate the Academic Integrity Policy outlined in the syllabus._**

