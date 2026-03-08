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

class Client {
private:
	string name;
	int tableNumber;
public:
	Client(string n = "Guest", int table = 0) : name(n), tableNumber(table) {
		cout << "Client " << name << " came to the table #" << tableNumber << endl;
	}
	~Client() {
		cout << "Client " << name << " went away" << endl;
	}
	void showClient() {
		cout << "Client: " << name << " (Table #" << tableNumber << ")" << endl;
	}
};

class Order {
private:
	int orderID;
	string status;
public:
	Order(int id, string s = "New") : orderID(id), status(s) {
		cout << "Order #" << orderID << " created with status: " << status << endl;
	}
	~Order() {
		cout << "Order #" << orderID << " deleted from memory" << endl;
	}

	Order() : Order(0, "Empty") {}

	void showOrder() {
		cout << "Order ID: " << orderID << ", Status: " << status << endl;
	}
};

int main() {
	Dish soup("Borscht", "Red and tasty", 120.5);
	Dish water("Water", 25.0);
	Dish empty;
	Client ulyana("Uliana", 5); 
	Client guest;
	Order myOrder(101); 
	Order emptyOrder;
}