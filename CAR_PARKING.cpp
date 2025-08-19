#include <iostream>
#include <string>
using namespace std;

class Car {
	public:
    string licensePlate;
    Car* next;
    Car(const string &Plate)
    {
        licensePlate = Plate;
        next = NULL;
    }
};

// Stack (used to represent cars on each floor)
class Stack {
    private:
    Car* top;
    public:
    Stack()
    {
        top = NULL;
    }
    void push(const string &Plate) {
        Car* newCar = new Car(Plate);
        newCar->next = top;
        top = newCar;
    }
    void pop() {
        if (top) {
            Car* temp = top;
            top = top->next;
            delete temp;
        }
    }

    bool isEmpty() const{ 
	return top == NULL;
	 }
    Car* peek() const { 
	return top;
	 }
};

// Queue (used for entry queue and overflow waitlist)
class Queue {
    private:
    Car* front;
    Car* rear;
    public:
    Queue()
    {
        front = NULL;
        rear = NULL;
    }
    void enqueue(const string &Plate) {
        Car* newCar = new Car(Plate);
        if (rear) {
            rear->next = newCar;
            rear = newCar;
        } else {
            front = rear = newCar;
        }
    }

    void dequeue() {
        if (front) {
            Car* temp = front;
            front = front->next;
            if (!front) rear = NULL;
            delete temp;
        }
    }

    bool isEmpty() const { 
    
	return front == NULL; 
	
	}

    Car* getFront() const {
	
	 return front; 
	 
	 }
};

// Main Class of ParkingLot
class ParkingLot {
private:
    int maxCarsPerFloor;
    int floorsCount;
    Stack* floors;
    Queue entryQueue;
    Queue overflowWaitlist;

public:
    ParkingLot(int floorCount, int maxCars)
    {
        floorsCount = floorCount;
        maxCarsPerFloor = maxCars;
        floors = new Stack[floorsCount];
    }


    // Add a car to the parking lot
    void addCar(const string& licensePlate) {
     // Try to park the car in the first available floor
        for (int i = 0; i < floorsCount; ++i) {
            int count = 0;
            Car* temp = floors[i].peek();
            while (temp) {
                count++;
                temp = temp->next;
            }

            if (count < maxCarsPerFloor) {
                floors[i].push(licensePlate);
                cout << "Car " << licensePlate << " parked on floor " << i + 1 << "\n";
                return;
            }
        }
        // If no space on any floor, add to entry queue
        if (entryQueue.isEmpty()) {
            entryQueue.enqueue(licensePlate);
            cout << "Car " << licensePlate << " added to entry queue.\n";
        } else {
            overflowWaitlist.enqueue(licensePlate);
            cout << "Car " << licensePlate << " added to overflow waitlist.\n";
        }
    }
    // Remove a car from the parking lot
    void removeCar(const string& licensePlate) {
        for (int i = 0; i < floorsCount; ++i) {
            Stack tempStack;
            bool found = false;
            // Search for the car on the floor
            while (!floors[i].isEmpty()) {
                if (floors[i].peek()->licensePlate == licensePlate) {
                    floors[i].pop();
                    cout << "Car " << licensePlate << " removed from floor " << i + 1 << "\n";
                    found = true;
                    break;
                }
                tempStack.push(floors[i].peek()->licensePlate);
                floors[i].pop();
            }
            // Restore cars to the floor
            while (!tempStack.isEmpty()) {
                floors[i].push(tempStack.peek()->licensePlate);
                tempStack.pop();
            }

            if (found) {
                // Move car from entry queue if available
                if (!entryQueue.isEmpty()) {
                    floors[i].push(entryQueue.getFront()->licensePlate);
                    cout << "Car " << entryQueue.getFront()->licensePlate << " moved from entry queue to floor " << i + 1 << "\n";
                    entryQueue.dequeue();
                }
                // Otherwise move from overflow waitlist to entry queue
                else if (!overflowWaitlist.isEmpty()) {
                    entryQueue.enqueue(overflowWaitlist.getFront()->licensePlate);
                    overflowWaitlist.dequeue();
                }
                return;
            }
        }

        cout << "Car " << licensePlate << " not found in the parking lot.\n";
    }

    // Display the status of the parking lot
    void displayStatus() const {
    cout << "\n--- [Parking Lot Status] ---\n";
    bool isAnyCarParked = false;

    // Display cars on each floor
    for (int i = 0; i < floorsCount; ++i) {
        cout << "Floor " << i + 1 << ": ";
        Stack tempStack;
        Car* temp = floors[i].peek();
        bool isFloorEmpty = true;

        while (temp) {
            tempStack.push(temp->licensePlate);
            temp = temp->next;
            isFloorEmpty = false;
        }

        if (isFloorEmpty) {
            cout << "No cars parked......";
        } else {
            while (!tempStack.isEmpty()) {
                cout << tempStack.peek()->licensePlate << " ";
                tempStack.pop();
            }
        }

        cout << "\n";

        // Check if at least one car is parked on any floor
        if (!isFloorEmpty) {
            isAnyCarParked = true;
        }
    }

    // If no cars are parked on any floor, display the message
    if (!isAnyCarParked) {
        cout << "\nThere is no car parked in the parking lot.\n";
    }

    // Display entry queue
    cout << "Entry Queue: ";
    Car* temp = entryQueue.getFront();
    if (!temp) {
        cout << "No cars in the entry queue.\n";
    } else {
        while (temp) {
            cout << temp->licensePlate << " ";
            temp = temp->next;
        }
        cout << "\n";
    }

    // Display overflow waitlist
    cout << "Overflow Waitlist: ";
    temp = overflowWaitlist.getFront();
    if (!temp) {
        cout << "No cars in the overflow waitlist.\n";
    } else {
        while (temp) {
            cout << temp->licensePlate << " ";
            temp = temp->next;
        }
        cout << "\n";
    }
}

    // Expand the parking lot by adding more floors
    void expandParkingLot(int additionalFloors) {
        Stack* newFloors = new Stack[floorsCount + additionalFloors];

        // Copy old floors to new larger array
        for (int i = 0; i < floorsCount; ++i) {
            newFloors[i] = floors[i];
        }

        // Delete old floors array and update the pointer
        delete[] floors;
        floors = newFloors;
        floorsCount += additionalFloors;

        cout << "Parking lot expanded by " << additionalFloors << " floors.\n";
    }
};

int main() {
    int floorCount, maxCars;
    cout << "\n->Enter the number of floors in the parking lot: ";
    cin >> floorCount;
    cout << "->Enter the maximum number of cars per floor: ";
    cin >> maxCars;

    ParkingLot parkinglot(floorCount, maxCars);

    int choice;
    do {
        cout << "\n1. Add Car\n2. Remove Car\n3. Display Status\n4. Expand Parking Lot\n5. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string plate;
                cout << "Enter license plate: ";
                cin >> plate;
                parkinglot.addCar(plate);
                break;
            }
            case 2: {
                string plate;
                cout << "Enter license plate to remove: ";
                cin >> plate;
                parkinglot.removeCar(plate);
                break;
            }
            case 3:
                parkinglot.displayStatus();
                break;
            case 4: {
                int additionalFloors;
                cout << "Enter number of additional floors: ";
                cin >> additionalFloors;
                parkinglot.expandParkingLot(additionalFloors);
                break;
            }
            case 5:
                cout << "Exiting from Parkinglot.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
