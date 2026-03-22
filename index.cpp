#include <iostream>
#include <vector>
#include <string>

using namespace std;

class MenuItem {
protected: 
	string name; 
	double price; 
public:
	MenuItem(string n, double p) : name(n), price(p) { 
		cout << "Base MenuItem created: " << name << endl; 
	} 
	virtual ~MenuItem() { 
		cout << "Base MenuItem destroyed: " << name << endl; 
	}
	virtual void showInfo() { 
		cout << "Item: " << name << " | Price: " << price << " uah" << endl; 
	} 
	void setPrice(double newPrice) { 
		this->price = newPrice; } 
};


class Dish : public MenuItem {
private:
	string description;
	static int totalDishes;//Static
public:
	Dish(string n, string d, double p) : MenuItem(n, p), description(d) { 
		totalDishes++; cout << "Dish created: " << description << endl; 
	}

	Dish() : MenuItem("Unknown", 0.0), description("No description") {
		totalDishes++; cout << "Default Dish created" << endl;
	}

	Dish(string n, double p) : Dish(n, "Regular dish", p) {
		cout << "Short constructor used for " << name << endl;
	}

	~Dish() {
		cout << "Desroyed a dish: " << name << endl;
		cout << "Desroyed a description: " << description << endl;
		cout << "Desroyed a price: " << price << endl;
	}
	Dish(const Dish& other) : MenuItem(other), description(other.description) { totalDishes++; cout << "Dish copied: " << name << " (Copy)" << endl; } //L-value reference 

	Dish(Dish&& other) noexcept : MenuItem(std::move(other)), description(std::move(other.description)) {
		cout << "Moved dish: " << name << endl;
	}// R-value reference move конструктор

	Dish& operator=(const Dish& other) {
		if (this == &other) 
		return *this;

		MenuItem::operator=(other);

		this->description = other.description; 
		cout << "Dish assignment operator (copy) called for " << name << endl; 
		return *this;
	}
	Dish& operator=(Dish&& other) noexcept {
		if (this == &other) 
		return *this; 

		MenuItem::operator=(std::move(other)); 

		this->description = std::move(other.description); 
		cout << "Dish assignment operator (move) called for " << name << endl; 
		return *this; 
	}

	static void showTotal() { 
		cout << "Total dishes created: " << totalDishes << endl; 
	}

	Dish operator+(const Dish& other) { 
		return Dish(this->name + " & " + other.name, "Mixed dish", this->price + other.price); //перевантаження і бінарний
	}
	Dish operator-() {
		this->price *= 0.9; cout << "Applied 10% discount to " << name << endl; return *this;
	}//унарний

	friend ostream& operator<<(ostream& os, const Dish& d) { os << d.name << " (" << d.price << " uah)"; return os; }
	
};


class Soup : public Dish {
public:
	Soup(string n, string d, double p) : Dish(n, d, p) {
		cout << "Soup specialized constructor called!" << endl; 
	}
	~Soup() { 
		cout << "Soup unique destructor called!" << endl; 
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
	Client customer;
	vector<Dish> items;
public:
	Order(int id, Client c) : orderID(id), customer(c), status("New") {
		cout << "Order #" << orderID << " created for "; customer.showClient();
	}
	~Order() {
		cout << "Order #" << orderID << " deleted from memory" << endl;
	}

	void addDish(const Dish& d) { items.push_back(d); }

	void showOrder() {
		cout << "--- Order ID: " << orderID << ", Status: " << status << " ---" << endl; customer.showClient(); cout << "Dishes in order:" << endl; for (const auto& item : items) {
			cout << " - " << item << endl;
		}
	}
	
};

class Restaurant {
private: 
	string restaurantName; 
	vector<MenuItem*> menu;
public: 
	Restaurant(string name) : restaurantName(name) {} void addToMenu(MenuItem* item) { menu.push_back(item); } 
	
	void showFullMenu() { 
		cout << "\n--- Welcome to " << restaurantName << " ---" << endl; for (auto const& item : menu) { item->showInfo(); } 
	} 
};

int Dish::totalDishes = 0;
int main() {
	Restaurant myRest("Astra's Bistro");
	Dish* pasta = new Dish("Pasta", "Carbonara", 150.0);

	Soup* borscht = new Soup("Borscht", "Ukrainian classic", 100.0);

	myRest.addToMenu(pasta);
	myRest.addToMenu(borscht); 
	myRest.showFullMenu();
	Dish genericDish("Basic", "None", 10.0);
	genericDish = *pasta;

	Client ulyana("Uliana", 5);
	Client guest;

	Order myOrder(101, ulyana);
	myOrder.addDish(*pasta);
	myOrder.showOrder();
	myOrder.addDish(*borscht);

	delete pasta; 
	delete borscht; 
	return 0;

}