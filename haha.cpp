#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Book {
public:
    int id;
    string name;
    string author;
    int numberOfCopies;

    Book() {}

    Book(int i, string n, string a, int num) : id(i), name(n), author(a), numberOfCopies(num) {}

    void display() {
        cout << "ID: " << id << ", Name: " << name << ", Author: " << author << ", Copies: " << numberOfCopies << endl;
    }
};

class User {
private:
    string username;
    string password;

public:
    string getUsername() const { return username; }

    void registerUser() {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ofstream outFile("users.txt", ios::app);
        outFile << username << " " << password << endl;
        outFile.close();

        cout << "User registered successfully!\n";
    }

    bool login() {
        string u, p;
        cout << "Enter username: ";
        cin >> u;
        cout << "Enter password: ";
        cin >> p;

        ifstream inFile("users.txt");
        string fileUser, filePass;
        while (inFile >> fileUser >> filePass) {
            if (u == fileUser && p == filePass) {
                username = u;  // Store the logged-in username
                cout << "Login successful!\n";
                inFile.close();
                return true;
            }
        }
        inFile.close();
        cout << "Invalid credentials!\n";
        return false;
    }

    void logout() {
        cout << "Logged out successfully!\n";
    }
};

class Library {
private:
    vector<Book> books;

    void loadBooksFromFile() {
        ifstream inFile("books.txt");
        books.clear();
        Book b;
        while (inFile >> b.id >> ws && getline(inFile, b.name) && getline(inFile, b.author) && inFile >> b.numberOfCopies) {
            books.push_back(b);
        }
        inFile.close();
    }

    void saveBooksToFile() {
        ofstream outFile("books.txt");
        for (Book& b : books) {
            outFile << b.id << endl;
            outFile << b.name << endl;
            outFile << b.author << endl;
            outFile << b.numberOfCopies << endl;
        }
        outFile.close();
    }

    // Save lending details to "lending.txt"
    void saveLendingDetails(const string& username, const string& bookName, int bookId) {
        ofstream outFile("lending.txt", ios::app);
        outFile << username << " borrowed \"" << bookName << "\" (ID: " << bookId << ")\n";
        outFile.close();
    }

public:
    Library() {
        loadBooksFromFile();
    }

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
        saveBooksToFile();
        cout << "Book added successfully!\n";
    }

    void viewBooks() {
        loadBooksFromFile();
        for (Book& b : books) {
            b.display();
        }
    }

    void editBook() {
        int id;
        cout << "Enter Book ID to edit: ";
        cin >> id;

        loadBooksFromFile();
        for (Book& b : books) {
            if (b.id == id) {
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, b.name);
                cout << "Enter new author: ";
                getline(cin, b.author);
                cout << "Enter new number of copies: ";
                cin >> b.numberOfCopies;
                saveBooksToFile();
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

        loadBooksFromFile();
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->id == id) {
                books.erase(it);
                saveBooksToFile();
                cout << "Book deleted successfully!\n";
                return;
            }
        }
        cout << "Book not found!\n";
    }

    // Lend a book and record lending details
    void lendBook(const string& username) {
        int id;
        cout << "Enter Book ID to lend: ";
        cin >> id;

        loadBooksFromFile();
        for (Book& b : books) {
            if (b.id == id && b.numberOfCopies > 0) {
                b.numberOfCopies--;
                saveBooksToFile();
                cout << "Book lent successfully! Remaining copies: " << b.numberOfCopies << endl;

                // Save lending details
                saveLendingDetails(username, b.name, b.id);
                return;
            }
        }
        cout << "Book not available for lending!\n";
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;

        loadBooksFromFile();
        for (Book& b : books) {
            if (b.id == id) {
                b.numberOfCopies++;
                saveBooksToFile();
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
    bool loggedIn = false;

    // User Section (Register/Login)
    int userChoice;
    do {
        cout << "\n--- User Section ---\n";
        cout << "1. Register\n2. Login\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> userChoice;

        switch (userChoice) {
            case 1:
                user.registerUser();
                break;
            case 2:
                loggedIn = user.login();
                break;
            case 0:
                cout << "Exiting the system...\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    } while (!loggedIn);

    // Book Management Section (after login)
    int bookChoice;
    do {
        cout << "\n--- Book Management Section ---\n";
        cout << "1. Add Book\n2. View Books\n3. Edit Book\n4. Delete Book\n5. Lend Book\n6. Return Book\n7. Logout\n";
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
                library.lendBook(user.getUsername());  // Pass username for lending record
                break;
            case 6:
                library.returnBook();
                break;
            case 7:
                user.logout();
                loggedIn = false;
                cout << "Returning to the User Section...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (loggedIn);

    return 0;
}
