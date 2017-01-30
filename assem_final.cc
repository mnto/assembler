/*****************************************************************************
*                                                                            *
* NAME: Minh To                                                              *
* LAST UPDATED: December 11, 2016                                            *
* PROJECT: Assembler                                                         *
* DESCRIPTION: This program simulates the SIC/XE machine, assembles the      *
*              program and outputs the object code in hex in filename.exe    *           
*                                                                            *
*****************************************************************************/

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "mnemonic.cc"
#include <map>
#include <bitset>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace std;

std::map <string, MNEM> mne_table;
std::map <string, int> symtab;
int offset = 0;
string n, i, x, b, p, e = "0";
string base;

// POPULATE MNEMONIC TABLE
void init() {
  mne_table["ADD"] = MNEM("i", 3, 24, "m");
  mne_table["ADDR"] = MNEM("i", 2, 144, "r1,r2");
  mne_table["AND"] = MNEM("i", 3, 64, "m");
  mne_table["CLEAR"] = MNEM("i", 2, 180, "r1");
  mne_table["COMP"] = MNEM("i", 3, 40, "m");
  mne_table["COMPR"] = MNEM("i", 2, 160, "r1,r2");
  mne_table["DIV"] = MNEM("i", 3, 36, "m");
  mne_table["DIVR"] = MNEM("i", 2, 156, "r1,r2");
  mne_table["HIO"] = MNEM("i", 1, 244, "");
  mne_table["J"] = MNEM("i", 3, 60, "m");
  mne_table["JEQ"] = MNEM("i", 3, 48, "m");
  mne_table["JGT"] = MNEM("i", 3, 52, "m");
  mne_table["JLT"] = MNEM("i", 3, 56, "m");
  mne_table["JSUB"] = MNEM("i", 3, 72, "m");
  mne_table["LDA"] = MNEM("i", 3, 0, "m");
  mne_table["LDB"] = MNEM("i", 3, 104, "m");
  mne_table["LDCH"] = MNEM("i", 3, 80, "m");
  mne_table["LDF"] = MNEM("i", 3, 112, "m");
  mne_table["LDL"] = MNEM("i", 3, 8, "m");
  mne_table["LDS"] = MNEM("i", 3, 108, "m");
  mne_table["LDT"] = MNEM("i", 3, 116, "m");
  mne_table["LDX"] = MNEM("i", 3, 4, "m");
  mne_table["LPS"] = MNEM("i", 3, 208, "m");
  mne_table["MUL"] = MNEM("i", 3, 32, "m");
  mne_table["MULR"] = MNEM("i", 2, 152, "r1,r2");
  mne_table["NORM"] = MNEM("i", 1, 200, "");
  mne_table["OR"] = MNEM("i", 3, 68, "m");
  mne_table["RD"] = MNEM("i", 3, 216, "m");
  mne_table["RMO"] = MNEM("i", 2, 172, "r1,r2");
  mne_table["RSUB"] = MNEM("i", 3, 76, "");
  mne_table["SHIFTL"] = MNEM("i", 2, 164, "r1,n");
  mne_table["SHIFTR"] = MNEM("i", 2, 168, "r1,n");
  mne_table["SIO"] = MNEM("i", 1, 240, "");
  mne_table["SSK"] = MNEM("i", 3, 236, "m");
  mne_table["STA"] = MNEM("i", 3, 12, "m");
  mne_table["STB"] = MNEM("i", 3, 120, "m");
  mne_table["STCH"] = MNEM("i", 3, 84, "m");
  mne_table["STF"] = MNEM("i", 3, 128, "m");
  mne_table["STI"] = MNEM("i", 3, 212, "m");
  mne_table["STL"] = MNEM("i", 3, 20, "m");
  mne_table["STS"] = MNEM("i", 3, 124, "m");
  mne_table["STSW"] = MNEM("i", 3, 232, "m");
  mne_table["STT"] = MNEM("i", 3, 132, "m");
  mne_table["STX"] = MNEM("i", 3, 16,"m");
  mne_table["SUB"] = MNEM("i", 3, 28, "m");
  mne_table["SUBR"] = MNEM("i", 2, 148, "r1,r2");
  mne_table["SVC"] = MNEM("i", 2, 176, "n");
  mne_table["TD"] = MNEM("i", 3, 224, "m");
  mne_table["TIO"] = MNEM("i", 1, 248, "");
  mne_table["TIX"] = MNEM("i", 3, 44, "m");
  mne_table["TIXR"] = MNEM("i", 2, 184,"r1");
  mne_table["WD"] = MNEM("i", 3, 220, "m");
  mne_table["RESW"] = MNEM("d", 0, 0, "c");
  mne_table["WORD"] = MNEM("d", 0, 0, "c");
  mne_table["RESB"] = MNEM("d", 0, 0, "c");
  mne_table["BYTE"] = MNEM("d", 0, 0, "c");
  mne_table["START"] = MNEM("d", 0, 0, "c");
  mne_table["END"] = MNEM("d", 0, 0, "c");
  mne_table["BASE"] = MNEM("a", 0, 0, "");
  mne_table["NOBASE"] = MNEM("a", 0, 0, "");
  mne_table["A"] = MNEM("r", 2, 0, "");
  mne_table["X"] = MNEM("r", 2, 1, "");
  mne_table["L"] = MNEM("r", 2, 2, "");
  mne_table["B"] = MNEM("r", 2, 3, "");
  mne_table["S"] = MNEM("r", 2, 4, "");
  mne_table["T"] = MNEM("r", 2, 5, "");
  mne_table["F"] = MNEM("r", 2, 6, "");
  mne_table["PC"] = MNEM("r", 2, 8, "");
  mne_table["SW"] = MNEM("r", 2, 9, "");
}

void upper(string & str) {
  //-------------- captitalize a string
  transform(str.begin(), str.end(), str.begin(), ::toupper);
}

int stoi(string num) {
  istringstream buffer(num);
  int result;
  buffer >> result;
  return result;
}

string formatHex(string str, size_t length) {
  //-------------- pad 0's to the front of a string of numbers until 
  // the desired length specified in the parameter is reached
  // or truncate the string if it's longer than the desired length
  string ret = "";
  if (str.size() < length) { // pad 0's in front
    for (size_t i = 0; i < length - str.size(); i++)
      ret += "0";
  }
  else // or truncate the string
    str = str.substr(str.size() - length);
  return ret + str;
}

string decimal_to_hex(int dec) {
  //-------------- convert a decimal number to hex string
  string result;
  stringstream ss;
  ss << std::hex << dec;
  result = ss.str();
  upper(result);
  return result;
}

int hex_to_decimal(string hex) {
  //-------------- convert a hex string to a decimal number
  stringstream ss;
  int dec;
  ss << hex;
  ss >> std::hex >> dec;
  return dec;
}

int disp_to_label(int pc, string label) {
  //-------------- return the distance from the program counter to a label
  return symtab[label] - pc;
}

int disp_to_base(string operand) {
  return symtab[operand] - symtab[base];
}

bool indirect(string operand) { return operand[0] == '@'; }

bool imm8(string operand) { return operand[0] == '#'; }

bool index(string operand) { return operand.find(",X") != string::npos;}

bool base_relative(string operand) {
  return operand == base;
}

bool pc_relative(int pc, string operand) {
  return disp_to_label(pc, operand) > -2047 
  and disp_to_label(pc, operand) < 2047;
}

string nixbpe(int pc, string mnem, string operand) {
  //-------------- return a string with the first 1.5 byte for format 3 and 4
  string byte1, nybble;
  // if INDEXed, set x
  if (index(operand)) {
    x = "1";
    operand = operand.substr(0, operand.find(",X")); // truncate ,X
  }
  // if INDIRECT, set n
  if (indirect(operand)) {
    n = "1"; 
    operand = operand.substr(1); // truncate @
    // set b or c if format 3
    if (e != "1") {
      if (pc_relative(pc, operand))
	p = "1";
      else 
	b = "1";
    }
  }
  // if IMMEDIATE, set i
  else if (imm8(operand)) {
    operand = operand.substr(1); // truncate #
    i = "1"; 
    // clear b, p = 0 if given an immediate value
    if (symtab.find(operand) == symtab.end()) {
      b = "0";
      p = "0";
    }
    // otherwise set b or p if format 3
    else {
      if (e != "1") {
	if (pc_relative(pc, operand))
	  p = "1";
	else 
	  b = "1";
      }
    }
  }
  // if DIRECT, set both n and i
  else { 
    n = "1";  
    i = "1";
    // set b or p if format 3
    if (e != "1") {
      if (mnem == "RSUB") {
	b = "0";
	p = "0";
      }
      else if (pc_relative(pc, operand)) { // pc relative
	p = "1";
      }
      else { // base relative
        b = "1";
      }
    }
  }
  byte1 = formatHex(decimal_to_hex(mne_table[mnem].getOpc() 
    + std::bitset<2>(n + i).to_ulong()), 2);
  nybble = decimal_to_hex(std::bitset<4>(x + b + p + e).to_ulong());
  return byte1 + nybble;
}

string format1(string mnem, string & exe) {
  //-------------- return opcode for format 1, add opcode to exe string
  string opcode = decimal_to_hex(mne_table[mnem].getOpc());

  // ADD to exe string
  exe += opcode;
  return opcode;
}

string format2(string mnem, string operand, string & exe) {
  //-------------- return opcode for format 2, add opcode to exe string
  string opcode = decimal_to_hex(mne_table[mnem].getOpc());
  string op1, op2;
  // for these special mnemonics, r1 = object code, r2 = 0
  if (mnem == "CLEAR" or mnem == "TIXR" or mnem == "SVC")
    opcode += decimal_to_hex(mne_table[operand].getOpc()) + "0";
  else {
    op1 = operand.substr(0, operand.find(","));
    op1 = decimal_to_hex(mne_table[op1].getOpc());
    op2 = operand.substr(operand.find(",") + 1);
    // special case for SHIFTR and SHIFTL
    if (mnem == "SHIFTR" or mnem == "SHIFTL")
      op2 = decimal_to_hex((stoi(op2) - 1));
    else
      op2 = decimal_to_hex(mne_table[op2].getOpc());
  }
  opcode += op1 + op2;

  // ADD to exe
  exe += opcode;
  return opcode;
}

string format3(int pc, string mnem, string operand, string & exe) {
  //-------------- return opcode for format 3, add opcode to exe string
  string ret =  nixbpe(pc, mnem, operand);
  if (index(operand))
    operand = operand.substr(0, operand.find(",X")); // truncate ,X
  if (imm8(operand)) {// if immediate, find immediate value
    operand = operand.substr(1); // truncate the #
    if (b == "1")  // base relative
      ret += formatHex(decimal_to_hex(disp_to_base(operand)), 3);
    else if (p == "1"){ // with mmediate label
      int label_dist = 0 - (~disp_to_label(pc, operand) + 1); // 2's complement
      ret += formatHex(decimal_to_hex(label_dist), 3); 
    }
    else if (b == "0" and p == "0") // immediate value
      ret += formatHex(decimal_to_hex(stoi(operand)), 3);
  }
  else { // if direct or indirect, find displacement
    if (indirect(operand))
      operand = operand.substr(1);
    if (b == "1")
      ret += formatHex(decimal_to_hex(disp_to_base(operand)), 3);
    else {
      if (mnem == "RSUB")
	ret += "000";
      else {
	int label_dist = 0 - (~disp_to_label(pc, operand) + 1); //2's complement
	ret += formatHex(decimal_to_hex(label_dist), 3); 
      }
    }
  }

  // ADD TO EXE STRING
  for (size_t i = 0; i < ret.size(); i++)
    exe += ret[i];
  return ret;
}

string format4(int pc, string mnem, string operand, string & exe) {
  //-------------- return opcode for format 4, add opcode to exe string
  string ret = nixbpe(pc, mnem, operand);
  if (mnem == "RSUB") // special case for RSUB
    ret += "00000";
  else if (imm8(operand)) {
    operand = operand.substr(1); // truncate the #
    // if the operand is an immediate number/value, address = value
    if (symtab.find(operand) == symtab.end()) 
      ret += formatHex(decimal_to_hex(stoi(operand)), 5);
    // if it is a label, then append the address of that label
    else
      ret += formatHex(decimal_to_hex(symtab[operand]), 5);
  }
  // if is indexed, address = address of label
  else if (index(operand)) {
    operand = operand.substr(0, operand.find(",X"));
    ret += formatHex(decimal_to_hex(symtab[operand]), 5);
  }
  // if direct or indirect, address = address of label
  else { 
    if (indirect(operand))
      operand = operand.substr(1);
    ret += formatHex(decimal_to_hex(symtab[operand]), 5);
  }

  // ADD TO EXE FILE
  stringstream ss;
  for (size_t i = 0; i < ret.size(); i++)
    exe += ret[i];
  return ret;
}

void error(string message) {
  //-------------- print an error message and exit
  cout << message << endl;
  exit(1);
}

bool lookup_mnem(string mnem) {
  return mne_table.find(mnem) != mne_table.end();
}

void base_mnemonic(string & mnem) {
  //-------------- strip off + sign from mnemonic
  if (mnem[0] == '+') {
    mnem.erase(0, 1);
    e = "1";
  }
}

int intstr_length(string & mnem, string operand) {
  //-------------- return how many bytes each command takes or to reserve
  if (mnem != "") { // if mnemonic exists
    base_mnemonic(mnem);
    if (lookup_mnem(mnem)) { // if mnemonic is legal
      if (mne_table[mnem].getType() == "i") { // if mnemonic is an instruction
        if (e == "1")                         // if is format 4
	  return mne_table[mnem].getFormat() + 1; // return 4
        else                                  // else return format
	  return mne_table[mnem].getFormat();
      }
      else if (mne_table[mnem].getType() == "d") {// if mnemonic is a directive
        int value = atoi(operand.c_str()); // convert string to integer
        if (mnem == "RESB")
          return value;
        else if (mnem == "BYTE") {
	  if (operand[0] == 'C') { // if chars then reserve as many bytes needed
	    int count = 0;
	    int i = 2;
	    while (operand[i] != '\'') { // count how many chars and return
	      count++;
	      i++;
	    }
	    return count;
	  }
	  else
	    return 1;             // otherwise just reserve 1 byte
	}
        else if (mnem == "RESW")  // reserve however many words needed
          return 3 * value;
        else                      // reserve 3 bytes (1 word)
          return 3;
      }
      else
        return 0;
    } 
    else {                // if mnemonic is illegal, print out error message
      error("Mnemonic " + mnem + " doensn't exist");
      return 0;
    }
  }
  return 0;
}

void make_symtab(string label) {
  //-------------- add a label to the symbol table 
  // if it hasn't been added yet, otherwise signal an error
  if (label != "") { // if there is a label
    if (symtab.find(label) == symtab.end()) // add label if not added
      symtab[label] = offset;
    else // throw error if symbol exists already
      error("Label " + label + " already exists"); 
  }
}

void intermediate(string label, string mnem, string opr, 
                  string comment, ostream & out) {
  //-------------- write to an intermediate file in standard output
  string address;
  if (mnem == "BASE" or mnem == "NOBASE" or 
      mnem == "START" or mnem == "END" or comment != "")
    out << '\t' << comment;
  else { 
    address = formatHex(decimal_to_hex(offset), 5); // format offset 
    out << address << '\t';
  }
  out << label << '\t' << mnem << '\t' << opr << endl;
}

void unpack_line(string line, string & label, string & mnem, 
                 string & operand, string & comment) {
  //-------------- separate label, mnemonic, operand, and comment fields
  stringstream linestream;
  linestream << line;
  if (line[0] != '.') { // if not a comment line
    if (line[0] > 32)   // check if there is a label
      linestream >> label;
    linestream >> mnem >> operand;  // separate into fields
  }
  else 
    comment = line; // comment line
}


void unpack_line2(string line, string & mnem, string & operand) {
  //-------------- read intermediate file again, separate into 
  // mnemonics, and operands
  stringstream linestream;
  string label;
  linestream << line;
  if (line[0] != '.') { // separate if not a comment line
    if (line[0] != '\t')
      linestream >> label;
    linestream >> mnem >> operand;
    if (mnem == "BASE") // if found BASE mnemonic, then set base
      base = operand;
  }
}

void pass1(string line, ostream & out) {
  //-------------- pass 1: separate into fields and make symbol table
  string label, mnem, opr, comment;
  stringstream linestream;
  unpack_line(line, label, mnem, opr, comment);
  make_symtab(label);
  intermediate(label, mnem, opr, comment, out);
  if (mnem == "START") // set address if it is specified
    offset = hex_to_decimal(opr);
  else if (comment == "") // if not a comment line then calculate the offset
    offset += intstr_length(mnem, opr);
  e = "0"; // reset format 4 bool at the end of pass 1
}

void pass2(string line, int & pc, string & exe) {
  string mnem, opr;
  //-------------- pass 2: print out the line in standard output with opcode
  cout << line;
  line = line.substr(line.find('\t') + 1);
  unpack_line2(line, mnem, opr);
  if (mnem != "" and mnem != "START" and mnem != "END") { // if mnemonic exists
    cout <<  '\t';
    pc += intstr_length(mnem, opr);
    if (mnem == "BASE") // set BASE
      base = opr;
    else if (mnem == "NOBASE") // clear BASE
      base = "";
    else if (mne_table[mnem].getType() == "i") { // instructions
      if (e == "1") // format 4
	cout << format4(pc, mnem, opr, exe);
      else if (mne_table[mnem].getFormat() == 1) // format 1
	cout << format1(mnem, exe);
      else if (mne_table[mnem].getFormat() == 2) // format 2
	cout << format2(mnem, opr, exe);
      else if(mne_table[mnem].getFormat() == 3)  // format 3
	cout << format3(pc, mnem, opr, exe); 
    }
    else { // directives: add data declarations to the exe file
      if (mnem == "RESB") {
	for (int i = 0; i < stoi(opr); i++)
	  exe += "00"; // reserve a byte for each byte
      }
      else if (mnem == "RESW") {
	for (int i = 0; i < stoi(opr); i++)
	  exe += "000000"; // reserve 3 bytes for each word
      }
      else if (mnem == "BYTE") {
	if (opr[0] == 'C') {
	  for (int i = 2; opr[i] != '\''; i++) {
	    string hex = decimal_to_hex(int(opr[i]));
	    cout << hex;
	    exe += hex; // reserve 1 byte
	  }
	}
	else if (opr[0] == 'X') {
	  for (int i = 2; opr[i] != '\''; i++) {
	    cout << opr[i];
	    exe += opr[i];
	  }
	}
	else
	  exe += opr; 
      }
      else if (mnem == "WORD") {
	string op = formatHex(decimal_to_hex(stoi(opr)), 6);
	cout << op;
	exe += op;     // resreve 1 word
      }
    }
  }
  cout << endl;
  n = "0";
  i = "0";
  x = "0";
  b = "0";
  p = "0";
  e = "0"; // clear bools
}

void to_exe(string exe, ostream & exefile) {
  //-------------- take the string that contains all the opcode 
  // and output it to .exe file
  string output;
  int ascii;
  // make opcode lowercase
  for (size_t i = 0; i < exe.size(); i++) {
    transform(exe.begin(), exe.end(), exe.begin(), ::tolower); 
  }
  // output to .exe file
  for (size_t i = 0; i < exe.size(); i += 2) {
    ascii = hex_to_decimal(exe.substr(i, 2));
    exefile << (unsigned char)(ascii);
  }
}

int main(int argc, char **argv) {
  init();
  string filename = argv[1];
  string line = "";
  int pc = 0;
  argc += 0; // only so that compiler wouldn't yell at me
  ifstream in(filename.c_str());
  ofstream out("inter.txt");
  string outfile = filename.substr(0, filename.size() - 4) + ".exe";
  ofstream exefile(outfile.c_str());
  char c;
  // PASS 1
  while (in.get(c)) {
    line += c; 
    if (c == '\n') {
      line = line.substr(0, line.size() - 1); // get rid of the '\n'
      pass1(line, out); 
      line = "";
    }
  }
  // PASS 2
  ifstream in2("inter.txt");
  string exe = "";
  // print out the symbol table before printing out anything else
  for (std::map<string, int>::iterator it = symtab.begin(); 
       it != symtab.end(); ++it) {
    string address = formatHex(decimal_to_hex(it->second), 5);
    cout << it->first << ": " <<  address << endl;
  }
  while (in2.get(c)) {
    line += c; 
    if (c == '\n') {
      line = line.substr(0, line.size() - 1); // get rid of the '\n'
      pass2(line, pc, exe); 
      line = "";
    }
  }
  to_exe(exe, exefile); // write to .exe file
}

