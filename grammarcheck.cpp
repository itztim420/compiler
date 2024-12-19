#include <iostream> 
#include <fstream> 
#include <string> 
#include <regex> 
#include <cstdlib> 
using namespace std;
class myLanguage
{
private:
	vector<string> vars;
	bool readingVar = false;
	string tempVar = "";
	bool readingIdent = false;
	bool mainCode = false;
	string tempIdent = "";
	vector<string> myStack;
	int table[36][34] = {
		{201, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // blank = 100, program | PB = 201
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 202, 100, 100, 100, 100,
		100, 100, 100, 100}, //; PC = 202
		{100, 203, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, //var DL PD = 203
		{100, 100, 204, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // begin SL PE = 204
		{100, 100, 100, 205, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // end. = 205
		{100, 100, 100, 100, 100, 100, 100, 206, 206, 206,
		206, 206, 206, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // L IB = 206
		{100, 100, 100, 100, 100, 100, 100, 206, 206, 206,
		206, 206, 206, 207, 207, 207, 207, 207, 207, 207,
		207, 207, 207, 100, 300, 300, 300, 300, 300, 300,
		300, 300, 100, 300}, // D IB = 207, lambda = 300
		{100, 100, 100, 100, 100, 100, 100, 208, 208, 208,
		208, 208, 208, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // DC DLB = 208
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 209, 100, 100, 100,
		100, 100, 100, 100}, // : TP DLC = 209
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 210, 100, 100, 100, 100,
		100, 100, 100, 100}, // ; = 210
		{100, 100, 100, 100, 100, 100, 100, 211, 211, 211,
		211, 211, 211, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // I DCB = 211
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 300, 100, 100, 100,
		100, 100, 100, 212}, // , DC = 212
		{100, 100, 100, 100, 213, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // integer = 213
		{100, 100, 100, 100, 100, 214, 100, 214, 214, 214,
		214, 214, 214, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // ST SLB = 214
		{100, 100, 100, 300, 100, 215, 100, 215, 215, 215,
		215, 215, 215, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // SL = 215
		{100, 100, 100, 100, 100, 216, 100, 217, 217, 217,
		217, 217, 217, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // W = 216, A = 217
		{100, 100, 100, 100, 100, 218, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // print WB = 218
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 219, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // ( WC = 219
		{100, 100, 100, 100, 100, 100, 220, 220, 220, 220,
		220, 220, 220, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // SR WD = 220
		{100, 100, 100, 100, 100, 100, 100, 221, 221, 221,
		221, 221, 221, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // I WE = 221
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 222, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // ) WF = 222
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 210, 100, 100, 100, 100,
		100, 100, 100, 100},
		{100, 100, 100, 300, 100, 300, 223, 300, 300, 300,
		300, 300, 300, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // "value=", = 223
		{100, 100, 100, 100, 100, 100, 100, 224, 224, 224,
		224, 224, 224, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100}, // I AB = 224
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 225, 100, 100,
		100, 100, 100, 100}, // = E AC = 225
		{100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 210, 100, 100, 100, 100,
		100, 100, 100, 100},
		{ 100, 100, 100, 100, 100, 100, 100, 226, 226, 226,
		226, 226, 226, 226, 226, 226, 226, 226, 226, 226,
		226, 226, 226, 226, 100, 100, 100, 100, 226, 226,
		100, 100, 100, 100 }, //  T EB = 226
		{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 300, 300, 100, 100, 227, 228,
		100, 100, 100, 100 }, // + T EB = 227, - T EB = 228
		{ 100, 100, 100, 100, 100, 100, 100, 229, 229, 229,
		229, 229, 229, 229, 229, 229, 229, 229, 229, 229,
		229, 229, 229, 229, 100, 100, 100, 100, 229, 229,
		100, 100, 100, 100 }, // F TB = 229
		{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 300, 300, 100, 100, 300, 300,
		230, 231, 100, 100 }, // * F TB = 230, / F TB = 231
		{ 100, 100, 100, 100, 100, 100, 100, 232, 232, 232,
		232, 232, 232, 233, 233, 233, 233, 233, 233, 233,
		233, 233, 233, 234, 100, 100, 100, 100, 223, 223,
		100, 100, 100, 100 }, // I = 232, N = 233, ( E ) = 234
		{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 235, 235, 235, 235, 235, 235, 235,
		235, 235, 235, 235, 100, 100, 100, 100, 235, 235,
		100, 100, 100, 100 }, // S D NB = 235
		{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 236, 236, 236, 236, 236, 236, 236,
		236, 236, 236, 100, 300, 300, 100, 100, 300, 300,
		300, 300, 100, 100 }, // D NB 236
		{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 300, 300, 300, 300, 300, 300, 300,
		300, 300, 300, 100, 100, 100, 100, 100, 237, 238,
		100, 100, 100, 100 }, // + = 237, - = 238
		{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 239, 240, 241, 242, 243, 244, 245,
		246, 247, 248, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100 }, // 0 = 239, 1 = 240, 2 = 241, 3 = 242, 4 = 243, 5 = 244, 6 = 245, 7 = 246, 8 = 247, 9 = 248
		{ 100, 100, 100, 100, 100, 100, 100, 249, 250, 251,
		252, 253, 254, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100 }, // a = 249, b = 250, c = 251, d = 252, l = 253, f = 254
	};

	int goTo(int row, int col) {
		return table[row][col];
	}
	int stateToRowNumber(string c) {
		if (c == "P")
			return 0;
		else if (c == "PB")
			return 1;
		else if (c == "PC")
			return 2;
		else if (c == "PD")
			return 3;
		else if (c == "PE")
			return 4;
		else if (c == "I")
			return 5;
		else if (c == "IB")
			return 6;
		else if (c == "DL")
			return 7;
		else if (c == "DLB")
			return 8;
		else if (c == "DLC")
			return 9;
		else if (c == "DC")
			return 10;
		else if (c == "DCB")
			return 11;
		else if (c == "TP")
			return 12;
		else if (c == "SL")
			return 13;
		else if (c == "SLB")
			return 14;
		else if (c == "ST")
			return 15;
		else if (c == "W")
			return 16;
		else if (c == "WB")
			return 17;
		else if (c == "WC")
			return 18;
		else if (c == "WD")
			return 19;
		else if (c == "WE")
			return 20;
		else if (c == "WF")
			return 21;
		else if (c == "SR")
			return 22;
		else if (c == "A")
			return 23;
		else if (c == "AB")
			return 24;
		else if (c == "AC")
			return 25;
		else if (c == "E")
			return 26;
		else if (c == "EB")
			return 27;
		else if (c == "T")
			return 28;
		else if (c == "TB")
			return 29;
		else if (c == "F")
			return 30;
		else if (c == "N")
			return 31;
		else if (c == "NB")
			return 32;
		else if (c == "S")
			return 33;
		else if (c == "D")
			return 34;
		else if (c == "L")
			return 35;
		else
			return -1;

	}
	int symbolToColNumber(string c) {
		if (c == "program")
			return 0;
		else if (c == "var")
			return 1;
		else if (c == "begin")
			return 2;
		else if (c == "end.")
			return 3;
		else if (c == "integer")
			return 4;
		else if (c == "print")
			return 5;
		else if (c == "\"Value=\",")
			return 6;
		else if (c == "a")
			return 7;
		else if (c == "b")
			return 8;
		else if (c == "c")
			return 9;
		else if (c == "d")
			return 10;
		else if (c == "l")
			return 11;
		else if (c == "f")
			return 12;
		else if (c == "0")
			return 13;
		else if (c == "1")
			return 14;
		else if (c == "2")
			return 15;
		else if (c == "3")
			return 16;
		else if (c == "4")
			return 17;
		else if (c == "5")
			return 18;
		else if (c == "6")
			return 19;
		else if (c == "7")
			return 20;
		else if (c == "8")
			return 21;
		else if (c == "9")
			return 22;
		else if (c == "(")
			return 23;
		else if (c == ")")
			return 24;
		else if (c == ";")
			return 25;
		else if (c == ":")
			return 26;
		else if (c == "=")
			return 27;
		else if (c == "+")
			return 28;
		else if (c == "-")
			return 29;
		else if (c == "*")
			return 30;
		else if (c == "/")
			return 31;
		else if (c == "$")
			return 32;
		else if (c == ",")
			return 33;
		else
			return -1;
	}
	bool pushToStack(int x) {
		switch (x) {
		case 100:
			return false;
		case 300:
			printStack();
			return true;
		case 201:
			myStack.push_back("PB");
			myStack.push_back("I");
			myStack.push_back("program");
			printStack();
			return true;
		case 202:
			myStack.push_back("PC");
			myStack.push_back(";");
			printStack();
			return true;
		case 203:
			myStack.push_back("PD");
			myStack.push_back("DL");
			myStack.push_back("var");
			printStack();
			return true;
		case 204:
			myStack.push_back("PE");
			myStack.push_back("SL");
			myStack.push_back("begin");
			printStack();
			return true;
		case 205:
			myStack.push_back("end.");
			printStack();
			return true;
		case 206:
			if (mainCode && readingIdent == false) {
				readingIdent = true;
				tempIdent = "";
			}
			myStack.push_back("IB");
			myStack.push_back("L");
			printStack();
			return true;
		case 207:
			if (mainCode && readingIdent == false) {
				readingIdent = true;
				tempIdent = "";
			}
			myStack.push_back("IB");
			myStack.push_back("D");
			printStack();
			return true;
		case 208:
			myStack.push_back("DLB");
			myStack.push_back("DC");
			printStack();
			return true;
		case 209:
			myStack.push_back("DLC");
			myStack.push_back("TP");
			myStack.push_back(":");
			printStack();
			return true;
		case 210:
			myStack.push_back(";");
			printStack();
			return true;
		case 211:
			myStack.push_back("DCB");
			myStack.push_back("I");
			printStack();
			return true;
		case 212:
			myStack.push_back("DC");
			myStack.push_back(",");
			printStack();
			return true;
		case 213:
			myStack.push_back("integer");
			printStack();
			return true;
		case 214:
			myStack.push_back("SLB");
			myStack.push_back("ST");
			printStack();
			return true;
		case 215:
			myStack.push_back("SL");
			printStack();
			return true;
		case 216:
			myStack.push_back("W");
			printStack();
			return true;
		case 217:
			myStack.push_back("A");
			printStack();
			return true;
		case 218:
			myStack.push_back("WB");
			myStack.push_back("print");
			printStack();
			return true;
		case 219:
			myStack.push_back("WC");
			myStack.push_back("(");
			printStack();
			return true;
		case 220:
			myStack.push_back("WD");
			myStack.push_back("SR");
			printStack();
			return true;
		case 221:
			myStack.push_back("WE");
			myStack.push_back("I");
			printStack();
			return true;
		case 222:
			myStack.push_back("WF");
			myStack.push_back(")");
			printStack();
			return true;
		case 223:
			myStack.push_back("\"Value=\",");
			printStack();
			return true;
		case 224:
			myStack.push_back("AB");
			myStack.push_back("I");
			printStack();
			return true;
		case 225:
			myStack.push_back("AC");
			myStack.push_back("E");
			myStack.push_back("=");
			printStack();
			return true;
		case 226:
			myStack.push_back("EB");
			myStack.push_back("T");
			printStack();
			return true;
		case 227:
			myStack.push_back("EB");
			myStack.push_back("T");
			myStack.push_back("+");
			printStack();
			return true;
		case 228:
			myStack.push_back("EB");
			myStack.push_back("T");
			myStack.push_back("-");
			printStack();
			return true;
		case 229:
			myStack.push_back("TB");
			myStack.push_back("F");
			printStack();
			return true;
		case 230:
			myStack.push_back("TB");
			myStack.push_back("F");
			myStack.push_back("*");
			printStack();
			return true;
		case 231:
			myStack.push_back("TB");
			myStack.push_back("F");
			myStack.push_back("/");
			printStack();
			return true;
		case 232:
			myStack.push_back("I");
			printStack();
			return true;
		case 233:
			myStack.push_back("N");
			printStack();
			return true;
		case 234:
			myStack.push_back(")");
			myStack.push_back("E");
			myStack.push_back("(");
			printStack();
			return true;
		case 235:
			myStack.push_back("NB");
			myStack.push_back("D");
			myStack.push_back("S");
			printStack();
			return true;
		case 236:
			myStack.push_back("NB");
			myStack.push_back("D");
			printStack();
			return true;
		case 237:
			myStack.push_back("+");
			printStack();
			return true;
		case 238:
			myStack.push_back("-");
			printStack();
			return true;
		case 239:
			myStack.push_back("0");
			printStack();
			return true;
		case 240:
			myStack.push_back("1");
			printStack();
			return true;
		case 241:
			myStack.push_back("2");
			printStack();
			return true;
		case 242:
			myStack.push_back("3");
			printStack();
			return true;
		case 243:
			myStack.push_back("4");
			printStack();
			return true;
		case 244:
			myStack.push_back("5");
			printStack();
			return true;
		case 245:
			myStack.push_back("6");
			printStack();
			return true;
		case 246:
			myStack.push_back("7");
			printStack();
			return true;
		case 247:
			myStack.push_back("8");
			printStack();
			return true;
		case 248:
			myStack.push_back("9");
			printStack();
			return true;
		case 249:
			myStack.push_back("a");
			printStack();
			return true;
		case 250:
			myStack.push_back("b");
			printStack();
			return true;
		case 251:
			myStack.push_back("c");
			printStack();
			return true;
		case 252:
			myStack.push_back("d");
			printStack();
			return true;
		case 253:
			myStack.push_back("l");
			printStack();
			return true;
		case 254:
			myStack.push_back("f");
			printStack();
			return true;
		default:
			return false;
		}
	}
	void printStack() {
		for (string item : myStack) {
			cout << item << "|";
		}
		cout << endl;
	}
public:
	bool accepted(vector<string> input)
	{
		cout << "---CheckingGrammar---" << endl;
		int i = 0;
		myStack.clear();
		myStack.push_back("$");
		myStack.push_back("P");
		printStack();
		string nextInStack;
		while (!myStack.empty())
		{

			nextInStack = myStack.back();
			myStack.pop_back();
			//cout<<"["<<nextInStack<<","<<input[i]<<"]"<<endl; 
			if (nextInStack == "$" && input[i] == "$") {
				cout << "No Error" << endl;
				return true;
			}
			else if (symbolToColNumber(nextInStack) != -1) {
				if (nextInStack == input[i]) {
					i++;
					//cout<< "match! "<< nextInStack<<endl; 
					if (nextInStack == "var") {
						readingVar = true;
					}
					else if (readingVar == true) {
						if (nextInStack == ":") {
							vars.push_back(tempVar);
							tempVar = "";
							readingVar = false;
							mainCode = true;
						}
						else if (nextInStack == ",") {
							vars.push_back(tempVar);
							tempVar = "";
						}
						else
							tempVar.push_back(nextInStack[0]);
					}
					else if (readingIdent && mainCode) {
						if (symbolToColNumber(nextInStack) <= 22 &&
							symbolToColNumber(nextInStack) >= 7) {
							tempIdent.push_back(nextInStack[0]);
						}
						else {
							bool hasIt = false;
							for (string vari : vars)
							{
								if (vari == tempIdent)
									hasIt = true;
							}
							if (!hasIt)
							{
								cout << "some errors" << endl;
								cout << "\"" << tempIdent << "\" unknown identifier" << endl;
								return false;
							}
							tempIdent = "";
							readingIdent = false;
						}
					}
				}
				else {
					cout << "some errors" << endl;
					cout << nextInStack << "is expected" << endl;
					return false;
				}
			}
			else if (!pushToStack(goTo(stateToRowNumber(nextInStack),
				symbolToColNumber(input[i]))))
			{
				cout << "some errors" << endl;
				bool program = false;
				bool var = false;
				bool begin = false;
				bool end = false;
				bool integer = false;
				bool print = false;
				bool value = false;
				int p_count = 0;
				for (int i = 0; i < input.size(); i++) {
					if (input[i] == "program")
						program = true;
					else if (input[i] == "var")
						var = true;
					else if (input[i] == "begin")
						begin = true;
					else if (input[i] == "end.")
						end = true;
					else if (input[i] == "integer")
						integer = true;
					else if (input[i] == "print")
						print = true;
					else if (input[i] == "\"Value=\",")
						value = true;
					else if (input[i] == "(")
						p_count++;
					else if (input[i] == ")")
						p_count--;
				}
				if (!program)
					cout << "program is expected" << endl;
				if (!var)
					cout << "var is expected" << endl;
				if (!begin)
					cout << "begin is expected" << endl;
				if (!end)
					cout << "end. is expected" << endl;
				if (!integer)
					cout << "integer is expected" << endl;
				if (!print)
					cout << "print is expected" << endl;				
				//how to check if print is mispelled 
				while (!myStack.empty()) {
					if (nextInStack == "WB" || p_count < 0) {
						cout << "( is missing" << endl;
						return false;
					}
					else if (nextInStack == "WE" || p_count > 0)
					{
						cout << ") is missing" << endl;
						return false;
					}
					else if (nextInStack == "DCB")
					{
						cout << ", is missing" << endl;
						return false;
					}
					else if (nextInStack == "PB" || nextInStack == "DLC" || nextInStack == "WF" || nextInStack == "AC")
					{
						cout << "; is missing" << endl;
						return false;
					}
					nextInStack = myStack.back();
					myStack.pop_back();
				}
				return false;
			}
		}
		return false;
	}
};

class CompilerFinal {
private:
	ifstream finalv1;
	vector<string> parsedInput;
public:
	void run() {
		parse();
		myLanguage l;
		l.accepted(parsedInput);
	}

	void parse() {
		ifstream finalv2("final24.txt");
		string myLine;
		string myString;
		while (finalv2 >> myLine) {
			// Output the text from the file 
			string parsedString = "";
			for (int i = 0; i < myLine.length(); i++) {
				if (
					myLine[i] == '(' ||
					myLine[i] == ')' ||
					/*myLine[i] == ',' ||*/
					myLine[i] == '*' ||
					myLine[i] == '+' ||
					myLine[i] == '-' ||
					myLine[i] == '/' ||
					myLine[i] == '{' ||
					myLine[i] == '}' ||
					myLine[i] == ':' ||
					myLine[i] == ';') {
					if (parsedString != "") {
						if (parsedString == "program" || parsedString == "var" || parsedString == "begin" || parsedString == "end." || parsedString == "integer" || parsedString == "print" || parsedString == "\"Value=\",")
							parsedInput.push_back(parsedString);
						else if (parsedString == "\"Value=\",") {
							parsedInput.push_back("\"Value=\",");
							parsedString = "";
							continue;
						}
						else
						{
							for (int i = 0; i < parsedString.length(); i++)
							{
								parsedInput.push_back(string(1, parsedString[i]));
							}
						}
					}
					parsedInput.push_back(string(1, myLine[i]));
					parsedString = "";
				}
				else {
					parsedString.push_back(myLine[i]);
				}
			}
			if (parsedString != "") {
				if (parsedString == "program" || parsedString == "var" || parsedString == "begin" || parsedString == "end." || parsedString == "integer" || parsedString == "print" || parsedString == "\"Value=\",")
					parsedInput.push_back(parsedString);
				else {
					for (int i = 0; i < parsedString.length(); i++)
					{
						parsedInput.push_back(string(1, parsedString[i]));
					}
				}
			}
		}
		parsedInput.push_back("$");
		cout << "-----Parsed Program-----" << endl;
		for (auto i : parsedInput)
			std::cout << i << '|';
		cout << endl << "------------------------" << endl;
		finalv2.close();
	}
};
int main() {
	CompilerFinal comp;
	comp.run();
	return 0;
}