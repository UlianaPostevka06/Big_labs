#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Dish {
private: 
	string name;
	string description;
	double price;
public:
	Dish(string n = "Unknown", string d = "No description", double p = 0.0) : name(n), description(d), price(p) {
		cout << "Created a dish: " << name << endl;
		cout << "Created a description: " << description << endl;
		cout << "Created a price: " << price << endl;
	}
	Dish(string n, double p): Dish(n, "Regular dish", p) { 
		cout << "Short constructor used for " << name << endl; }
	~Dish() {
		cout << "Desroyed a dish: " << name << endl;
		cout << "Desroyed a description: " << description << endl;
		cout << "Desroyed a price: " << price << endl;
	}
};

int main() {
	Dish soup("Borscht", "Red and tasty", 120.5);
	Dish water("Water", 25.0);
	Dish empty;
}