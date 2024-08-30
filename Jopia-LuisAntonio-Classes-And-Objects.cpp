#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class Book {
private:
    string bookTitle;
    string bookAuthor;
    int bookYearPublished;

public:
    // Default constructor
    Book() : bookTitle(""), bookAuthor(""), bookYearPublished(0) {}

    // Parameterized constructor
    Book(const string& inputTitle, const string& inputAuthor, int inputYearPublished)
        : bookTitle(inputTitle), bookAuthor(inputAuthor), bookYearPublished(inputYearPublished) {}

    // Set book details
    void addBook(const string& inputTitle, const string& inputAuthor, int inputYearPublished) {
        bookTitle = inputTitle;
        bookAuthor = inputAuthor;
        bookYearPublished = inputYearPublished;
    }

    // Display book details
    void displayBook() const {
        cout << "    Title: " << bookTitle << endl;
        cout << "    Author: " << bookAuthor << endl;
        cout << "    Year: " << bookYearPublished << endl;
    }

    // Search book by title
    bool searchBook(const string& inputTitle) const {
        return bookTitle == inputTitle;
    }

    // Getters
    string getTitle() const { return bookTitle; }
    string getAuthor() const { return bookAuthor; }
    int getYearPublished() const { return bookYearPublished; }

    // Write book details to a file
    void writeToFile(ofstream& outFile) const {
        outFile << bookTitle << endl;
        outFile << bookAuthor << endl;
        outFile << bookYearPublished << endl;
    }

    // Read book details from a file
    void readFromFile(ifstream& inFile) {
        getline(inFile, bookTitle);
        getline(inFile, bookAuthor);
        inFile >> bookYearPublished;
        inFile.ignore(); // Ignores newline character after year
    }
};

class Library {
private:
    static const int maximumQuantityOfBooks = 1000;
    Book bookCollection[maximumQuantityOfBooks];
    int bookCount;

public:
    // Constructor
    Library() : bookCount(0) {}

    // Load books from the file at the start
    void loadBooksFromFile() {
        ifstream inFile("Library-Management-System-Data.txt");
        if (inFile.is_open()) {
            while (!inFile.eof() && bookCount < maximumQuantityOfBooks) {
                bookCollection[bookCount].readFromFile(inFile);
                if (inFile) { // Only increment if read is successful
                    bookCount++;
                }
            }
            inFile.close();
        }
    }

    // Add a book to the library and save to the file
    void addBook(const string& inputTitle, const string& inputAuthor, int inputYearPublished) {
        if (bookCount < maximumQuantityOfBooks) {
            bookCollection[bookCount].addBook(inputTitle, inputAuthor, inputYearPublished);
            bookCount++;
            cout << "    Book added successfully!" << endl;

            // Save the new book to the file
            ofstream outFile("Library-Management-System-Data.txt", ios::app);
            if (outFile.is_open()) {
                bookCollection[bookCount - 1].writeToFile(outFile);
                outFile.close();
            }
        } else {
            cout << "    Library is full. Cannot add more books." << endl;
        }
    }

    // Display all books in the library
    void displayAllBooks() const {
        ifstream inFile("Library-Management-System-Data.txt");
        if (!inFile.is_open()) {
            cout << "No books in the library or unable to open the file." << endl;
            return;
        }

        string title, author;
        int year;
        
        // Set the column widths for a neat table
        cout << endl;
        cout << left << setw(25) << "Title"
             << setw(25) << "Author"
             << setw(5) << "Year" << endl;
        cout << "-----------------------------------------------------------" << endl;

        // Read the file and display its contents
        while (getline(inFile, title) && getline(inFile, author) && (inFile >> year)) {
            inFile.ignore(); // Ignore the newline after year
            cout << left << setw(25) << title
                 << setw(25) << author
                 << setw(5) << year << endl;
        }

        inFile.close();
        cout << endl;
    }

    // Search for a book by title
	void searchBook() const {
	    string inputTitle;
	    char searchAgain;
	    do {
	        cout << "    Enter a book title to search: ";
	        getline(cin, inputTitle);
	        
	        // Convert search input to lowercase
	        string lowerInputTitle = toLowerCase(inputTitle);
	
	        bool found = false;
	        for (int i = 0; i < bookCount; i++) {
	            // Convert book title to lowercase for comparison
	            if (toLowerCase(bookCollection[i].getTitle()) == lowerInputTitle) {
	                cout << "    Book found!" << endl;
	                bookCollection[i].displayBook();
	                found = true;
	                break;
	            }
	        }
	
	        if (!found) {
	            cout << "    Book not found!" << endl;
	        }
	
	        // Ensure only 'Y' or 'N' are accepted
	        do {
	            cout << "    Search Book Again [Y/N]? ";
	            cin >> searchAgain;
	            cin.ignore(); // Ignore newline character
	            if (searchAgain != 'Y' && searchAgain != 'y' && searchAgain != 'N' && searchAgain != 'n') {
	                cout << "    Invalid input. Please enter 'Y' or 'N'." << endl;
	            }
	        } while (searchAgain != 'Y' && searchAgain != 'y' && searchAgain != 'N' && searchAgain != 'n');
	        
	    } while (searchAgain == 'Y' || searchAgain == 'y');
	    
	    cout << "    Press enter to go back to Menu" << endl;
	    cin.get(); // Wait for user to press a key
	}

    // Convert a string to lowercase
	string toLowerCase(const string& input) const {
	    string lowerCaseStr = input;
	    for (char& c : lowerCaseStr) {
	        c = tolower(c);
	    }
	    return lowerCaseStr;
	}

    int getBookCount() const { return bookCount; }
    Book getBook(int index) const { return bookCollection[index]; }
};

// Function to check if a string represents a valid year (all digits and within range)
bool isValidYear(const string& input, int& year) {
    // Check if all characters are digits
    for (char c : input) {
        if (!isdigit(c)) {
            return false;
        }
    }
    // Convert string to integer
    year = 0;
    for (char c : input) {
        year = year * 10 + (c - '0');
    }
    // Check range
    return (year >= 1000 && year <= 2024);
}

int main() {
    Library myLibrary;
    myLibrary.loadBooksFromFile(); // Load existing books from file at startup
    int choice;
    string title, author, yearString;
    int yearInt;

    do {
        // Display menu
        cout << "================ LIBRARY MANAGEMENT SYSTEM ================" << endl << endl;
        cout << "Welcome to the Library Management System! Your gateway" << endl;
        cout << "to seamless library services." << endl << endl;
        cout << "===========================================================" << endl << endl;
        cout << "                           MENU" << endl;
        cout << "    1 - Add Book" << endl;
        cout << "    2 - Display Book" << endl;
        cout << "    3 - Search Book" << endl;
        cout << "    4 - Exit" << endl << endl;
        
        // Validating menu input
        cout << "Please select an option to get started (1-4): ";
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "Invalid input. Please select a number between 1 and 4." << endl;
            cout << "Please select an option to get started (1-4): ";
            cin.clear(); // Clear the error state
            // Basic method to clear input buffer
            char ch;
            while (cin.get(ch) && ch != '\n'); // Discard characters until newline
        }
        cout << endl;
        cin.ignore(); // Ignore newline character left in the input buffer

        switch (choice) {
            case 1:
                // Add Book
                cout << "= ADD BOOK ================================================" << endl << endl;
                cout << "    Enter title: ";
                getline(cin, title);
                cout << "    Enter author: ";
                getline(cin, author);
                do {
                    cout << "    Enter year: ";
                    getline(cin, yearString);
                    if (!isValidYear(yearString, yearInt)) {
                        cout << "    Invalid input. Please enter a year, 2024 or earlier." << endl;
                    }
                } while (!isValidYear(yearString, yearInt));
                myLibrary.addBook(title, author, yearInt);
                cout << "    Press enter to go back to Menu" << endl << endl;
                cin.get(); // Wait for user to press enter key
                break;

            case 2:
                // Display All Books
                cout << "= DISPLAY BOOK ============================================" << endl;
                myLibrary.displayAllBooks();
                cout << "Press enter to go back to Menu" << endl;
                cin.get(); // Wait for user to press a key
                break;

            case 3:
                // Search Book
                cout << "= SEARCH BOOK =============================================" << endl << endl;
                myLibrary.searchBook();
                break;

            case 4:
                // Exit
                cout << "Thank you for using our LMS! Have a great day, and we hope" << endl;
                cout << "to see you again soon!" << endl;
                break;
        }
    } while (choice != 4);

    return 0;
}