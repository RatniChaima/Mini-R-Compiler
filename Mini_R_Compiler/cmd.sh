
bison -d syntaxique.y

flex lexical.l

gcc lex.yy.c syntaxique.tab.c -ll -o projet

./projet
