// Colin Sawyer
// Cpsc 3300 Computer Organization
// Project 2: Simulation of Eckert's microprogrammed machine
// 3/15/18

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <bitset>
using namespace std;

void printSigs(int num){

  //string binary = to_string(num);
  bitset<24> binary(num);
  string binaryString = binary.to_string();
  string str;
  str += " ";
  if(binaryString[0] == '1'){str+= "ip ";} else{str+= "   ";}
  if(binaryString[1] == '1'){str+= "lp ";} else{str+= "   ";}
  if(binaryString[2] == '1'){str+= "ep ";} else{str+= "   ";}
  if(binaryString[3] == '1'){str+= "lm ";} else{str+= "   ";}
  if(binaryString[4] == '1'){str+= "r ";} else{str+= "  ";}
  if(binaryString[5] == '1'){str+= "w ";} else{str+= "  ";}
  if(binaryString[6] == '1'){str+= "ld ";} else{str+= "   ";}
  if(binaryString[7] == '1'){str+= "ed ";} else{str+= "   ";}
  if(binaryString[8] == '1'){str+= "li ";} else{str+= "   ";}
  if(binaryString[9] == '1'){str+= "ei ";} else{str+= "   ";}
  if(binaryString[10] == '1'){str+= "la ";} else{str+= "   ";}
  if(binaryString[11] == '1'){str+= "ea ";} else{str+= "   ";}
  if(binaryString[12] == '1'){str+= "a ";} else{str+= "  ";}
  if(binaryString[13] == '1'){str+= "s ";} else{str+= "  ";}
  if(binaryString[14] == '1'){str+= "eu ";} else{str+= "   ";}
  if(binaryString[15] == '1'){str+= "lb ";} else{str+= "   ";}
  if(binaryString[16] == '1'){str+= "cd ";} else{str+= "   ";}
  if(binaryString[17] == '1'){str+= "map ";} else{str+= "    ";}
  if(binaryString[18] == '1'){str+= "hlt ";} else{str+= "    ";}
  str+= "crja=";
  int crja = num & 31;
  cout << str << setw(2) << setfill('0') << hex << crja << endl;
  return;
}
int main() {
  //open the files to read from
  ifstream ram("ram.txt");
  ifstream addr("addr.txt");
  ifstream uprog("uprog.txt");

  //create registers
  int pc = 0, mar = 0;
  int mdr = 0, acc = 0, alu = 0, b = 0, ir = 0;

  //set ram values array initialized to 0, then read in from ram file
  int rvalues[256];
  for(int i = 0; i < 256; i++) {rvalues[i] = 0;}
  int rtotal = 0;
  int temp;
  while(ram >> hex >> temp) {rvalues[rtotal] = temp; rtotal++;}

  // read in values from addr file
  int avalues[16];
  for(int i = 0; i < 16; i++) {addr >> hex >> avalues[i];}

  //read in values from the uprog file
  long int uvalues[32];
  for(int i = 0; i < 32; i++) {uprog >> hex >> uvalues[i];}

  //print out all the information before simulation takes place
  cout << "microprogram simulation of Eckert's simple machine" << endl;
  cout << "(all values are shown in hexadecimal)\n" << endl;
  //print out the initial ram values with their address
  cout << "contents of RAM memory" << endl;
  cout << "addr value" << endl;
  for(int i = 0; i < rtotal; i++){
    if(i < 16) {cout << " ";}
    cout << setfill('0') << " " << hex << i << ":  " << setw(3) << rvalues[i] << endl;
  }

  //print out the contents of the address from
  cout << "\ncontents of address ROM" << endl;
  cout << "opc addr" << endl;
  for(int i = 0; i < 16; i++){
    cout << setfill('0') << " " << hex << i << ":  " << setw(2) << avalues[i] << endl;
  }

  //print out contents of control ROM
  cout << "\ncontents of control ROM with active signals identified" << endl;
  cout << "addr contents" << endl;
  for(int i = 0; i < 32; i++){
    if(i < 16) {cout << " ";}
    cout << " " << hex << i << ":  " << setfill('0') << setw(6) << uvalues[i];
    printSigs(uvalues[i]);
  }
  //print out register values
  cout << "\ninitial register values" << endl;
  cout << " pc mar mdr acc alu   b  ir" << endl;
  cout << "  " << pc << "   " << mar << "   " << mdr << "   " << acc << "   ";
  cout << alu << "   " << b << "   " << ir << endl;


  //begin the Simulation
  cout << "\ncontrol signals and register values after each microinstruction" << endl;
  cout << "                     mh" << endl;
  cout << "   ilelrwleleleasel cal" << endl;
  cout << "uc pppm  ddiiaa  ub dpt ja pc mar mdr acc alu  b  ir" << endl;

  int ip = 0, lp = 1, ep = 2, lm = 3, r = 4, w = 5, ld = 6, ed = 7, li = 8;
  int ei = 9, la = 10, ea = 11, a = 12, s = 13, eu = 14, lb = 15, cd = 16, map = 17;
  int hlt = 18;
  int current = uvalues[pc];

  for(int i = 0; i < 10; i++){
    string step;
    int bus, crja, next = 0;
    crja = current & 31;
    bitset<24> instruction(current);
    string instructionString = instruction.to_string();
    //handles enable signals to the bus
    if(instructionString[ep] == '1') {bus = pc; step+= "ep ";}
    if(instructionString[ed] == '1') {bus = mdr; step+= "ed ";}
    if(instructionString[ei] == '1') {bus = ir; step+= "ei ";}
    if(instructionString[ea] == '1') {bus = acc; step+= "ea ";}
    if(instructionString[eu] == '1') {bus = alu; step+= "eu ";}

    //handles read/write signals ---- may not be bus but MDR instead
    if(instructionString[r] == '1') {mdr = rvalues[mar]; step+= "r ";}
    if(instructionString[w] == '1') {rvalues[mar] = mdr; step+= "w ";}

    //handles add/subtract signals
    if(instructionString[a] == '1') {alu = acc + b; step+= "a ";}
    if(instructionString[s] == '1') {alu = acc - b; step+= "s ";}

    // handles load signals from the bus
    if(instructionString[lp] == '1') {pc = bus; step+= "lp ";}
    if(instructionString[lm] == '1') {mar = bus; step+= "lm ";}
    if(instructionString[ld] == '1') {mdr = bus; step+= "ld ";}
    if(instructionString[li] == '1') {ir = bus; step+= "li ";}
    if(instructionString[la] == '1') {acc = bus; step+= "la ";}
    if(instructionString[lb] == '1') {b = bus; step+= "lb ";}

    //increment PC signal
    if(instructionString[ip] == '1') {pc++; step+= "ip ";}

    // microinstruction counter
    if(instructionString[map] == '0' && instructionString[cd] == '0'){
      current = uvalues[crja];
    }
    //opcode mapping
    if(instructionString[map] == '1') {
      next = ir & 0xf00;
      next = next >> 8;
      current = uvalues[next];
      step+= "map ";
    }
    else {next++;}

    //halt
    if(instructionString[hlt] == '1'){
      //print final string
      break;
    }
    //print instruction steps
    string micro = instruction.to_string();
    cout << setw(2) << setfill('0') << hex << next << " ";
    for(int i = 0; i < 16; i++) {cout << micro[i];}
    cout << " " << micro[16] << micro[17] << micro[18] << " ";
    temp = current & 31;
    cout << setw(2) << setfill('0') << hex << temp << "   ";
    cout << setw(3) << setfill('0') << hex << pc << "  " << mar << " " << mdr;
    cout << " " << acc << " " << alu << "   " << b << " " << ir << "   " << step << endl;


  }








  return 0;
}
