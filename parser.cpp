

class ExprAST {
public:
    virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST {
    double val;

public:
    NumberExprAST(double Val) : Val(Val) {}
};

class VariableExprAST : public ExprAST {
private:
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
};

class BinaryExprAST : public ExprAST {
private:
    char Op; 
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op,
                  std::unique_ptr<ExprAST> LHS,
                  std::unique_ptr<ExprAST> RHS)
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {};
}

class CallExprAST : public ExprAST {
private:
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(std::move(Args)) {}
};

class PrototypeAST {
private:
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {}
    const std::string &getName() const { return Name; }
};

class FunctionAST {
private:
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

static int CurTok;
static int getNextToken() {
    return CurTok = gettok();
}

std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "Error:%s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}