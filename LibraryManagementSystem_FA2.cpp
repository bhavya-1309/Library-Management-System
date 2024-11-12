#include <iostream>
#include <string>
#include <regex>
#include <ctime>
using namespace std;

class LibraryManagementSystem;

class Book {
private:
    string title;
    string author;
    string ISBN;
public:
    Book* next;

    void getdata() {
        cout << "\nEnter book title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter author name: ";
        getline(cin, author);
        cout << "Enter ISBN (13 digits): ";
        cin >> ISBN;
        while (!isValidISBN(ISBN)) {
            cout << "Invalid ISBN. Please enter a valid 13-digit ISBN: ";
            cin >> ISBN;
        }
        cout << endl;
    }

    void display() {
        cout << "Title: " << title << ", Author: " << author << ", ISBN: " << ISBN << endl;
    }

    static bool isValidISBN(const string& isbn) {
        return (isbn.length() == 13 && all_of(isbn.begin(), isbn.end(), ::isdigit));
    }

    friend class LibraryManagementSystem;
};

class BorrowedBook {
public:
    string studentName;
    string bookTitle;
    string borrowDate;
    string returnDate;
    string studentPRN;
    string libraryCardNo;
    BorrowedBook* next;

    void getdata() {
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, studentName);
        cout << "Enter PRN: ";
        cin >> studentPRN;
        cout << "Enter library card number: ";
        cin >> libraryCardNo;
        cout << "Enter borrow date (dd-mm-yyyy): ";
        cin >> borrowDate;
        while (!isValidDate(borrowDate)) {
            cout << "Invalid date format. Enter date as dd-mm-yyyy: ";
            cin >> borrowDate;
        }
        cout << "Return the book after exactly 20 days.\n\n";
    }

    void display() {
        cout << "Borrowed Book: " << bookTitle << " by " << studentName
             << " (PRN: " << studentPRN << ", Library Card: " << libraryCardNo << ")"
             << " (Borrowed on: " << borrowDate << ", Return by: " << returnDate << ")" << endl;
    }

    static bool isValidDate(const string& date) {
        return regex_match(date, regex("\\d{2}-\\d{2}-\\d{4}"));
    }
};

class BookStack {
private:
    Book* top;
public:
    BookStack() : top(NULL) {}

    void push(Book* book) {
        book->next = top;
        top = book;
        cout << "Book added to the library stack.\n\n";
    }

    Book* pop() {
        if (top == NULL) {
            cout << "\nNo books to remove from the library stack!\n\n";
            return NULL;
        }
        Book* removedBook = top;
        top = top->next;
        cout << "\nBook removed from the library stack.\n\n";
        return removedBook;
    }

    void displayLibraryStack() {
        if (top == NULL) {
            cout << "\nNo books in the library stack!\n\n";
            return;
        }
        cout << "\nBooks in the Library Stack (Top first):\n";
        Book* current = top;
        while (current != NULL) {
            current->display();
            current = current->next;
        }
        cout << endl;
    }

    Book* getTop() const { return top; }
};

class BookQueue {
private:
    BorrowedBook* front;
    BorrowedBook* rear;
public:
    BookQueue() : front(NULL), rear(NULL) {}

    void enqueue(BorrowedBook* book) {
        book->next = NULL;
        if (rear == NULL) {
            front = rear = book;
        } else {
            rear->next = book;
            rear = book;
        }
        cout << "\nBook borrowed and added to the borrowing queue.\n\n";
    }

    BorrowedBook* dequeue() {
        if (front == NULL) {
            cout << "\nNo borrowed books to return!\n\n";
            return NULL;
        }
        BorrowedBook* book = front;
        front = front->next;
        if (front == NULL) rear = NULL;
        cout << "\nBook returned.\n\n";
        return book;
    }

    void displayBorrowedBooks() {
        if (front == NULL) {
            cout << "\nNo borrowed books!\n\n";
            return;
        }
        cout << "\nBorrowed Books in Queue:\n";
        BorrowedBook* current = front;
        while (current != NULL) {
            current->display();
            current = current->next;
        }
        cout << endl;
    }

    BorrowedBook* getFront() const { return front; }
};

class ReturnedBookQueue {
private:
    BorrowedBook* front;
    BorrowedBook* rear;
public:
    ReturnedBookQueue() : front(NULL), rear(NULL) {}

    void enqueue(BorrowedBook* book) {
        book->next = NULL;
        if (rear == NULL) {
            front = rear = book;
        } else {
            rear->next = book;
            rear = book;
        }
    }

    void displayReturnedBooks() {
        if (front == NULL) {
            cout << "\nNo returned books!\n\n";
            return;
        }
        cout << "\nReturned Books:\n";
        BorrowedBook* current = front;
        while (current != NULL) {
            current->display();
            current = current->next;
        }
        cout << endl;
    }
};

class LibraryManagementSystem {
private:
    BookStack libraryStack;
    BookQueue borrowedBooksQueue;
    ReturnedBookQueue returnedBooksQueue;

public:
    void addBookToLibrary() {
        Book* newBook = new Book;
        newBook->getdata();
        libraryStack.push(newBook);
    }

    void arrangeBooksInShelf() {
        Book* removedBook = libraryStack.pop();
        if (removedBook) {
            cout << "Arranged in Shelf: ";
            removedBook->display();
        }
    }

    void borrowBook() {
        BorrowedBook* newBorrow = new BorrowedBook;
        cout << "\nEnter book title to borrow: ";
        cin.ignore();
        getline(cin, newBorrow->bookTitle);
        newBorrow->getdata();
        borrowedBooksQueue.enqueue(newBorrow);
    }

    void returnBook() {
        BorrowedBook* returnedBook = borrowedBooksQueue.dequeue();
        if (returnedBook) {
            returnedBooksQueue.enqueue(returnedBook);
        }
    }

    void displayBorrowedBooks() {
        borrowedBooksQueue.displayBorrowedBooks();
    }

    void displayReturnedBooks() {
        returnedBooksQueue.displayReturnedBooks();
    }

    void displayBooks() {
        libraryStack.displayLibraryStack();
    }

    void searchBookByTitleOrAuthor(const string& keyword) {
        Book* current = libraryStack.getTop();
        bool found = false;
        while (current != NULL) {
            if (current->title == keyword || current->author == keyword) {
                current->display();
                found = true;
            }
            current = current->next;
        }
        if (!found) {
            cout << "No book found with the title or author '" << keyword << "'\n";
        }
    }

    void displayOverdueBooks() {
        BorrowedBook* current = borrowedBooksQueue.getFront();
        cout << "\nOverdue Books:\n";
        bool found = false;
        while (current != NULL) {
            if (isOverdue(current->borrowDate)) {
                current->display();
                found = true;
            }
            current = current->next;
        }
        if (!found) {
            cout << "No overdue books.\n";
        }
    }

    void generateReport() {
        cout << "\nLibrary Transaction Report:\n";
        cout << "Total Books in Library:\n";
        displayBooks();
        cout << "\nBorrowed Books:\n";
        displayBorrowedBooks();
        cout << "\nReturned Books:\n";
        displayReturnedBooks();
    }

    void updateBookInfo(const string& isbn) {
        Book* current = libraryStack.getTop();
        while (current != NULL) {
            if (current->ISBN == isbn) {
                cout << "\nUpdating book info for ISBN " << isbn << ":\n";
                current->getdata();
                return;
            }
            current = current->next;
        }
        cout << "Book with ISBN " << isbn << " not found.\n";
    }

    void markBookAsLost(const string& isbn) {
        Book* prev = NULL;
        Book* current = libraryStack.getTop();
        while (current != NULL) {
            if (current->ISBN == isbn) {
                cout << "\nBook with ISBN " << isbn << " is marked as lost and removed from the library stack.\n";
                if (prev) prev->next = current->next;
                else libraryStack.push(current->next);
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
        cout << "Book with ISBN " << isbn << " not found.\n";
    }

    bool isOverdue(const string& borrowDate) {
        return false; // Placeholder
    }
};

int main() {
    LibraryManagementSystem library;
    int choice;
    do {
        cout << "Library Management System\n";
        cout << "1. Add Book to Library\n";
        cout << "2. Arrange Books in Shelf\n";
        cout << "3. Display Books\n";
        cout << "4. Borrow Book\n";
        cout << "5. Return Book\n";
        cout << "6. Display Borrowed Books\n";
        cout << "7. Display Returned Books\n";
        cout << "8. Search Book by Title/Author\n";
        cout << "9. Display Overdue Books\n";
        cout << "10. Generate Transaction Report\n";
        cout << "11. Update Book Information\n";
        cout << "12. Mark Book as Lost\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        string keyword, isbn;
        switch (choice) {
            case 1:
                library.addBookToLibrary();
                break;
            case 2:
                library.arrangeBooksInShelf();
                break;
            case 3:
                library.displayBooks();
                break;
            case 4:
                library.borrowBook();
                break;
            case 5:
                library.returnBook();
                break;
            case 6:
                library.displayBorrowedBooks();
                break;
            case 7:
                library.displayReturnedBooks();
                break;
            case 8:
                cout << "Enter title or author to search: ";
                cin.ignore();
                getline(cin, keyword);
                library.searchBookByTitleOrAuthor(keyword);
                break;
            case 9:
                library.displayOverdueBooks();
                break;
            case 10:
                library.generateReport();
                break;
            case 11:
                cout << "Enter ISBN to update: ";
                cin >> isbn;
                library.updateBookInfo(isbn);
                break;
            case 12:
                cout << "Enter ISBN to mark as lost: ";
                cin >> isbn;
                library.markBookAsLost(isbn);
                break;
            case 0:
                cout << "Exiting the Library Management System.\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);
    return 0;
}

