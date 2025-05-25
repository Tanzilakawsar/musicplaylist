#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define MAX_SONGS 100

/* =================== Array‑based =================== */
string songNames[MAX_SONGS], 
artists[MAX_SONGS], 
genres[MAX_SONGS];
int arrCount = 0;

bool arrSongExists(const string& s, const string& a) {
    for (int i = 0; i < arrCount; ++i)
        if (songNames[i] == s && artists[i] == a) 
        return true;
    return false;
}

void arrSaveToFile() {
    std::ofstream out("array_playlist.txt");
    if (!out) {
        std::cerr << "Could not open array_playlist.txt for writing.\n";
        return;
    }

    for (int i = 0; i < arrCount; ++i) {
       out << songNames[i] << '\n'
            << artists[i]   << '\n'
            << genres[i]    << '\n';
    }
}


void arrLoadFromFile() {
    ifstream in("array_playlist.txt");
    string s, a, g;  arrCount = 0;
    while (getline(in, s) && getline(in, a) && getline(in, g)) {
        int pos = 0;
        while (pos < arrCount && songNames[pos] < s)
         ++pos;
        for (int i = arrCount; i > pos; --i) {
            songNames[i] = songNames[i - 1];
            artists[i]   = artists[i - 1];
            genres[i]    = genres[i - 1];
        }
        songNames[pos] = s; 
         artists[pos] = a; 
          genres[pos] = g;
        ++arrCount;
    }
}
int arrFindSongIndex(const string& s, const string& a) {
    for (int i = 0; i < arrCount; ++i)
        if (songNames[i] == s && artists[i] == a)
            return i;
    return -1;
}
bool arrInsertSong(const string& s, const string& a, const string& g) {
    if (arrCount >= MAX_SONGS) { 
        cout << "Array playlist full.\n"; 
        return false;
     }
    if (arrSongExists(s, a))   {
         cout << "Song already exists.\n";  
         return false;
        }

    int pos = 0; while (pos < arrCount && songNames[pos] < s) ++pos;
    for (int i = arrCount; i > pos; --i) {
        songNames[i] = songNames[i - 1];
        artists[i]   = artists[i - 1];
        genres[i]    = genres[i - 1];
    }
    songNames[pos] = s;  artists[pos] = a;  genres[pos] = g;
    ++arrCount;
    cout << "Song inserted.\n";
    return true;
}

bool arrDeleteSong(const string& s, const string& a) {
    for (int i = 0; i < arrCount; ++i)
        if (songNames[i] == s && artists[i] == a) {
            for (int j = i; j < arrCount - 1; ++j) {
                songNames[j] = songNames[j + 1];
                artists[j]   = artists[j + 1];
                genres[j]    = genres[j + 1];
            }
            --arrCount;  cout << "Song deleted.\n";  return true;
        }
    cout << "Song not found.\n";  return false;
}

void arrDisplaySongs() {
    if (!arrCount) {
         cout << "No songs in array playlist.\n"; 
         return; 
        }
    cout << "\n--- Array Playlist ---\n";
    for (int i = 0; i < arrCount; ++i)
           cout << i + 1 << ". " << songNames[i] << " by " << artists[i]
             << " [" << genres[i] << "]\n";
}

void arrClearAll() {
    arrCount = 0;
    ofstream("array_playlist.txt", ios::trunc);
    cout << "All songs removed from array playlist.\n";
}

/* =================== Linked‑list =================== */
struct Node {
    string songName, artist, genre;
    Node* next;
};

Node* head = nullptr;

bool llSongExists(const string& s, const string& a) {
    for (Node* t = head; t; t = t->next)
        if (t->songName == s && t->artist == a) 
        return true;
    return false;
}

void llInsertSong(const string& s, const string& a, const string& g) {
    if (llSongExists(s, a)) {
         cout << "Song already exists.\n";
         return; 
        }
    Node* n = new Node{s, a, g, nullptr};
    if (!head || head->songName > s) {
         n->next = head;
          head = n; }
    else {
        Node* cur = head;
        while (cur->next && cur->next->songName < s)
         cur = cur->next;
        n->next = cur->next;  
        cur->next = n;
    }
    cout << "Song inserted.\n";
}

bool llDeleteSong(const string& s, const string& a) {
    Node *cur = head, *prev = nullptr;
    while (cur) {
        if (cur->songName == s && cur->artist == a) {
            (prev ? prev->next : head) = cur->next;
            delete cur; 
            cout << "Song deleted.\n"; return true;
        }
        prev = cur; 
        cur = cur->next;
    }
    cout << "Song not found.\n"; return false;
}

bool llFindSong(const string& s, const string& a) {
    int i = 1;
    for (Node* t = head; t; t = t->next, ++i)
        if (t->songName == s && t->artist == a) {
            cout << "Song Found at position: " << i << endl;
            return true;
        }
    cout << "Song Not found.\n";
    return false;
}


void llDisplaySongs() {
    if (!head) { cout << "No songs in linkedlist playlist.\n"; return; }
    cout << "\n--- Linked List Playlist ---\n";
    int i = 1;
    for (Node* t = head; t; t = t->next)
        cout << i++ << ". " << t->songName << " by " << t->artist
             << " [" << t->genre << "]\n";
}

void llSaveToFile() {
    std::ofstream out("linkedlist_playlist.txt");
    if (!out) {
        std::cerr << "Could not open file for writing.\n";
        return;
    }

    int i = 1;
    for (Node* t = head; t; t = t->next) {
        out << t->songName << '\n'     
            << t->artist   << '\n'
            << t->genre    << '\n';
    }
   
}


void llLoadFromFile() {
    while (head) { Node* t = head; head = head->next; delete t; }
    ifstream in("linkedlist_playlist.txt");
    string s, a, g;
    while (getline(in, s) && getline(in, a) && getline(in, g))
        llInsertSong(s, a, g);
}

void llClearAll() {
    while (head) { Node* t = head; head = head->next; delete t; }
    ofstream("linkedlist_playlist.txt", ios::trunc);
    cout << "All songs removed from linkedlist playlist.\n";
}


/* =================== Menus =================== */
void arrayMenu() {
    int ch; string s, a, g;
    while (true) {
        cout << "\n--- Array Playlist Menu ---\n"
             << "1. Insert Song\n2. Delete Song\n3. Search Song\n"
             << "4. Display Playlist\n5. Delete ALL Songs\n"
             << "6. Save Playlist\n7. Back to Main Menu\nEnter choice: ";
        cin >> ch; cin.ignore();
        switch (ch) {
            case 1:
                cout << "Song: "; 
                getline(cin, s);
                cout << "Artist: "; 
                getline(cin, a);
                cout << "Genre: ";
                 getline(cin, g);
                if (arrInsertSong(s, a, g)) {
                    char c; 
                    cout << "Save now? (y/n): "; 
                    cin >> c; 
                    cin.ignore();
                    if (c=='y'||c=='Y') 
                    arrSaveToFile();
                } break;
            case 2:
                cout << "Song: "; 
                getline(cin, s);
                cout << "Artist: "; 
                getline(cin, a);
                if (arrDeleteSong(s, a)) 
                arrSaveToFile();
                break;
            case 3:
    cout << "Song: "; getline(cin, s);
                cout << "Artist: "; getline(cin, a);
                {
                    int index = arrFindSongIndex(s, a);
                    if (index != -1)
                        cout << "Song Found at index: " << index + 1 << endl;
                    else
                        cout << "Song Not found.\n";
                }
                break;

            case 4: arrDisplaySongs();
             break;
            case 5: arrClearAll(); 
            break;
            case 6: arrSaveToFile(); 
            cout<<"Playlist saved.\n"; 
            break;
            case 7: return;
            default: cout << "Invalid choice.\n";
        }
    }
}

void linkedListMenu() {
    int ch; string s, a, g;
    while (true) {
        cout << "\n--- Linked List Playlist Menu ---\n"
             << "1. Insert Song\n2. Delete Song\n3. Search Song\n"
             << "4. Display Playlist\n5. Delete ALL Songs\n"
             << "6. Save Playlist\n7. Back to Main Menu\nEnter choice: ";
        cin >> ch; cin.ignore();
        switch (ch) {
            case 1:
                cout << "Song: "; 
                getline(cin, s);
                cout << "Artist: "; 
                getline(cin, a);
                cout << "Genre: "; 
                getline(cin, g);
                llInsertSong(s, a, g);
                { char c; 
                    cout << "Save now? (y/n): ";
                     cin >> c; 
                     cin.ignore();
                  if (c=='y'||c=='Y')
                   llSaveToFile(); }
                break;
            case 2:
                cout << "Song: "; 
                getline(cin, s);
                cout << "Artist: ";
                 getline(cin, a);
                if (llDeleteSong(s, a))
                 llSaveToFile();
                break;
           case 3:
    cout << "Song: "; 
    getline(cin, s);
    cout << "Artist: "; 
    getline(cin, a);
    llFindSong(s, a);
    break;

    
             
            case 4: llDisplaySongs(); 
            break;
            case 5: llClearAll(); 
            break;
            case 6: llSaveToFile(); 
            cout<<"Playlist saved.\n";
             break;
            case 7: return;
            default: cout << "Invalid choice.\n";
        }
    }
}

int main() {
    arrLoadFromFile();
    llLoadFromFile();
    int ch;
    while (true) {
        cout << "\n=== MUSIC PLAYER ===\n1. Array Playlist\n2. Linked List Playlist\n3. Exit\nChoice: ";
        cin >> ch;
         cin.ignore();
        if (ch == 1)
         arrayMenu();
        else if (ch == 2)
         linkedListMenu();
        else if (ch == 3) { 
            cout << "Thank You!...\n"; 
            break; 
        }
        else cout << "Invalid choice.\n";
    }
    return 0;
}