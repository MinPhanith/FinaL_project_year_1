#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <conio.h>
#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <windows.h>
#include <ctime> // For capturing current date and time

using namespace std;

struct User {
    string username;
    string password;
};

struct Book {
    int id;
    string name;
    string author;
    int pages;
    int yearReleased;
    int availableCopies;
};

vector<Book> library;
vector<User> users;

class LibraryManager {
private:
    string loggedInUser;
    int nextBookID;
public:
    LibraryManager() : loggedInUser(""), nextBookID(1) {}

    void loadUsers() {
        ifstream infile("users.txt");
        if (infile.is_open()) {
            User user;
            while (infile >> user.username >> user.password) {
                users.push_back(user);
            }
            infile.close();
        }
    }

    void saveUsers() {
        ofstream outfile("users.txt", ios::app);
        if (outfile.is_open()) {
            for (const auto& user : users) {
                outfile << user.username << " " << user.password << endl;
            }
            outfile.close();
        }
    }

    void loadBooks() {
        ifstream infile("books.txt");
        if (infile.is_open()) {
            Book book;
            while (infile >> book.id >> book.name >> book.author >> book.pages >> book.yearReleased >> book.availableCopies) {
                library.push_back(book);
                if (book.id >= nextBookID) {
                    nextBookID = book.id + 1;
                }
            }
            infile.close();
        }
    }

    void saveBooks() {
        ofstream outfile("books.txt");
        if (outfile.is_open()) {
            for (const auto& book : library) {
                outfile << book.id << " " << book.name << " " << book.author << " "
                        << book.pages << " " << book.yearReleased << " "
                        << book.availableCopies << endl;
            }
            outfile.close();
        }
    }

    string inputWithMask() {
        string input;
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b' && !input.empty()) {
                cout << "\b \b";
                input.pop_back();
            } else {
                input.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        return input;
    }

    bool authenticate(const string& username, const string& password) {
        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                return true;
            }
        }
        return false;
    }

    bool login() {
        string username, password;

        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);

        cout << "Enter password: ";
        password = inputWithMask();

        if (authenticate(username, password)) {
            cout << "Login successful." << endl;
            loggedInUser = username;
            return true;
        } else {
            cout << "Invalid username or password." << endl;
            return false;
        }
    }

    void registerUser() {
        User newUser;
        cout << "Enter new username: ";
        cin >> newUser.username;
        cout << "Enter new password: ";
        newUser.password = inputWithMask();
        users.push_back(newUser);
        saveUsers();
        cout << "User registered successfully!" << endl;
    }

    void viewBookList() {
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "Author"
             << setw(10) << "Pages" << setw(15) << "Year Released" << setw(15)
             << "Available Copies" << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;
        for (const auto& book : library) {
            cout << setw(5) << book.id << setw(20) << book.name << setw(20) << book.author
                 << setw(10) << book.pages << setw(15) << book.yearReleased << setw(15)
                 << book.availableCopies << endl;
        }
    }

    void viewBookDetails() {
        string bookName;
        cout << "Enter book ID or Name: ";
        cin.ignore();
        getline(cin, bookName);
        try {
            int i = stoi(bookName);
            for (auto& book : library) {
                if (book.id == i) {
                    cout << "Name: " << book.name << endl;
                    cout << "Author: " << book.author << endl;
                    cout << "Pages: " << book.pages << endl;
                    cout << "Year Released: " << book.yearReleased << endl;
                    cout << "Available Copies: " << book.availableCopies << endl;
                    return;
                }
            }
        } catch (const invalid_argument& e) {
            for (auto& book : library) {
                if (book.name == bookName) {
                    cout << "Name: " << book.name << endl;
                    cout << "Author: " << book.author << endl;
                    cout << "Pages: " << book.pages << endl;
                    cout << "Year Released: " << book.yearReleased << endl;
                    cout << "Available Copies: " << book.availableCopies << endl;
                    return;
                }
            }
        } catch (const out_of_range& e) {
            cout << "Input number is out of range" << endl;
        }
        cout << "Book not found!" << endl;
    }

    void addBook() {
        Book newBook;
        newBook.id = nextBookID++;
        cout << "Enter book name: ";
        cin.ignore();
        getline(cin, newBook.name);
        cout << "Enter author name: ";
        getline(cin, newBook.author);
        cout << "Enter number of pages: ";
        cin >> newBook.pages;
        cout << "Enter year released: ";
        cin >> newBook.yearReleased;
        cout << "Enter available copies: ";
        cin >> newBook.availableCopies;
        library.push_back(newBook);
        saveBooks();
        cout << "Book added successfully!" << endl;
    }

    void deleteBook() {
        string bookID;
        cout << "Enter book ID or Name to delete: ";
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (it->id == i) {
                    library.erase(it);
                    saveBooks();
                    cout << "Book deleted successfully!" << endl;
                    return;
                }
            }
        } catch (const invalid_argument& e) {
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (it->name == bookID) {
                    library.erase(it);
                    saveBooks();
                    cout << "Book deleted successfully!" << endl;
                    return;
                }
            }
        } catch (const out_of_range& e) {
            cout << "Input number is out of range." << endl;
        }
        cout << "Book not found!" << endl;
    }

    void editBookAmount() {
        string bookID;
        int newAmount;
        cout << "Enter book ID or Name to edit: ";
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto& book : library) {
                if (book.id == i) {
                    cout << "Enter new amount: ";
                    cin >> newAmount;
                    book.availableCopies = newAmount;
                    saveBooks();
                    cout << "Book amount updated successfully!" << endl;
                    return;
                }
            }
        } catch (const invalid_argument& e) {
            for (auto& book : library) {
                if (book.name == bookID) {
                    cout << "Enter new amount: ";
                    cin >> newAmount;
                    book.availableCopies = newAmount;
                    saveBooks();
                    cout << "Book amount updated successfully!" << endl;
                    return;
                }
            }
        } catch (const out_of_range& e) {
            cout << "Input number is out of range" << endl;
        }

        cout << "Book not found!" << endl;
    }

    void lendBook() {
        string bookID;
        cout << "Enter book ID to lend: ";
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto& book : library) {
                if (book.id == i && book.availableCopies > 0) {
                    book.availableCopies--;
                    saveBooks();
                    cout << "Book lent successfully!" << endl;
                    return;
                }
            }
        } catch (const invalid_argument& e) {
            for (auto& book : library) {
                if (book.name == bookID && book.availableCopies > 0) {
                    book.availableCopies--;
                    saveBooks();
                    cout << "Book lent successfully!" << endl;
                    return;
                }
            }
        } catch (const out_of_range& e) {
            cout << "Input number is out of range" << endl;
        }

        cout << "Book not found or no copies available!" << endl;
    }

    void returnBook() {
        string bookID;
        string studentID, studentName;

        cout << "Enter student ID: ";
        cin >> studentID;
        cout << "Enter student name: ";
        cin.ignore(); // Clear input buffer
        getline(cin, studentName);

        cout << "Enter book ID or Name to return: ";
        getline(cin, bookID);

        try {
            int i = stoi(bookID);
            for (auto& book : library) {
                if (book.id == i) {
                    book.availableCopies++;
                    saveBooks();
                    saveReturnDetails(book, studentID, studentName);
                    cout << "Book returned successfully!" << endl;
                    return;
                }
            }
        } catch (const invalid_argument& e) {
            for (auto& book : library) {
                if (book.name == bookID) {
                    book.availableCopies++;
                    saveBooks();
                    saveReturnDetails(book, studentID, studentName);
                    cout << "Book returned successfully!" << endl;
                    return;
                }
            }
        } catch (const out_of_range& e) {
            cout << "Input number is out of range." << endl;
        }
        cout << "Book not found!" << endl;
    }

    void saveReturnDetails(const Book& book, const string& studentID, const string& studentName) {
        ofstream outfile("returns.txt", ios::app);
        if (outfile.is_open()) {
            // Get the current date and time
            time_t now = time(0);
            char* dt = ctime(&now);

            // Save student details, book info, and return date/time
            outfile << "Student ID: " << studentID << ", Student Name: " << studentName
                    << ", Book ID: " << book.id << ", Book Name: " << book.name
                    << ", Return Date/Time: " << dt << endl;

            outfile.close();
        } else {
            cout << "Error opening returns file!" << endl;
        }
    }

    void logout() {
        loggedInUser = "";
        cout << "Logged out successfully!" << endl;
    }

    bool isLoggedIn() {
        return !loggedInUser.empty();
    }

    string getLoggedInUser() {
        return loggedInUser;
    }
};

// Menu function
void displayMenu(LibraryManager& manager) {
    int choice;
    do {
        cout << "\nLibrary Management System Menu\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Add Book\n";
        cout << "4. View Book List\n";
        cout << "5. View Book Details\n";
        cout << "6. Lend Book\n";
        cout << "7. Return Book\n";
        cout << "8. Delete Book\n";
        cout << "9. Edit Book Amount\n";
        cout << "10. Logout\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.registerUser();
                break;
            case 2:
                manager.login();
                break;
            case 3:
                if (manager.isLoggedIn()) {
                    manager.addBook();
                } else {
                    cout << "Please login first!" << endl;
                }
                break;
            case 4:
                manager.viewBookList();
                break;
            case 5:
                manager.viewBookDetails();
                break;
            case 6:
                if (manager.isLoggedIn()) {
                    manager.lendBook();
                } else {
                    cout << "Please login first!" << endl;
                }
                break;
            case 7:
                if (manager.isLoggedIn()) {
                    manager.returnBook();
                } else {
                    cout << "Please login first!" << endl;
                }
                break;
            case 8:
                if (manager.isLoggedIn()) {
                    manager.deleteBook();
                } else {
                    cout << "Please login first!" << endl;
                }
                break;
            case 9:
                if (manager.isLoggedIn()) {
                    manager.editBookAmount();
                } else {
                    cout << "Please login first!" << endl;
                }
                break;
            case 10:
                manager.logout();
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 0);
}

// Main function
int main() {
    LibraryManager manager;
    manager.loadUsers();
    manager.loadBooks();
    displayMenu(manager);
    return 0;
}
