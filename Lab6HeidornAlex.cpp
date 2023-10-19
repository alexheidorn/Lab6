/*
* Alex Heidorn
* Created: 10-12-23
* CS2 Lab6
*  
*/

/*Results
Expression: ( 3 + 7 ) / 5 - 4 / 2 =
( 3 + 7 ) / 5 - 4 / 2 
Prefix version of expression: - / + 3 7 5 / 4 2 
Result is 0
PS C:\Users\alexh\CS2>

Expression: 7 + 2 * 2 + 3 =  
7 + 2 * 2 + 3 
Prefix version of expression: + 7 + * 2 2 3 
Result is 14
PS C:\Users\alexh\CS2>

Expression: 4 - 3 * 2 =
4 - 3 * 2 
Prefix version of expression: - 4 * 3 2 
Result is -2
PS C:\Users\alexh\CS2> 

Expression: ( 4 - 3 ) * 2 = 
( 4 - 3 ) * 2 
Prefix version of expression: * - 4 3 2 
Result is 2
PS C:\Users\alexh\CS2> 

Expression: ( 7 - 2 ) * 2 + 3 = 
( 7 - 2 ) * 2 + 3 
Prefix version of expression: + * - 7 2 2 3 
Result is 13
PS C:\Users\alexh\CS2> 

Expression: 7 - 2 * ( 2 + 3 ) = 
7 - 2 * ( 2 + 3 ) 
Prefix version of expression: - 7 * 2 + 2 3 
Result is -3
PS C:\Users\alexh\CS2> 
*/

#include <iostream>
#include <string>
#include <list>
#include <math.h>
using namespace std;
    const int NUMBER = 0;
    const int LPAREN = 1;
    const int RPAREN = 2;
    const int ADD = 3;
    const int MINUS = 4;
    const int TIMES = 5;
    const int DIVIDE = 6;
    const int POWER = 7;
    const int SEMI = 8;
    const string OPERATOR_STRINGS[] = {"NUMBER", "(", ")", "+", "-", "*", "/", "^", ";"};
    const int OPERATOR_PRECEDENCE[] = {-1, 0, 0, 1, 1, 2, 2, 3, 0};
    const bool OPERATOR_LEFT_ASSOCIATIVE[] = { false, false, false, true, true, true, true, false, false};
    class ExpressionPart {
    protected:
        int eType;
        ExpressionPart() {}
    public:
        ExpressionPart(int et) { eType = et; }
        int getEType() { return eType; }
        void setEType(int et) { eType = et; }
        virtual void print() {}
    };
    class ExpressionNumber : public ExpressionPart {
    protected:
        double number;
        ExpressionNumber() {}
    public:
        ExpressionNumber(double n) : ExpressionPart(NUMBER) {
            number = n;
        }
        ExpressionNumber(string s) {
            try {
                number = stoi(s);
                setEType(NUMBER);
            }
            catch (exception e) {
                throw "NOT A NUMBER";
            }
        }
        double getNumber() { return number; }
        void print() {
            cout << number << " ";
        }
    };
    class ExpressionOperator : public ExpressionPart {
    protected:
        ExpressionOperator() {}
    public:
        ExpressionOperator(string s) {
            int op;
            for (op = LPAREN; op <= POWER; op++) {
                if (s.compare(OPERATOR_STRINGS[op]) == 0) {
                    break;
                }
            }
            if (op > SEMI) {
                throw ("Unknown operator " + s);
            }
            setEType(op);
        }
        void print() {
            cout << OPERATOR_STRINGS[getEType()] << " ";
        }
    };
const string INFIX_FORMAT_ERROR = "Expression not in infix format";
//read in expression backwards
list<ExpressionPart*> readExpr() { 
    cout << "Expression: ";
    list<ExpressionPart*> expressions;
    ExpressionPart *epart = NULL;
    string theString;
    do {
        cin >> theString;
        try {
            double n = stoi(theString);
            epart = new ExpressionNumber(n);
        }
        catch (exception e) {
            try {
                epart = new ExpressionOperator(theString);
            }
            catch (exception e) {
                cerr << "ERROR!  Unknown expression part " << theString << endl;
                break;
            }
        }
        //put semi colon at the back
        if (epart->getEType() == SEMI){ 
            expressions.push_back(epart);
        }
        //switch parentheses
        else{
            if (epart->getEType() == LPAREN) { epart->setEType(RPAREN); } 
            else if (epart->getEType() == RPAREN) { epart->setEType(LPAREN); } 
            //push everything else to front
            expressions.push_front(epart); 
        }
    } while (epart->getEType() != SEMI);
    return expressions;
}
void showExpr(list<ExpressionPart*> expressions) {
    //iterate through expression backwards (puts it in the correct direction for display)
    for (auto ep = expressions.rbegin(); ep != expressions.rend(); ++ep) { 
        if ((*ep)->getEType() == SEMI){
            //do nothing
        }
        //swap parantheses (again) so it's displayed correctly
        else{
            if ((*ep)->getEType() == LPAREN) { cout << ") "; } 
            else if ((*ep)->getEType() == RPAREN) { cout << "( "; }
            else (*ep)->print();
        }
    }
    cout << endl;
}
/*void evalPostfixExpr(list<ExpressionPart*> expressions) {
    list<ExpressionPart*> exprStack;
    for (auto ep : expressions) {
        switch (ep->getEType()) {
            case SEMI:
                if (exprStack.size() == 1) {
                    ExpressionPart* res = exprStack.front();
                    if (res->getEType() == NUMBER) {
                        cout << "Result is " << ((ExpressionNumber*) res)->getNumber() << endl;
                    }
                    else {
                        throw INFIX_FORMAT_ERROR;
                    }
                }
                else {
                    throw INFIX_FORMAT_ERROR;
                }
                break;
            case NUMBER:
                exprStack.push_front(ep);
                break;
            case LPAREN:
            case RPAREN:
                throw INFIX_FORMAT_ERROR;
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
                ExpressionPart *rgt = exprStack.front();
                exprStack.pop_front();
                ExpressionPart *lft = exprStack.front();
                exprStack.pop_front();
                if ((lft->getEType() != NUMBER) && (rgt->getEType() != NUMBER)) {
                    throw INFIX_FORMAT_ERROR;
                }
                double res = ((ExpressionNumber*)lft)->getNumber();
                switch (ep->getEType()) {
                    case ADD:
                        res += ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case MINUS:
                        res -= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case TIMES:
                        res *= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case DIVIDE:
                        res /= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case POWER:
                        res = pow(res, ((ExpressionNumber *) rgt)->getNumber());
                        break;
                }
                exprStack.push_front(new ExpressionNumber(res));
                break;
        }
    }
}
*/
void evalPrefixExpr(list<ExpressionPart*> expressions) {
    list<ExpressionPart*> exprStack;
    for (auto ep : expressions) {
        switch (ep->getEType()) {
            case SEMI:
                if (exprStack.size() == 1) {
                    ExpressionPart* res = exprStack.front();
                    if (res->getEType() == NUMBER) {
                        cout << "Result is " << ((ExpressionNumber*) res)->getNumber() << endl;
                    }
                    else {
                        throw INFIX_FORMAT_ERROR;
                    }
                }
                else {
                    throw INFIX_FORMAT_ERROR;
                }
                break;
            case NUMBER:
                exprStack.push_front(ep);
                break;
            case LPAREN:
            case RPAREN:
                throw INFIX_FORMAT_ERROR;
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
            //reverse L & R operator from postfix version
                ExpressionPart *lft = exprStack.front(); 
                exprStack.pop_front();
                ExpressionPart *rgt = exprStack.front();
                exprStack.pop_front();
                if ((lft->getEType() != NUMBER) || (rgt->getEType() != NUMBER)) {
                    throw INFIX_FORMAT_ERROR;
                }
                double res = ((ExpressionNumber*)lft)->getNumber();
                switch (ep->getEType()) {
                    case ADD:
                        res += ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case MINUS:
                        res -= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case TIMES:
                        res *= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case DIVIDE:
                        res /= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case POWER:
                        res = pow(res, ((ExpressionNumber *) rgt)->getNumber());
                        break;
                }
                exprStack.push_front(new ExpressionNumber(res));
                break;
        }
    }
}
/*list<ExpressionPart*> convertToPostfix(list<ExpressionPart*> ifExprs) {
    list<ExpressionPart*> pfExprs;
    list<ExpressionPart*> opStack;
    for (auto ep : ifExprs) {
        switch (ep->getEType()) {
            case SEMI:
                while (!opStack.empty()) {
                    ExpressionPart* opp = opStack.front();
                    if (opp->getEType() == LPAREN) {
                        throw INFIX_FORMAT_ERROR;
                    }
                    else {
                        pfExprs.push_back(opp);
                    }
                    opStack.pop_front();
                }
                pfExprs.push_back(ep);
                return pfExprs;
                break;
            case LPAREN:
                opStack.push_front(ep);
                break;
            case RPAREN:
                while ((!opStack.empty()) && (opStack.front()->getEType() != LPAREN)) {
                    pfExprs.push_back(opStack.front());
                    opStack.pop_front();
                }
                if (opStack.empty()) {
                    throw INFIX_FORMAT_ERROR;
                }
                else {
                    opStack.pop_front();
                }
                break;
            case NUMBER:
                pfExprs.push_back(ep);
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
                while ((!opStack.empty()) &&
                        (opStack.front()->getEType() != LPAREN) &&
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] > OPERATOR_PRECEDENCE[ep->getEType()]) ||
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] == OPERATOR_PRECEDENCE[ep->getEType()]) && OPERATOR_LEFT_ASSOCIATIVE[ep->getEType()]))) {
                    ExpressionPart *opp = opStack.front();
                    pfExprs.push_back(opp);
                    opStack.pop_front();
                }
                opStack.push_front(ep);
                break;
        }
    }
    pfExprs.clear();
    return pfExprs;
}
*/
list<ExpressionPart*> convertToPrefix(list<ExpressionPart*> ifExprs) {
    list<ExpressionPart*> pfExprs;
    list<ExpressionPart*> opStack;
    for (auto ep : ifExprs) {
        switch (ep->getEType()) {
            case SEMI:
                while (!opStack.empty()) {
                    ExpressionPart* opp = opStack.front();
                    if (opp->getEType() == LPAREN) {
                        throw INFIX_FORMAT_ERROR;
                    }
                    else {
                        pfExprs.push_back(opp);
                    }
                    opStack.pop_front();
                }
                pfExprs.push_back(ep);
                return pfExprs;
                break;
            case LPAREN:
                opStack.push_front(ep);
                break;
            case RPAREN:
                while ((!opStack.empty()) && (opStack.front()->getEType() != LPAREN)) {
                    pfExprs.push_back(opStack.front());
                    opStack.pop_front();
                }
                if (opStack.empty()) {
                    throw INFIX_FORMAT_ERROR;
                }
                else {
                    opStack.pop_front();
                }
                break;
            case NUMBER:
                pfExprs.push_back(ep);
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
                while ((!opStack.empty()) &&
                        (opStack.front()->getEType() != LPAREN) &&
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] > OPERATOR_PRECEDENCE[ep->getEType()]) ||
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] == OPERATOR_PRECEDENCE[ep->getEType()]) && OPERATOR_LEFT_ASSOCIATIVE[ep->getEType()]))) {
                    ExpressionPart *opp = opStack.front();
                    pfExprs.push_back(opp);
                    opStack.pop_front();
                }
                opStack.push_front(ep);
                break;
        }
    }
    pfExprs.clear();
    return pfExprs;
}
int main() {
    try {
        list<ExpressionPart *> expressions = readExpr();
        showExpr(expressions);
        //list<ExpressionPart *> pfExprs = convertToPostfix(expressions);
        list<ExpressionPart *> pfExprs = convertToPrefix(expressions);
        //cout << "Postfix version of expression: ";
        cout << "Prefix version of expression: ";
        showExpr(pfExprs);
        //evalPostfixExpr(pfExprs);
        evalPrefixExpr(pfExprs);
    }
    catch (string s) {
        cerr << s << endl;
    }
    return 0;
}