#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits> 
using namespace std;

class Vehicle
{
    protected:
    string RegistrationNumber;
    string Make;
    string Model;
    int Age;
    string Type;
    public:
    Vehicle(string registrationNumber, string make, string model, int age): RegistrationNumber(registrationNumber),  Make(make), Model(model), Age(age) {};
    string returnRegistrationNumber() const {return RegistrationNumber;}
    string returnMake() const {return Make;}
    string returnModel() const {return Model;}
    int returnAge() const {return Age;}
    string returnType(){return Type;}

    virtual float CostPerDay() const = 0;
    virtual int returnDoors() const = 0;
    virtual int returnSeats() const = 0;
    virtual int returnEngine() const = 0;
    virtual int returnWheels() const = 0;
     
    // bool operator<(const Vehicle& p) const
    // {
    //     return returnRegistrationNumber() < p.returnRegistrationNumber(); // order by name
    // }
    
   
};

class Car: public Vehicle
{
    
    double Doors;
    double Seats;

    public: 
    Car(string registrationNumber, string make, string model, int age, double doors, double seats): Vehicle(registrationNumber, make, model, age){
        Doors = doors;
        Seats = seats;
        Type = "Car";
    };

    float CostPerDay() const{
        float cost = (2500 - (Age*150)-(Doors*200))/100;
        if(cost<10){
            return 10;
        }
        else{
            return cost;
        }
    }

    int returnDoors()const{return Doors;}  
    int returnSeats() const {return Seats;};
    int returnEngine() const {return -1;};
    int returnWheels() const {return -1;};

};

class Bike: public Vehicle
{
    int roundNearestHundredth(int d) const
    {
        int d_i = d;
        return ((d_i % 100) < 50) ? d_i - (d_i % 100) : d_i + (100 - (d_i % 100));
    }

    protected:
    double EngineSize;
    double Wheels;
   
    
    public: 
    Bike(string registrationNumber , string make, string model, int age, double engineSize, double wheels): Vehicle(registrationNumber, make, model, age){
        EngineSize = engineSize;
        Wheels = wheels;
        Type = "Bike";
    };

    float CostPerDay() const{
        return (1500 + roundNearestHundredth(EngineSize))/100;
    }
    int returnDoors()const{return -1;}  
    int returnSeats() const {return -1;};
    int returnEngine() const {return EngineSize;};
    int returnWheels() const {return Wheels;};
};

struct HistoryRecord{
    string DateStart, DateEnd, Name, Address, PhoneNumber;
    double Cost, NumberOfDays;
    HistoryRecord(string dateStart, string dateEnd, double numberOfDays, double cost, string name, string address, string phoneNumber)
    :DateStart(dateStart), DateEnd(dateEnd), Name(name), Address(address), PhoneNumber(phoneNumber), Cost(cost), NumberOfDays(numberOfDays){};

};

class VehicleHistory
{
    float TotalMoneyGained = 0;
    float TotalDaysRented= 0;
    int TotalRecords = 0;
    int index = 1;
    vector <HistoryRecord*> HistoricalRecordVector;
    public:
    void AddRecord(double CostPerDay){
        string fromDate, toDate;
        int days;
        string name, address, phonenumber;

        cout << "Enter the date you want to start renting: " ;
        cin >> fromDate;
        cout << "\nEnter the date you want to stop renting: ";
        cin >> toDate;
        cout << "\nEnter the number of days you want to rent: ";
        cin >> days;
        TotalMoneyGained = TotalMoneyGained + (CostPerDay*days);
        cout <<"\nEnter your name: ";
        cin >> name;
        cout <<"\nEnter your address: ";
        cin >> address;
        cout <<"\nEnter your phone number: ";
        cin >> phonenumber;
        HistoricalRecordVector.push_back(new HistoryRecord(fromDate, toDate, days, TotalMoneyGained, name, address, phonenumber ));


    }
    
    void printOverview(float CostPerDay){
        cout << "Cost Per Day:         " << char(156) << fixed << setprecision(2) << CostPerDay <<endl;
        cout << "Total Rental Income:  " << char(156) << fixed << setprecision(2) << TotalMoneyGained <<endl;
        cout << "Total Days Rented:    "  << fixed << setprecision(2) << TotalDaysRented <<endl;

    }
    
    void printHistory(string header){
        int userInput = 0;
        
        while(userInput != 9){

            cout << index;
            cout << "\n" <<header;
            cout << "----------------- \n";
            
            cout << "Rental:             "<< index << " of " << TotalRecords << endl;
            cout << "Date From:          "<< HistoricalRecordVector[index-1]->DateStart << endl;
            cout << "Date To:            "<< HistoricalRecordVector[index-1]->DateEnd << endl;
            cout << "Rental Days:        "<< HistoricalRecordVector[index-1]->NumberOfDays << endl;
            cout << "Total Cost:         "<< HistoricalRecordVector[index-1]->Cost << endl;
            cout << "Customer Name:      "<< HistoricalRecordVector[index-1]->Name << endl;
            cout << "Customer Address:   "<< HistoricalRecordVector[index-1]->Address << endl;
            cout << "Customer Number:    "<< HistoricalRecordVector[index-1]->PhoneNumber << endl;
            
            if(index == 1){
                cout << "\n \n1) View the next record\n9) Return to vehicle information screen \n\nPlease choose an option:";
                cin >> userInput;
              
                if(userInput == 1){
                    index ++;
                }
            }
            else if(index == TotalRecords){
                cout << "\n \n1) View the previous record\n9) Return to vehicle information screen \n\nPlease choose an option:";
                cin >> userInput;
              
                if(userInput == 1){
                    index --;
                }
            }    
            else{
                cout << "\n \n1) View the previous record\n2) View the next record\n9) Return to vehicle information screen \n\nPlease choose an option:";
                cin >> userInput;
              
                if(userInput == 1){
                    index --;
                }
                else if(userInput == 2){
                    index ++;
                }


            }
            
        }
        
    }

    VehicleHistory(string registrationNumber){
        ifstream History;
        History.open("./History/"+registrationNumber+".csv");
        if(History.fail()){
            cerr << "No history found for this file" << endl;
                throw;
        }
        string line;
        while(getline(History,line)){
            stringstream inputString(line);
            string StartingDate, EndDate, NumberOfDays, TotalCost, Name, Address, PhoneNumber;
             
            getline(inputString, StartingDate,',');
            getline(inputString, EndDate,',');
            getline(inputString, NumberOfDays,',');
            getline(inputString, TotalCost,',');
            getline(inputString, Name,',');
            getline(inputString, Address,',');
            getline(inputString, PhoneNumber,',');
            TotalRecords++;
            TotalMoneyGained = TotalMoneyGained + stod(TotalCost);
            TotalDaysRented = TotalDaysRented + stod(NumberOfDays);

            HistoricalRecordVector.push_back(new HistoryRecord(StartingDate, EndDate, stod(NumberOfDays), stod(TotalCost), Name, Address, PhoneNumber ));
        }
        History.close();

    }    

    // friend ostream& operator<<(ostream& os, const VehicleHistory& dt);

};

class AllVehicles
{
    vector<Vehicle*> VehicleVector; 

    void SortRegistrationNumber(){
        sort( VehicleVector.begin( ), VehicleVector.end( ), [ ]( const auto& lhs, const auto& rhs ){
            return lhs->returnRegistrationNumber() < rhs->returnRegistrationNumber();
        });
    }
 
    void SortCost(){
        sort( VehicleVector.begin( ), VehicleVector.end( ), [ ]( const auto& lhs, const auto& rhs ){
            return lhs->CostPerDay() < rhs->CostPerDay();
        });
    }

    void AddVehicle(){
        int option;
        string RegistrationNumber, Make, Model;
        int Age;
        cout << "Add a vehicle:" <<endl << "--------------" <<endl <<"1) Add a car"<<endl<<"2) Add a bike"<<endl <<endl << "Please choose and option: ";
        cin >> option;
       
    
        while(option != 1 && option != 2){
            cout << "Enter a valid input" <<endl << "Please choose and option: ";
            cin >> option;
        }
        if(option == 1){

            //
            int Doors, Seats;
            cout << "Adding a car:" <<endl << "Please enter a valid 7 characted registration number. It must be unique of the form AB12XYZ: " ;
            cin >> RegistrationNumber; //Make sure that its unique and that its not present in the vector already.
            cout << "Enter Make: ";
            cin >> Make;
            cout << "Enter Model: ";
            cin >> Model;
            cout << "Enter Age: ";
            cin >> Age;
            cout << "Enter Doors: ";
            cin >> Doors;
            cout << "Enter Seat: ";
            cin >> Seats;
            VehicleVector.push_back(new Car(RegistrationNumber, Make, Model, Age, Doors, Seats));
            ofstream myfile;
            myfile.open ("History/"+RegistrationNumber+".csv"); 
            myfile.close();
  

        }
        else if(option == 2){
            int Engine, Wheels;

            cout << "Adding a bike:" <<endl << "Please enter a valid 7 characted registration number. It must be unique of the form AB12 XYZ: " ;
            cin >> RegistrationNumber;
            cout << "Enter Make: ";
            cin >> Make;
            cout << "Enter Model: ";
            cin >> Model;
            cout << "Enter Age: ";
            cin >> Age;
            cout << "Enter Engine: ";
            cin >> Engine;
            cout << "Enter Wheels: ";
            cin >> Wheels;
            VehicleVector.push_back(new Bike(RegistrationNumber, Make, Model, Age, Engine, Wheels));
        
    }

    };
 
    void DeleteVehicle(){

        string VehicleID;
        cout << "Enter the vehicle ID you wish to delete: ";
        cin >> VehicleID;
        auto it = find_if(VehicleVector.begin(), VehicleVector.end(), [&VehicleID](const auto& obj) {return obj->returnRegistrationNumber() == VehicleID;});

        if (it != VehicleVector.end())
        {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        auto index = std::distance(VehicleVector.begin(), it);
        auto begin = VehicleVector.begin();
        begin = begin +index;
    
        VehicleVector.erase(begin);
        }
        else{
            cout << "Vehicle ID not valid" <<endl;
        }

    }

    void SearchForBikeMenu(){
        cout << " \n Search for bike by: \n" << " ---------------- \n";
        cout << "1) Registration number \n2) Max Engine Size\n3) Two or Three Wheeler\n9) Return to main menu \n\nPlease choose an option: "; 
        int userInput = 0;
        int validInput = 0;
        int selectedIndex = 0;
        while(!(validInput)){
            validInput=1;
            while(!(cin >> userInput)){
                cout << "\nBad value! Choose from the options above: " ;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            switch (userInput)
            {
            case 1:
                selectedIndex = AllVehicleSearchBy(1);
                if(selectedIndex != -1){
                    showVehicle(selectedIndex);
                }
                break;
            case 2: 
                selectedIndex = AllVehicleSearchBy(4);
                if(selectedIndex != -1){
                    showVehicle(selectedIndex);
                }
                break;
            case 3: 
                selectedIndex = AllVehicleSearchBy(5);
                if(selectedIndex != -1){
                    showVehicle(selectedIndex);
                }
                break;
            case 9:
                break;
            default:
                validInput=0;
                cout << "Bad Input, choose from the options above ";
                break;
            }
        
        }
        
    }  
    
    void showVehicle(int index){
        cout << VehicleVector[index]->returnRegistrationNumber() << ": " << VehicleVector[index]->returnMake()<<" "<< VehicleVector[index]->returnModel() <<endl;
        cout << "-------------------- \n";
        int userInput = 0;
        VehicleHistory vehicleDetails(VehicleVector[index]->returnRegistrationNumber());
        vehicleDetails.printOverview(VehicleVector[index]->CostPerDay());
        while(userInput !=9){
            cout << "\n1) Rent Vehicle \n2) View historical rentals\n9) Return to main menu \n\nPlease choose an option: "; 
        
            while(!(cin >> userInput)){
                cout << "\nBad value! Choose from the options above: " ;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if(userInput == 1){
                vehicleDetails.AddRecord(VehicleVector[index]->CostPerDay());

            }
            else if(userInput == 2){
                string header =  VehicleVector[index]->returnRegistrationNumber() + ": " + VehicleVector[index]->returnMake() + " " + VehicleVector[index]->returnModel() + " Rental History \n";
                vehicleDetails.printHistory(header);
                
            }
            else{
                cout << "\nBad value! Choose from the options above" ;


            }

        }
        // cout << vehicleDetails;
        
        
    }
   
    void SearchForCarMenu(){
        cout << " \n Search for car by: \n" << " ---------------- \n";
        cout << "1) Registration number \n2) Number of seats\n3) Number of doors\n9) Return to main menu \n\nPlease choose an option: "; 
        int userInput = 0;
        int validInput = 0;
        int selectedIndex = 0;
        while(!(validInput)){
            validInput=1;
            while(!(cin >> userInput)){
                cout << "\nBad value! Choose from the options above: " ;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            switch (userInput)
            {
            case 1:
            
                selectedIndex = AllVehicleSearchBy(1);
                 if(selectedIndex!=-1){
                    showVehicle(selectedIndex);
                }
                
                
                break;
            case 2: 
                selectedIndex = AllVehicleSearchBy(2);
                if(selectedIndex!=-1){
                    showVehicle(selectedIndex);
                }
                
                break;
            case 3: 
                if(selectedIndex != -1){
                    selectedIndex = AllVehicleSearchBy(3);
                }
                showVehicle(selectedIndex);
                break;
            case 9:
                break;
            default:
                validInput=0;
                cout << "Bad Input, choose from the options above ";
                break;
            }
        
        }
        
    }  
    
    int AllVehicleSearchBy(int option){
        //Option 0: Car by door
        //Option 1: Car by seat
        //Option 2: Bike by engine
        //Option 3: Bike by wheel
        //Option 4: By registration Number
        int searchVariable;
        int userInput = -1;
        vector<int> indices;
        auto it = VehicleVector.begin();
        string RegistrationNumber;

      
        switch (option)
        {
        case 1:
            cout << "\nEnter the registration number: ";
            cin >> RegistrationNumber;
            while ((it = find_if(it, VehicleVector.end(), [&] (const auto &e) { return e->returnRegistrationNumber() ==  RegistrationNumber; }))
            != VehicleVector.end())
            {
                indices.push_back(distance(VehicleVector.begin(), it));
                it++;
            }
            break;
        case 2:
            cout << "\nEnter the number of seats: ";
            cin >> searchVariable;
            while ((it = find_if(it, VehicleVector.end(), [&] (const auto &e) { return e->returnSeats() ==  searchVariable; }))
            != VehicleVector.end())
            {
                indices.push_back(distance(VehicleVector.begin(), it));
                it++;
            }
            break;
        case 3:
            cout << "\nEnter the number of doors: ";
            cin >> searchVariable;
            while ((it = find_if(it, VehicleVector.end(), [&] (const auto &e) { return e->returnDoors() ==  searchVariable; }))
            != VehicleVector.end())
            {
                indices.push_back(distance(VehicleVector.begin(), it));
                it++;
            }
            break;
        case 4:
            cout << "\nMax engine size: ";
            cin >> searchVariable;
            while ((it = find_if(it, VehicleVector.end(), [&] (const auto &e) { return e->returnEngine() ==  searchVariable; }))
            != VehicleVector.end())
            {
                indices.push_back(distance(VehicleVector.begin(), it));
                it++;
            }
            break;
        case 5:
            cout << "\nEnter the number of wheels (2-3): ";
            cin >> searchVariable;
            while ((it = find_if(it, VehicleVector.end(), [&] (const auto &e) { return e->returnWheels() ==  searchVariable; }))
            != VehicleVector.end())
            {
                indices.push_back(distance(VehicleVector.begin(), it));
                it++;
            }
            break;
       
        default:
            cout << "program broken, please contact developer";
            break;
        }
        
        cout << "\nList of cars matching search: \n\n";
        cout<< "\tRegistration Number \t Cost Per Day \t Make \t Model\n";
        cout<< "\t ----------------- \t ------------ \t ---- \t ------- \n";

        int iterator = 1;
        for (auto &e: indices) {
            cout << iterator << " \t " << VehicleVector[e]->returnRegistrationNumber() << " \t \t     " << char(156) << fixed << setprecision(2) << VehicleVector[e]->CostPerDay() << " \t " << VehicleVector[e]->returnMake() << " \t " << VehicleVector[e]->returnModel() << endl ;
            iterator = iterator +1;
        }
        cout << "Enter number to choose vehicle or 0 to return to main menu: ";

        while(!(cin >> userInput) || (userInput > iterator || userInput < 0 )){
            cout << "Bad value! Choose from the options above: " ;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if(userInput != 0){
            return indices[userInput-1];
        }
        return -1;
        

        
    }
 
    public:
    AllVehicles(){
        string fname = "VehicleList.csv";
        ifstream VehicleList;
        VehicleList.open(fname);

        if(VehicleList.fail()){
            cerr << "Could not load file" << endl;
                throw;

        }
        string line;
        while(getline(VehicleList,line)){
            stringstream inputString(line);
            string RegistrationNumber, Type, Make, Model, Age, Doors, Seats, Engine, Wheels;
            getline(inputString, RegistrationNumber,',');
            getline(inputString, Type,',');
            getline(inputString, Make,',');
            getline(inputString, Model,',');
            getline(inputString, Age,',');
            getline(inputString, Doors,',');
            getline(inputString, Seats,',');
            getline(inputString, Engine,',');
            getline(inputString, Wheels,',');
            if(Type == "Car"){
                VehicleVector.push_back(new Car(RegistrationNumber, Make, Model, stoi(Age), stoi(Doors), stoi(Seats)));
            }
            else{
                VehicleVector.push_back(new Bike(RegistrationNumber, Make, Model, stoi(Age), stoi(Engine), stoi(Wheels)));
            }
           

        }
        VehicleList.close();

    };
  
    ~AllVehicles(){
        VehicleVector.clear();
    }
    
    void MainMenu(){ 
        int userInput=0;
        while (userInput != 9){
            cout << "Vehicle Rental System" << endl << "---------------------" << endl;

            cout << "Registration Number \t Cost Per Day \t Vehicle Type" << endl << "---------------- \t ---------- \t ------------" << endl;
         
            for (auto* s : VehicleVector){ cout << s->returnRegistrationNumber()<< "\t \t \t "<< char(156) << fixed << setprecision(2) << s->CostPerDay()<< "\t \t    " << s->returnType()  << endl;} 
            cout << endl<< "1) Add Vehicle \n2) Remove Vehicle \n3) Search for car \n4) Search for bike \n5) Sort vehicles by registration number \n6) Sort by cost per day \n9) Exit" <<endl;
            cout << "Please choose an option : ";
            while(!(cin >> userInput)){
                cout << "Bad value! Choose from the options above \n" ;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            switch(userInput){
                case 1:
                    AddVehicle();
                    break;
                case 2:
                    DeleteVehicle();
                    break;
                case 3:
                    SearchForCarMenu();
                    break;
                case 4: 
                    SearchForBikeMenu();
                    break;
                case 5: 
                    SortRegistrationNumber();
                    break;
                case 6:
                    SortCost();
                    break;
                case 9:
                    cout << "Thank you for using the product!";
                    break;
                default:
                    cout << "Please enter a valid input from the list!" <<endl;
                    break;
            }
            


        }
        
    
    }
    
    friend ostream& operator<<(ostream& os, const AllVehicles& dt);

};

ostream& operator<<(ostream& os, const AllVehicles& dt)
{
    ofstream myfile;
    myfile.open ("VehicleList.csv");
    
    for (auto* s : dt.VehicleVector){ 
        myfile << s->returnRegistrationNumber()<< ","<< s->returnType()<< ","<<s->returnMake()<< ","<< s->returnModel() << ","<< s->returnAge() << ","<<s->returnDoors() << ","<< s->returnSeats() << ","<< s->returnEngine() << ","<< s->returnWheels() << endl; 
    }
    myfile.close();

    return os;
}

// ostream& operator<<(ostream& os, const VehicleHistory& dt)
// {
//     ofstream myfile;
//     myfile.open ("sample1.csv");
    
//     for (auto* a : dt.HistoricalRecordVector){ 
//         myfile << a->DateStart()<< ","<< a->DateEnd()<< ","<< a->NumberOfDays()<< ","<< a->Cost() << ","<< a->Name() << ","<<a->Address() << ","<< a->PhoneNumber()  << endl; 
//     }
//     myfile.close();

//     return os;
// }

int main(){
    AllVehicles lista;
    lista.MainMenu();
    cout << lista;

    
  
}


//Taking in inputs with space for add vehicles,
