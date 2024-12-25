#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Book class to store details of books
class Book {
public:
    int id;
    string name;
    string author;
    int numberOfCopies;

    Book(int i, string n, string a, int num) : id(i), name(n), author(a), numberOfCopies(num) {}

    void display() {
        cout << "ID: " << id << ", Name: " << name << ", Author: " << author << ", Copies: " << numberOfCopies << endl;
    }
};

// User class to handle user-related operations
class User {
private:
    string username;
    string password;

public:
    void registerUser() {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        cout << "User registered successfully!\n";
    }

    bool login() {
        string u, p;
        cout << "Enter username: ";
        cin >> u;
        cout << "Enter password: ";
        cin >> p;

        if (u == username && p == password) {
            cout << "Login successful!\n";
            return true;
        } else {
            cout << "Invalid credentials!\n";
            return false;
        }
    }

    void logout() {
        cout << "Logged out successfully!\n";
    }
};

// Library class to handle book management operations
class Library {
private:
    vector<Book> books;

public:
    void addBook() {
        int id, copies;
        string name, author;

        cout << "Enter Book ID: ";
        cin >> id;
        cout << "Enter Book Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Author Name: ";
        getline(cin, author);
        cout << "Enter Number of Copies: ";
        cin >> copies;

        books.push_back(Book(id, name, author, copies));
        cout << "Book added successfully!\n";
    }

    void viewBooks() {
        if (books.empty()) {
            cout << "No books available!\n";
            return;
        }

        for (Book& b : books) {
            b.display();
        }
    }

    void editBook() {
        int id;
        cout << "Enter Book ID to edit: ";
        cin >> id;

        for (Book& b : books) {
            if (b.id == id) {
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, b.name);
                cout << "Enter new author: ";
                getline(cin, b.author);
                cout << "Enter new number of copies: ";
                cin >> b.numberOfCopies;
                cout << "Book details updated!\n";
                return;
            }
        }
        cout << "Book not found!\n";
    }

    void deleteBook() {
        int id;
        cout << "Enter Book ID to delete: ";
        cin >> id;

        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->id == id) {
                books.erase(it);
                cout << "Book deleted successfully!\n";
                return;
            }
        }
        cout << "Book not found!\n";
    }

    void lendBook() {
        int id;
        cout << "Enter Book ID to lend: ";
        cin >> id;

        for (Book& b : books) {
            if (b.id == id && b.numberOfCopies > 0) {
                b.numberOfCopies--;
                cout << "Book lent successfully! Remaining copies: " << b.numberOfCopies << endl;
                return;
            }
        }
        cout << "Book not available for lending!\n";
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;

        for (Book& b : books) {
            if (b.id == id) {
                b.numberOfCopies++;
                cout << "Book returned successfully! Total copies: " << b.numberOfCopies << endl;
                return;
            }
        }
        cout << "Invalid Book ID!\n";
    }
};

int main() {
    Library library;
    User user;
    int choice;
    bool loggedIn = false;

    do {
        cout << "\n1. User Section\n2. Book Management Section\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int userChoice;
                do {
                    cout << "\n--- User Section ---\n";
                    cout << "1. Register\n2. Login\n3. Logout\n0. Back\n";
                    cout << "Enter choice: ";
                    cin >> userChoice;

                    switch (userChoice) {
                        case 1:
                            user.registerUser();
                            break;
                        case 2:
                            loggedIn = user.login();
                            break;
                        case 3:
                            if (loggedIn) {
                                user.logout();
                                loggedIn = false;
                            } else {
                                cout << "You are not logged in!\n";
                            }
                            break;
                    }
                } while (userChoice != 0);
                break;
            }
            case 2: {
                if (!loggedIn) {
                    cout << "Please login first to access the Book Management Section!\n";
                    break;
                }
                int bookChoice;
                do {
                    cout << "\n--- Book Management Section ---\n";
                    cout << "1. Add Book\n2. View Books\n3. Edit Book\n4. Delete Book\n5. Lend Book\n6. Return Book\n0. Back\n";
                    cout << "Enter choice: ";
                    cin >> bookChoice;

                    switch (bookChoice) {
                        case 1:
                            library.addBook();
                            break;
                        case 2:
                            library.viewBooks();
                            break;
                        case 3:
                            library.editBook();
                            break;
                        case 4:
                            library.deleteBook();
                            break;
                        case 5:
                            library.lendBook();
                            break;
                        case 6:
                            library.returnBook();
                            break;
                    }
                } while (bookChoice != 0);
                break;
            }
        }
    } while (choice != 0);

    return 0;
}
