parser grammar DecafParser;

options {
    tokenVocab=DecafLexer;
}

program: import_decl* field_decl* method_decl*;

import_decl: IMPORT ID SEMI;

field_decl: type ( ID | array_field_decl ) ( COMMA ( ID | array_field_decl ) )* SEMI ;

array_field_decl: ID SOP INT_LITERAL SCL;

method_decl: (type | VOID) ID POP ( (type ID) ( COMMA type ID )* )? PCL block;


block: COP field_decl* statement* CCL;

type: INT | LONG | BOOL;

statement: location assign_expr SEMI
         | method_call SEMI
         | IF POP expr PCL block (ELSE block)?
         | FOR POP ID ASSIGN expr SEMI expr SEMI for_update PCL block
         | WHILE POP expr PCL block
         | RETURN (expr)? SEMI
         | BREAK SEMI
         | CONTINUE SEMI;

for_update: location assign_expr ;

assign_expr: assign_op expr
           | increment;

assign_op: ASSIGN
         | PLUS_ASSIGN
         | MINUS_ASSIGN
         | MUL_ASSIGN
         | DIV_ASSIGN
         | MOD_ASSIGN;

increment: INCREMENT
         | DECREMENT;

method_call: method_name POP (expr (COMMA expr)* )? PCL
            |method_name POP (extern_arg (COMMA extern_arg)* )? PCL;


method_name: ID;

location: ID
        | ID SOP expr SCL;

expr: location
    | method_call
    | literal
    | INT POP expr PCL
    | LONG POP expr PCL
    | LEN POP ID PCL
    | expr bin_op expr
    | MINUS expr
    | NOT expr
    | POP expr PCL;

extern_arg: expr | STRING_LITERAL ;

bin_op: arith_op
      | rel_op
      | eq_op
      | cond_op;

arith_op: PLUS
        | MINUS
        | STAR
        | DIV
        | MOD;

rel_op: LT
      | GT
      | LE
      | GE;

eq_op: EQ
     | NEQ;

cond_op: AND
       | OR;

literal: (MINUS)? INT_LITERAL
       | LONG_LITERAL
       | CHAR_LITERAL
       | BOOL_LITERAL;

alpha_num: ALPHA | DIGIT ;
