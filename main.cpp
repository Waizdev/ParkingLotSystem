#include <iostream>

using namespace std;

struct Car{
	int carID;
};
class OverflowWaitlist {
    struct Node {
        Car car;
        Node* next;
    };
    Node* head;
	Node *tail;

public:
    OverflowWaitlist() {
	 head = tail = nullptr;
	}

    void addToWaitlist(Car car) {
        Node* newNode = new Node{ car, nullptr };
        if (!tail){
		head = tail = newNode;
	}
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    Car removeFromWaitlist() {
        if (!head){
		 return { -1 };
	}
        Car car = head->car;
        Node* temp = head;
        head = head->next;
        delete temp;
        if (!head){
		 tail = nullptr;
	}
        return car;
    }
    void displayWaitlist() {
        if (isEmpty()) {
            cout << "Waitlist is empty.\n";
            return;
        }
        cout << "Cars in waitlist: ";
        Node* temp = head;
        while (temp) {
            cout << temp->car.carID << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    bool isEmpty() {
	 return head == nullptr; 
	 }
};

class FloorManager {
    Car* cars; 
    int maxCapacity;
    int top; 

public:
    FloorManager(int capacity) {
        maxCapacity = capacity;
        cars = new Car[maxCapacity];
        top = -1;
    }

    ~FloorManager() {
	 delete[] cars;
	  }

    bool parkCar(Car car) {
        if (top >= maxCapacity - 1){
		 return false; 
	}
        cars[++top] = car;
        return true;
    }

    bool removeCar(int carID) {
        int index = -1;
        for (int i = 0; i <= top; i++) {
            if (cars[i].carID == carID) {
                index = i;
                break;
            }
        }
        if (index == -1) 
		return false; 

        for (int i = index; i < top; i++) {
            cars[i] = cars[i + 1];
        }
        top--;
        return true;
    }

    bool isFull() {
	 return top >= maxCapacity - 1;
	  }
    bool isEmpty() {
	 return top == -1;
	  }

    Car* getCars() {
        return cars;
    }

    int getTop() {
        return top;
    }
};

class EntryExitManager {
    Car* entryQueue;
    int front, rear, size, capacity;
    OverflowWaitlist waitlist;

public:
    EntryExitManager(int c) {
        capacity = c;
        entryQueue = new Car[capacity];
        front = 0;
        rear = -1;
        size = 0;
    }

    ~EntryExitManager() { 
	delete[] entryQueue;
	 }

    void addCar(Car car) {
        if (size >= capacity) {
            waitlist.addToWaitlist(car);
        } else {
            rear = (rear + 1) % capacity;
            entryQueue[rear] = car;
            size++;
        }
    }

    Car processNextCar() {
        if (size == 0){
		 return  {-1} ;
	}
        Car car = entryQueue[front];
        front = (front + 1) % capacity;
        size--;
        return car;
    }

    void addToWaitlist(Car car) {
	 waitlist.addToWaitlist(car);
	  }
    Car getFromWaitlist() { 
	return waitlist.removeFromWaitlist();
	 }
	  void displayWaitlist() {
        waitlist.displayWaitlist();
    }
};

class ParkingLot {
    FloorManager** floors; 
    int numFloors;
    int floorCapacity;
    EntryExitManager entryExitManager;

public:
    ParkingLot(int n, int f, int queueSize)
        : entryExitManager(queueSize) {
        numFloors = n;
        floorCapacity = f;

        
        floors = new FloorManager*[numFloors];
        for (int i = 0; i < numFloors; i++) {
            floors[i] = new FloorManager(floorCapacity);
        }
    }

    ~ParkingLot() {
        for (int i = 0; i < numFloors; i++) {
            delete floors[i];
        }
        delete[] floors;
    }

    void addCar(int carID) {
        Car car = { carID };

        for (int i = 0; i < numFloors; i++) {
            if (!floors[i]->isFull()) {
                floors[i]->parkCar(car);
                return;
            }
        }

        entryExitManager.addToWaitlist(car);
    }

    void removeCar(int carID) {
        bool carRemoved = false;
        for (int i = 0; i < numFloors; i++) {
            if (floors[i]->removeCar(carID)) {
                carRemoved = true;
                cout << "Car with ID " << carID << " removed from Floor " << i + 1 << endl;
                Car nextCar = entryExitManager.getFromWaitlist();
                if (nextCar.carID != -1) {
                    addCar(nextCar.carID);
                }
                break;
            }
        }
        if (!carRemoved) {
            cout << "Car not found in parking lot.\n";
        }
    }
    void display_waitlist(){
    	  entryExitManager.displayWaitlist();
	}

    void displayStatus() {
        cout << "Total Floors: " << numFloors << endl;
        for (int i = 0; i < numFloors; i++) {
            cout << "Floor " << i + 1 << endl;
            if (!floors[i]->isEmpty()) {
                Car* cars = floors[i]->getCars();
                for (int j = 0; j <= floors[i]->getTop(); j++) {
                    cout << "   Car ID: " << cars[j].carID << ", Position: " << (j + 1) << endl;
                }
            } else {
                cout << "   No cars parked on this floor.\n";
            }
        }
    }
};

int main() {
    ParkingLot lot(3, 5, 10);  
        cout << "\n       Welcome to Parking Lot System";
    while (true) {

        cout << "\n  ___________________________________";
        cout << "\n | Make your choice in numbers please |";
        cout << "\n |___________________________________ |";
        cout << "\n | 1. Add a car                       |";
        cout << "\n |___________________________________ |";
        cout << "\n | 2. Remove a car                    |";
        cout << "\n |___________________________________ |";
        cout << "\n | 3. Get parking lot status          |";
        cout << "\n |___________________________________ |";
		cout << "\n | 4. display waitlist                |";
		cout << "\n |___________________________________ |";
        cout << "\n | 5. Exit                            |";
        cout << "\n |___________________________________ |";
        cout << endl;
        cout << "Enter your choice: ";
        
        int choice, carID;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter car ID: ";
                cin >> carID;
                lot.addCar(carID);
                break;
            }
            case 2: {
                cout << "Enter car ID to remove: ";
                cin >> carID;
                lot.removeCar(carID);
                break;
            }
            case 3: {
                lot.displayStatus();
                break;
            }
            case 4:
         		lot.display_waitlist();
         		break;
            case 5: {
                cout << "Exiting system.\n";
                return 0;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
    return 0;
}

