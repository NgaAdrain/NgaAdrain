#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

class Position {
public:
	Position() {}
	Position(int x, int y) {
		this->x = x;
		this->y = y;
		abs_length = sqrt(abs(pow(x, 2) + pow(y, 2)));
	}
	int retX() { return x; }
	int retY() { return y; }
	float ret_abs() { return abs_length; }
	float length(Position another);
	friend ostream& operator << (ostream& stream, Position& p);
private:
	int x, y;
	float abs_length;
};

float Position::length(Position another) {
	return sqrt(pow(abs(another.retX() - this->x), 2) + pow(abs(another.retY() - this->y), 2));
}

ostream & operator<<(ostream & stream, Position & p)
{
	stream << "( " << p.x << "," << p.y << ") length : " << p.abs_length << endl;
	return stream;
}

bool &operator ==(Position &prev_p, Position &after_p) {
	bool result = (prev_p.retX() == after_p.retX());
	result = result && (prev_p.retY() == after_p.retY());
	result = result && (prev_p.ret_abs() == after_p.ret_abs());
	return result;
}

bool &operator !=(Position &prev_p, Position &after_p) {
	bool result = (prev_p.retX() == after_p.retX());
	result = result && (prev_p.retY() == after_p.retY());
	result = result && (prev_p.ret_abs() == after_p.ret_abs());
	result = ~result;
	return result;
}

class Matrix {
public:
	Matrix() {}
	friend istream& operator >> (istream& stream, Matrix& m);
	friend ostream& operator << (ostream& stream, Matrix& m);
	int path_Search();
	void quick_Sort();
private:
	Position ** pList;
	int num_of_charge=0, destination=0;
};

istream& operator >> (istream& stream, Matrix& m) {
	stream >> m.num_of_charge;
	stream >> m.destination;
	m.pList = new Position*[m.num_of_charge+2];
	m.pList[0] = new Position(0, 0);
	for (int index = 1; index <= m.num_of_charge; index++) {
		int tempx, tempy;
		stream >> tempx;
		stream >> tempy;
		m.pList[index] = new Position(tempx, tempy);
	}
	m.pList[m.num_of_charge + 1] = new Position(m.destination, m.destination);
	return stream;
}

ostream & operator<<(ostream & stream, Matrix & m)
{
	for (int x = 0; x < m.num_of_charge + 2; x++) {
		stream << m.pList[x] << endl;
		stream << *m.pList[x] << endl;
	}
	return stream;
}

int Matrix::path_Search() {
	int w = (int) pList[1]->ret_abs() + 1;
	int x = 1;
	while (1) {
		if (x + 1 == num_of_charge)
			break;
		float temp = pList[x]->length(*pList[x+1]);
		int temp_index = x + 1;
		for (int y = x + 1; y <= num_of_charge; y++) {
			if (temp > pList[x]->length(*pList[y])) {
				temp = pList[x]->length(*pList[y]);
				temp_index = y;
			}
		}
		x = temp_index;
		cout << *pList[x];
		if (temp > w)
			w = (int) temp;
	}
	return w;
}

void Matrix::quick_Sort() {
	Position* front;
	Position* end;
	Position* pivot = pList[num_of_charge/2];
	int front_i = 1, end_i = num_of_charge;
	front = pList[front_i]; end = pList[end_i];
	while(*front != *end || *front != *pivot || *end != *pivot){
		if (front->ret_abs() != end->ret_abs()) {
			if (front->ret_abs() > pivot->ret_abs()) {
				if (end->ret_abs() < pivot->ret_abs()) {
					Position* temp = new Position(end->retX(), end->retY());
					*end = *front;
					*front = *temp;
					end = pList[--end_i];
					delete temp;
				}
				else {
					end = pList[--end_i];
				}
			}
			else { // front <= pivot
				if (pList[front_i]->ret_abs() != pivot->ret_abs()) {
					front = pList[++front_i];
				}
				else{
					front_i += 1;
					front = pList[front_i];
				}
			}
		}
		else { // end 와 front가 같음 => cross 
			if (front->ret_abs() == end->ret_abs() &&  pList[--front_i]->ret_abs() == pivot->ret_abs())
				break;
			else{
				Position* temp = new Position(end->retX(), end->retY());
				*front = *pivot;
				*pivot = *temp;
				delete temp;
				//cout << "기준 : " << *pivot;
				//for(int x = 1; x <= num_of_charge; x++)
				//	cout << *pList[x];
				front_i = 1; 
				front = pList[front_i];
				end_i = num_of_charge;
				end = pList[end_i];
			}
		}
	}
}

void main() {
	Matrix input;
	fstream file;
	file.open("battery.inp", ios::in);
	file >> input;
	file.close();
	cout << input;
	input.quick_Sort();				//수정
	cout << input;
	cout << input.path_Search();	//수정
	//file.open("battery.out", ios::out);
	//file << input.path_Search;
	//file.close();
}

