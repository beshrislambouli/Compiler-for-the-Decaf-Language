parser grammar DecafParser;

options {
    tokenVocab=DecafLexer;
}

program: import_decl* field_decl* method_decl* EOF;

import_decl: IMPORT ID SEMI;

field_decl: field_type field ( COMMA field )* SEMI ;

field: ID #Id_Field_Decl
     | ID SOP INT_LITERAL SCL #Array_Field_Decl
     ;

method_decl: method_type ID POP ( (parameter) ( COMMA parameter )* )? PCL block;

parameter: field_type ID;

block: COP field_decl* statement* CCL;

field_type: INT | LONG | BOOL;
method_type: VOID | INT | LONG | BOOL;

statement: location assign_op expr SEMI #Location_Assign_Op
         | location increment SEMI #Location_Incr
         | ID POP (extern_arg (COMMA extern_arg)* )? PCL SEMI #Method_Call_Stmt
         | IF POP expr PCL block (ELSE block)? #If_Else_Stmt
         | FOR POP ID ASSIGN expr SEMI expr SEMI for_update PCL block #For_Stmt
         | WHILE POP expr PCL block #While_Stmt
         | RETURN (expr)? SEMI #Return_Stmt
         | BREAK SEMI #Break_Stmt
         | CONTINUE SEMI #Continue_Stmt;

for_update: location assign_op expr #For_Upd_Assign_Op
          | location increment #For_Upd_Incr
          ;
          
// assign_expr: assign_op expr
//           | increment;

assign_op: ASSIGN
         | PLUS_ASSIGN
         | MINUS_ASSIGN
         | MUL_ASSIGN
         | DIV_ASSIGN
         | MOD_ASSIGN;

increment: INCREMENT
         | DECREMENT;

// method_call: method_name POP (expr (COMMA expr)* )? PCL
//            |method_name POP (extern_arg (COMMA extern_arg)* )? PCL;


// method_name: ID;

location: ID #Loc_Var
        | ID SOP expr SCL #Loc_Array;

// expr: location
//    | method_call
//    | literal
//    | INT POP expr PCL
//    | LONG POP expr PCL
//    | LEN POP ID PCL
//    | expr bin_op expr
//    | MINUS expr
//    | NOT expr
//    | POP expr PCL;

expr: MINUS expr #Minus_Expr
    | NOT expr #Not_Expr
    | INT POP expr PCL #INT_Expr
    | LONG POP expr PCL #LONG_Expr
    | expr bin_mul_op expr #Mul_Op_Expr
    | expr bin_add_op expr #Add_Op_Expr
    | expr rel_op expr #Rel_Op_Expr
    | expr eq_op expr #Eq_Op_Expr
    | expr AND expr #And_Op_Expr
    | expr OR expr #Or_Op_Expr
    | location #Loc_Expr
    | ID POP (extern_arg (COMMA extern_arg)* )? PCL #Method_Call_Expr
    | literal #Literal_Expr
    | LEN POP ID PCL #Len_Expr
    | POP expr PCL #Paren_Expr
    ;

extern_arg: expr #Expr_Arg
          | STRING_LITERAL #String_Arg;

bin_mul_op: STAR
          | DIV
          | MOD ;
          
bin_add_op: PLUS
          | MINUS;

// bin_op: arith_op
//      | rel_op
//      | eq_op
//      | cond_op;

// arith_op: PLUS
//        | MINUS
//        | STAR
//        | DIV
//        | MOD;

rel_op: LT
      | GT
      | LE
      | GE;

eq_op: EQ
     | NEQ;

// cond_op: AND
//       | OR;

literal: INT_LITERAL #Int_Lit
       | LONG_LITERAL #Long_Lit
       | CHAR_LITERAL #Char_Lit
       | BOOL_LITERAL #Bool_Lit;
