lexer grammar DecafLexer;



IMPORT : 'import' ;
SEMI: ';';
COMMA: ',';
SOP : '[' ;
SCL : ']' ;
VOID: 'void' ;
POP: '(' ;
PCL: ')' ;
COP: '{' ;
CCL: '}' ;
INT: 'int';
LONG: 'long';
BOOL: 'bool';
IF: 'if' ;
ELSE: 'else' ;
FOR: 'for' ;
ASSIGN: '=';
WHILE: 'while';
RETURN: 'return';
BREAK: 'break';
CONTINUE: 'continue';
PLUS_ASSIGN: '+=';
MINUS_ASSIGN: '-=';
MUL_ASSIGN: '*=';
DIV_ASSIGN: '/=';
MOD_ASSIGN: '%=';
INCREMENT: '++';
DECREMENT: '--';
MINUS: '-';
NOT: '!';
LEN: 'len';
PLUS: '+';
STAR: '*';
DIV: '/';
MOD: '%';
LT: '<';
GT: '>';
LE: '<=';
GE: '>=';
EQ: '==';
NEQ: '!=';
AND: '&&';
OR: '||';


fragment ALPHA : [a-zA-Z_];
fragment DIGIT : [0-9] ;
fragment HEX_DIGIT : [0-9a-fA-F] ;
fragment CHAR : PRINTABLE_CHAR | ESCAPE_SEQUENCE;



// Printable characters (ASCII 32 to 126, excluding ', ", and \)
fragment PRINTABLE_CHAR : ' '..'!' | '#'..'&' | '('..'[' | ']'..'~' ;
// Supported escape sequences
fragment ESCAPE_SEQUENCE : '\\' [tnrf"'\\];  
CHAR_LITERAL   : '\'' CHAR '\'';
STRING_LITERAL : '"' (CHAR)* '"';      
BOOL_LITERAL  : 'true' | 'false';
INT_LITERAL : DECIMAL_LITERAL | HEX_LITERAL;
LONG_LITERAL  : (DECIMAL_LITERAL | HEX_LITERAL) 'L';
ID : ALPHA (ALPHA | DIGIT)* ;
DECIMAL_LITERAL : DIGIT+; 
HEX_LITERAL   : '0x' HEX_DIGIT+;
// Whitespace and Comments
WS            : [ \t\n\r\f]+ -> skip;        // Skip spaces, tabs, line breaks, form feeds
COMMENT       : '/*' .*? '*/' -> skip;       // Block comments
LINE_COMMENT  : '//' ~[\r\n]* -> skip;       // Line comments

ERROR : . ; // Must keep as last rule in grammar!
