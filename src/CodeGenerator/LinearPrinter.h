#include <iostream>
#include <string>
#include <sstream>
#include "Linear.h"

namespace Linear {

class Program;
class Method;

class Operand;
    class Literal;
    class Location;
        class Var;
        class Arr;


class Instr;

    class Statement;
        class Binary;
        class Unary;
        class Assign;

    class Helper;
        class Push_Scope;
        class Pop_Scope;
        class Declare;
    
    class Label;
    class Method_Call;
    class Return;
    
    class Jump;
        class J_Cond;
        class J_Uncond;
        
class PrettyPrinter : public Visitor {
private:
    int indentLevel = 0;
    std::ostream& out;
    
    std::string getIndent() {
        return std::string(indentLevel * 2, ' ');
    }
    
    void indent() { indentLevel++; }
    void dedent() { if (indentLevel > 0) indentLevel--; }
    
    std::string typeToString(Type type) {
        switch (type) {
            case Type::Int: return "int";
            case Type::Long: return "long";
            case Type::Void: return "void";
            default: return "unknown";
        }
    }

public:
    PrettyPrinter(std::ostream& out = std::cout) : out(out) {}
    
    void visit(Program& program) override {
        out << "Program {\n";
        indent();
        
        if (!program.globals.empty()) {
            out << getIndent() << "Globals:\n";
            indent();
            for (auto& global : program.globals) {
                global->accept(*this);
            }
            dedent();
        }
        
        if (!program.methods.empty()) {
            out << getIndent() << "Methods:\n";
            indent();
            for (auto& method : program.methods) {
                method->accept(*this);
            }
            dedent();
        }
        
        dedent();
        out << "}\n";
    }

    void visit(Method& method) override {
        out << getIndent() << typeToString(method.type) << " " << method.id << "(";
        
        for (size_t i = 0; i < method.params.size(); ++i) {
            if (i > 0) out << ", ";
            out << typeToString(method.params[i]->type) << " " << method.params[i]->id;
        }
        
        out << ") {\n";
        indent();
        
        for (auto& instr : method.instrs) {
            instr->accept(*this);
        }
        
        dedent();
        out << getIndent() << "}\n";
    }

    void visit(Operand& operand) override {
        out << typeToString(operand.type) << " " << operand.id;
    }

    void visit(Literal& literal) override {
        if (literal.is_string) {
            out << "\"" << literal.id << "\"";
        } else {
            out << literal.id;
        }
    }

    void visit(Location& location) override {
        out << location.id;
    }

    void visit(Var& var) override {
        if (var.is_array_var) {
            out << var.id << "[]";
        } else {
            out << var.id;
        }
    }

    void visit(Arr& arr) override {
        out << arr.id << "[";
        arr.index->accept(*this);
        out << "]";
    }

    void visit(Instr& instr) override {
        out << getIndent() << "// Unknown instruction\n";
    }

    void visit(Statement& statement) override {
        out << getIndent();
        statement.dist->accept(*this);
        out << " = ";
        
        for (size_t i = 0; i < statement.operands.size(); ++i) {
            if (i > 0) out << ", ";
            statement.operands[i]->accept(*this);
        }
        
        out << ";\n";
    }

    void visit(Binary& binary) override {
        out << getIndent();
        binary.dist->accept(*this);
        out << " = ";
        
        if (binary.operands.size() >= 2) {
            binary.operands[0]->accept(*this);
            
            // Note: Binary::Op enum is empty in the provided code
            // You'll need to implement proper op string conversion
            switch (binary.op) {
                // Add cases when Op enum is populated
                default: out << " <op> ";
            }
            
            binary.operands[1]->accept(*this);
        }
        
        out << ";\n";
    }

    void visit(Unary& unary) override {
        out << getIndent();
        unary.dist->accept(*this);
        out << " = ";
        
        // Note: Unary::Op enum is empty in the provided code
        // You'll need to implement proper op string conversion
        switch (unary.op) {
            // Add cases when Op enum is populated
            default: out << "<op> ";
        }
        
        if (!unary.operands.empty()) {
            unary.operands[0]->accept(*this);
        }
        
        out << ";\n";
    }

    void visit(Assign& assign) override {
        out << getIndent();
        assign.dist->accept(*this);
        out << " = ";
        
        if (!assign.operands.empty()) {
            assign.operands[0]->accept(*this);
        }
        
        out << ";\n";
    }

    void visit(Helper& helper) override {
        out << getIndent() << "// Helper instruction\n";
    }

    void visit(Push_Scope& pushScope) override {
        out << getIndent() << "push_scope;\n";
    }

    void visit(Pop_Scope& popScope) override {
        out << getIndent() << "pop_scope;\n";
    }

    void visit(Declare& declare) override {
        out << getIndent() << "declare ";
        declare.location->accept(*this);
        out << ";\n";
    }

    void visit(Label& label) override {
        // Labels are typically not indented in assembly-like code
        dedent(); 
        out << label.label << ":\n";
        indent();
    }
    
    void visit(Method_Call& methodCall) override {
        // out << getIndent();
        
        // if (methodCall.return_location) {
        //     methodCall.return_location->accept(*this);
        //     out << " = ";
        // }
        
        // out << methodCall.id << "(";
        
        // for (size_t i = 0; i < methodCall.args.size(); ++i) {
        //     if (i > 0) out << ", ";
        //     methodCall.args[i]->accept(*this);
        // }
        
        // out << ");\n";
    }

    void visit(Return& ret) override {
        out << getIndent() << "return";
        
        if (ret.return_value) {
            out << " ";
            ret.return_value->accept(*this);
        }
        
        out << ";\n";
    }

    void visit(Jump& jump) override {
        out << getIndent() << "goto " << jump.label << ";\n";
    }

    void visit(J_Cond& jCond) override {
        out << getIndent() << "if (";
        jCond.condition->accept(*this);
        out << ") goto " << jCond.label << ";\n";
    }

    void visit(J_Uncond& jUncond) override {
        // out << getIndent() << "goto " << jUncond.label << ";\n";
    }
};

} // namespace Linear