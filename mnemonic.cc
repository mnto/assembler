#include <iostream>

using namespace std;

class MNEM {
private:
string _type;
int _format;
int _opcode;
string _operand;


public:
MNEM() {
  _type = "";
  _format = 0;
  _opcode = 0;
  _operand = "";
}

MNEM(string t, int f, int opc, string opr) {
  _type = t;
  _format = f;
  _opcode = opc;
  _operand = opr;
}

string getType() {return _type;}
    
int getFormat() {return _format;}

int getOpc() {return _opcode;}

string getOpr() {return _operand;}

void setType(string newType) {_type = newType;}

void setFomrat(int newForm) {_format = newForm;}

void setOpc(int newOpc) {_opcode = newOpc;}

void setOpr(string newOpr) {_operand = newOpr;}

};

