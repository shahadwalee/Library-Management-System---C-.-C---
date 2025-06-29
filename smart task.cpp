#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string BooksFileName = "Books.txt";

void ShowMainMenue();

struct sBook
{
    string BookID;
    string Title;
    string Author;
    int Year;
    int Pages;
    bool MarkForDelete = false;
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
            vString.push_back(sWord);
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
        vString.push_back(S1);

    return vString;
}

sBook ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sBook Book;
    vector<string> vBookData = SplitString(Line, Seperator);

    if (vBookData.size() >= 5)
    {
        Book.BookID = vBookData[0];
        Book.Title = vBookData[1];
        Book.Author = vBookData[2];
        Book.Year = stoi(vBookData[3]);
        Book.Pages = stoi(vBookData[4]);
    }
    return Book;
}

string ConvertRecordToLine(sBook Book, string Seperator = "#//#")
{
    string stBookRecord = "";
    stBookRecord += Book.BookID + Seperator;
    stBookRecord += Book.Title + Seperator;
    stBookRecord += Book.Author + Seperator;
    stBookRecord += to_string(Book.Year) + Seperator;
    stBookRecord += to_string(Book.Pages);
    return stBookRecord;
}

bool BookExistsByID(string BookID, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            sBook Book = ConvertLinetoRecord(Line);
            if (Book.BookID == BookID)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

sBook ReadNewBook()
{
    sBook Book;
    cout << "Enter Book ID? ";
    getline(cin >> ws, Book.BookID);

    while (BookExistsByID(Book.BookID, BooksFileName))
    {
        cout << "\nBook with ID [" << Book.BookID << "] already exists, Enter another ID? ";
        getline(cin >> ws, Book.BookID);
    }

    cout << "Enter Title? ";
    getline(cin, Book.Title);

    cout << "Enter Author? ";
    getline(cin, Book.Author);

    cout << "Enter Publication Year? ";
    cin >> Book.Year;

    cout << "Enter Number of Pages? ";
    cin >> Book.Pages;

    return Book;
}

vector<sBook> LoadBooksDataFromFile(string FileName)
{
    vector<sBook> vBooks;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            sBook Book = ConvertLinetoRecord(Line);
            vBooks.push_back(Book);
        }
        MyFile.close();
    }
    return vBooks;
}

void PrintBookRecordLine(sBook Book)
{
    cout << "| " << setw(10) << left << Book.BookID;
    cout << "| " << setw(25) << left << Book.Title;
    cout << "| " << setw(20) << left << Book.Author;
    cout << "| " << setw(6) << left << Book.Year;
    cout << "| " << setw(6) << left << Book.Pages;
}

void ShowAllBooksScreen()
{
    vector<sBook> vBooks = LoadBooksDataFromFile(BooksFileName);

    cout << "\n\t\t\t\t\tBook List (" << vBooks.size() << ") Book(s).";
    cout << "\n_______________________________\n" << endl;

    cout << "| " << left << setw(10) << "Book ID";
    cout << "| " << left << setw(25) << "Title";
    cout << "| " << left << setw(20) << "Author";
    cout << "| " << left << setw(6) << "Year";
    cout << "| " << left << setw(6) << "Pages";
    cout << "\n_______________________________\n" << endl;

    if (vBooks.size() == 0)
        cout << "\t\t\t\tNo Books Available In the System!";
    else
        for (sBook Book : vBooks)
        {
            PrintBookRecordLine(Book);
            cout << endl;
        }

    cout << "\n_______________________________\n" << endl;
}

void PrintBookCard(sBook Book)
{
    cout << "\nBook Details:";
    cout << "\n-----------------------------------";
    cout << "\nBook ID   : " << Book.BookID;
    cout << "\nTitle     : " << Book.Title;
    cout << "\nAuthor    : " << Book.Author;
    cout << "\nYear      : " << Book.Year;
    cout << "\nPages     : " << Book.Pages;
    cout << "\n-----------------------------------\n";
}

bool FindBookByID(string BookID, vector<sBook> vBooks, sBook& Book)
{
    for (sBook B : vBooks)
    {
        if (B.BookID == BookID)
        {
            Book = B;
            return true;
        }
    }
    return false;
}

sBook ChangeBookRecord(string BookID)
{
    sBook Book;
    Book.BookID = BookID;

    cout << "\nEnter Title? ";
    getline(cin >> ws, Book.Title);

    cout << "Enter Author? ";
    getline(cin, Book.Author);

    cout << "Enter Publication Year? ";
    cin >> Book.Year;

    cout << "Enter Number of Pages? ";
    cin >> Book.Pages;

    return Book;
}

bool MarkBookForDeleteByID(string BookID, vector<sBook>& vBooks)
{
    for (sBook& B : vBooks)
    {
        if (B.BookID == BookID)
        {
            B.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector<sBook> SaveBooksDataToFile(string FileName, vector<sBook> vBooks)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (sBook B : vBooks)
        {
            if (!B.MarkForDelete)
            {
                string DataLine = ConvertRecordToLine(B);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vBooks;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void AddNewBook()
{
    sBook Book = ReadNewBook();
    AddDataLineToFile(BooksFileName, ConvertRecordToLine(Book));
}

void AddNewBooks()
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Adding New Book:\n\n";
        AddNewBook();
        cout << "\nBook Added Successfully!\nAdd another book? (Y/N)? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

bool DeleteBookByID(string BookID, vector<sBook>& vBooks)
{
    sBook Book;
    if (FindBookByID(BookID, vBooks, Book))
    {
        PrintBookCard(Book);
        char Confirm;
        cout << "\nAre you sure you want to delete this book? (Y/N)? ";
        cin >> Confirm;
        if (toupper(Confirm) == 'Y')
        {
            MarkBookForDeleteByID(BookID, vBooks);
            SaveBooksDataToFile(BooksFileName, vBooks);
            cout << "\nBook Deleted Successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\nBook with ID [" << BookID << "] Not Found!\n";
    }
    return false;
}

bool UpdateBookByID(string BookID, vector<sBook>& vBooks)
{
    sBook Book;
    if (FindBookByID(BookID, vBooks, Book))
    {
        PrintBookCard(Book);
        char Confirm;
        cout << "\nAre you sure you want to update this book? (Y/N)? ";
        cin >> Confirm;
        if (toupper(Confirm) == 'Y')
        {
            for (sBook& B : vBooks)
            {
                if (B.BookID == BookID)
                {
                    B = ChangeBookRecord(BookID);
                    break;
                }
            }
            SaveBooksDataToFile(BooksFileName, vBooks);
            cout << "\nBook Updated Successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\nBook with ID [" << BookID << "] Not Found!\n";
    }
    return false;
}

string ReadBookID()
{
    string BookID;
    cout << "\nEnter Book ID: ";
    cin >> BookID;
    return BookID;
}

void ShowDeleteBookScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Book Screen";
    cout << "\n-----------------------------------\n";
    vector<sBook> vBooks = LoadBooksDataFromFile(BooksFileName);
    string BookID = ReadBookID();
    DeleteBookByID(BookID, vBooks);
}

void ShowUpdateBookScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Book Screen";
    cout << "\n-----------------------------------\n";
    vector<sBook> vBooks = LoadBooksDataFromFile(BooksFileName);
    string BookID = ReadBookID();
    UpdateBookByID(BookID, vBooks);
}

void ShowAddNewBooksScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Books Screen";
    cout << "\n-----------------------------------\n";
    AddNewBooks();
}

void ShowFindBookScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tFind Book Screen";
    cout << "\n-----------------------------------\n";
    vector<sBook> vBooks = LoadBooksDataFromFile(BooksFileName);
    sBook Book;
    string BookID = ReadBookID();
    if (FindBookByID(BookID, vBooks, Book))
        PrintBookCard(Book);
    else
        cout << "\nBook with ID [" << BookID << "] Not Found!\n";
}

void ShowEndScreen()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ended Successfully";
    cout << "\n-----------------------------------\n";
}

enum enMainMenueOptions {
    eListBooks = 1, eAddNewBook = 2,
    eDeleteBook = 3, eUpdateBook = 4,
    eFindBook = 5, eExit = 6
};

void GoBackToMainMenue()
{
    cout << "\nPress any key to return to Main Menu...";
    system("pause>0");
    ShowMainMenue();
}

short ReadMainMenueOption()
{
    short Choice;
    cout << "Choose an option [1-6]: ";
    cin >> Choice;
    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions Option)
{
    switch (Option)
    {
    case eListBooks:
        system("cls");
        ShowAllBooksScreen();
        GoBackToMainMenue();
        break;
    case eAddNewBook:
        ShowAddNewBooksScreen();
        GoBackToMainMenue();
        break;
    case eDeleteBook:
        ShowDeleteBookScreen();
        GoBackToMainMenue();
        break;
    case eUpdateBook:
        ShowUpdateBookScreen();
        GoBackToMainMenue();
        break;
    case eFindBook:
        ShowFindBookScreen();
        GoBackToMainMenue();
        break;
    case eExit:
        ShowEndScreen();
        exit(0);
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "============================================\n";
    cout << "\t   Library Management System\n";
    cout << "============================================\n";
    cout << "\t[1] List Books\n";
    cout << "\t[2] Add New Book\n";
    cout << "\t[3] Delete Book\n";
    cout << "\t[4] Update Book\n";
    cout << "\t[5] Find Book\n";
    cout << "\t[6] Exit\n";
    cout << "============================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

int main()
{
    ShowMainMenue();
    return 0;
}