//Version 2 of PCB
//Uses four pointers to keep track of children and their siblings

#ifndef VERSION2PCB_H
#define VERSION2PCB_H
class Version2PCB
{
private:
	int youngerSib,
		olderSib,
		parent,
		firstChild;

public:

	Version2PCB() {
		parent = -1;
		youngerSib = -1;
		olderSib = -1;
		firstChild = -1;
	}

	Version2PCB(const int p) {
		parent = p;
		youngerSib = -1;
		olderSib = -1;
		firstChild = -1;
	}

	void setYoungSib(const int ys) { youngerSib = ys; }
	void setOlderSib(const int os) { olderSib = os; }
	void setFirstChild(const int fc) { firstChild = fc; }

	int getYoungSib()const { return youngerSib; }
	int getParent()const { return parent; }
	int getOlderSib()const { return olderSib; }
	int getFirstChild()const { return firstChild; }
};
#endif