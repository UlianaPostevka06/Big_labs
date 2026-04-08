#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <algorithm> 

using namespace std;

class IOrderProcess { 
public: 
	virtual void confirmOrder() = 0; 
	virtual ~IOrderProcess() {} 
};

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
		this->price = newPrice; 
	} 
	void staticType() {
		cout << "This is a basic MenuItem" << endl; 
	}

	virtual void prepare() = 0; 

	string getName() const { return name; } double getPrice() const { return price; }
};


class Dish : public MenuItem {
private:
	string description;
	static int totalDishes;
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
	}

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
	void staticType() {
		cout << "This is a specific dish" << endl; 
	}
	void showInfo() override final { 
		cout << "Dish: " << name << " | Desc: " << description << endl; 
	}
	void prepare() override { 
		cout << "Cooking " << name << " according to the recipe..." << endl; 
	}

	Dish operator+(const Dish& other) { 
		return Dish(this->name + " & " + other.name, "Mixed dish", this->price + other.price);
	}
	Dish operator-() {
		this->price *= 0.9; cout << "Applied 10% discount to " << name << endl; return *this;
	}

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


class Order : public IOrderProcess {
private:
	int orderID;
	string status;
	Client customer;
	vector<Dish> items;
public:
	void confirmOrder() override {
		cout << "Order #" << orderID << " has been confirmed and sent to kitchen!" << endl; 
	}
	Order(int id, Client c) : orderID(id), customer(c), status("New") {
		cout << "Order #" << orderID << " created for "; customer.showClient();
	}
	~Order() {
		cout << "Order #" << orderID << " deleted from memory" << endl;
	}

	void logOrderToHistory() { 
		ofstream historyFile("order_history.txt", ios::app); 
		historyFile << "Order ID: " << orderID << " | Status: " << status << endl; historyFile.close(); 
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
	vector<unique_ptr<MenuItem>> menu; 
public: 
	Restaurant(string name) : restaurantName(name) {} 
	void addToMenu(unique_ptr<MenuItem> item) { menu.push_back(std::move(item)); } 
	void saveMenuToFile() { ofstream outFile("menu_data.txt"); 
	for (const auto& item : menu) { outFile << item->getName() << " " << item->getPrice() << endl; } outFile.close(); } 
	void loadMenuFromFile() { ifstream inFile("menu_data.txt"); string name; double price; while (inFile >> name >> price) { 
		addToMenu(make_unique<Dish>(name, "Restored from file", price)); 
	} 
	inFile.close(); 
	} 
	void showFullMenu() { cout << "\n--- Welcome to " << restaurantName << " ---" << endl; 
	if (menu.empty()) cout << "The menu is currently empty." << endl; for (const auto& item : menu) { item->showInfo(); } 
	} 
};
int Dish::totalDishes = 0;

void printItem(MenuItem& item) {
	item.showInfo();
}

int main() {
	Restaurant myRest("Crown Restaurant");
	myRest.loadMenuFromFile();
	int roleChoice;
	string password;
	while (true) {
		cout << "\nSelect an account" << endl;
		cout << "1. Admin Login" << endl;
		cout << "2. Customer View" << endl;
		cout << "0. Exit Program" << endl;
		cout << "Select Role: ";
		cin >> roleChoice;
		if (roleChoice == 0)
			break;

		if (roleChoice == 1) {
			cout << "Enter Admin Password: ";
			cin >> password;
			if (password == "admin123")
			{
				int adminAction;
				cout << "\n--- ADMIN PANEL ---" << endl;
				cout << "1. Add New Dish" << endl;
				cout << "2. View All" << endl;
				cin >> adminAction;

				if (adminAction == 1) {
					string n;
					double p;
					cout << "Dish Name: "; cin >> n; cout << "Price: "; cin >> p;

					myRest.addToMenu(make_unique<Dish>(n, "Admin Added", p));
					myRest.saveMenuToFile();

					cout << "Dish added and saved to database." << endl;
				}
			}
			else { cout << "Access Denied: Incorrect Password." << endl; }
		}
		else if (roleChoice == 2) {
			myRest.showFullMenu();
			int customerAction;
			cout << "\n1. Make an Order\n2. Back to Main Menu\nSelect: "; cin >> customerAction;

			if (customerAction == 1) {
				try {
					Client user("Ulyana", 7);
					Order newOrder(505, user);
					newOrder.confirmOrder();
					newOrder.logOrderToHistory();
					cout << "Order processed successfully." << endl;
				}
				catch (const exception& e) { cout << "Error: " << e.what() << endl; }
			}
		}
	}
}