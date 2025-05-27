#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <set>
using namespace std;

void loadUsersFromFile();
void loadTransactions();
void saveUsersToFile();
void saveTransactions();
void restoreFromBackup();
void backupUsersBeforeChange(); 

struct User {
    string walletId;      
    string username;
    string passwordHash;
    string role;
    string fullName;
    string email;
    string phone;
    int wallet = 5000;
    bool passwordGenerated = false;
};

struct Transaction {
    string sender;
    string receiver;
    int amount;
    string timestamp;
    string status;
};

vector<User> users;
vector<Transaction> transactions;
unordered_map<string, pair<string, time_t>> otpStore;
set<string> usedWalletIds; 

string bcryptHash(const string& input) {
    return "$bcrypt$" + input;
}


string generateUniqueWalletId() {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string walletId;
    int counter = 0;
    
    do {
        walletId = "WL";
        
        time_t now = time(0);
        string timeStr = to_string(now % 100000); 
        
        for (int i = 0; i < 4; ++i) {
            walletId += chars[rand() % chars.length()];
        }
        

        walletId += to_string(counter);
        counter++;
        
    } while (usedWalletIds.find(walletId) != usedWalletIds.end() && counter < 1000);
    
    usedWalletIds.insert(walletId); 
    return walletId;
}

string generateRandomPassword(int length = 8) {
    const string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*";
    string password;
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        password += chars[rand() % chars.length()];
    }
    return password;
}

string generateOTP(const string& username) {
    srand(time(0));
    string otp = "";
    for (int i = 0; i < 6; i++) otp += to_string(rand() % 10);
    otpStore[username] = {otp, time(0)};
    return otp;
}

bool verifyOTP(const string& username) {
    string input;
    cout << "Nhap OTP: ";
    cin >> input;
    auto it = otpStore.find(username);
    if (it != otpStore.end()) {
        time_t now = time(0);
        if (now - it->second.second <= 120 && input == it->second.first) {
            otpStore.erase(it);
            return true;
        }
    }
    return false;
}


void backupUsersBeforeChange() {
 
    ifstream src("users.txt", ios::binary);
    ofstream dst("users_backup.txt", ios::binary);
    
    if (src && dst) {
        dst << src.rdbuf();
        cout << "Da tao backup tu dong truoc khi thay doi du lieu.\n";
    }
    
    src.close();
    dst.close();
}

void restoreFromBackup() {
  
    ifstream checkFile("users_backup.txt");
    if (!checkFile.good()) {
        cout << "File backup khong ton tai.\n";
        checkFile.close();
        return;
    }
    

    checkFile.seekg(0, ios::end);
    if (checkFile.tellg() == 0) {
        cout << "File backup trong hoac khong co du lieu de phuc hoi.\n";
        checkFile.close();
        return;
    }
    checkFile.close();
    
   
    char confirm;
    cout << "Ban co chac chan muon phuc hoi du lieu? Thao tac nay se ghi de du lieu hien tai (y/n): ";
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Huy bo phuc hoi.\n";
        return;
    }
    
 
    ifstream src("users_backup.txt");
    if (!src.is_open()) {
        cout << "Khong the mo file backup de doc.\n";
        return;
    }
    

    ofstream dst("users.txt");
    if (!dst.is_open()) {
        cout << "Khong the mo file users.txt de ghi.\n";
        src.close();
        return;
    }
    
  
    string line;
    while (getline(src, line)) {
        dst << line << "\n";
    }
    
    src.close();
    dst.close();
    
    cout << "Phuc hoi du lieu thanh cong tu users_backup.txt.\n";
    

    loadUsersFromFile();
    cout << "Du lieu da duoc cap nhat trong he thong.\n";
}

void saveUsersToFile() {
   
    ofstream out("users.txt");
    for (auto& u : users) {
        out << u.walletId << ',' << u.username << ',' << u.passwordHash << ',' << u.role << ','
            << u.fullName << ',' << u.email << ',' << u.phone << ',' << u.wallet << ',' << u.passwordGenerated << '\n';
    }
    out.close();
}

void loadUsersFromFile() {
    ifstream in("users.txt");
    User u;
    users.clear();
    usedWalletIds.clear();
    
    while (getline(in, u.walletId, ',')) {
        usedWalletIds.insert(u.walletId); 
        getline(in, u.username, ',');
        getline(in, u.passwordHash, ',');
        getline(in, u.role, ',');
        getline(in, u.fullName, ',');
        getline(in, u.email, ',');
        getline(in, u.phone, ',');
        in >> u.wallet;
        in.ignore();
        in >> u.passwordGenerated;
        in.ignore();
        users.push_back(u);
    }
}

void saveTransactions() {
    ofstream out("transactions.txt");
    for (auto& t : transactions) {
        out << t.sender << ',' << t.receiver << ',' << t.amount << ',' << t.timestamp << ',' << t.status << '\n';
    }
}

void loadTransactions() {
    ifstream in("transactions.txt");
    Transaction t;
    transactions.clear();
    while (getline(in, t.sender, ',')) {
        getline(in, t.receiver, ',');
        in >> t.amount;
        in.ignore();
        getline(in, t.timestamp, ',');
        getline(in, t.status);
        transactions.push_back(t);
    }
}

User* findUser(const string& username) {
    for (auto& u : users) if (u.username == username) return &u;
    return nullptr;
}


User* findUserByWalletId(const string& walletId) {
    for (auto& u : users) if (u.walletId == walletId) return &u;
    return nullptr;
}

void updateUserInfo(User& user) {
    cout << "Wallet ID: " << user.walletId << "\n";
    cout << "(Luu y: Username khong the thay doi)\n";
    string newName, newEmail, newPhone;
    cout << "Nhap ten moi: "; cin.ignore(); getline(cin, newName);
    cout << "Nhap email moi: "; getline(cin, newEmail);
    cout << "Nhap sdt moi: "; getline(cin, newPhone);

    cout << "--- Thong tin thay doi ---\n";
    cout << "Ho ten: " << user.fullName << " -> " << newName << "\n";
    cout << "Email : " << user.email << " -> " << newEmail << "\n";
    cout << "SDT   : " << user.phone << " -> " << newPhone << "\n";

    string otp = generateOTP(user.username);
    cout << "OTP da gui den nguoi dung: " << otp << endl;
    if (verifyOTP(user.username)) {
     
        backupUsersBeforeChange();
        
        user.fullName = newName;
        user.email = newEmail;
        user.phone = newPhone;
        
        saveUsersToFile();
        cout << "Cap nhat thanh cong!\n";
    } else {
        cout << "OTP sai hoac het han. Huy bo.\n";
    }
}

void changePassword(User& user) {
    string current, newPass;
    cout << "Nhap mat khau hien tai: "; cin >> current;
    if (bcryptHash(current) != user.passwordHash) {
        cout << "Sai mat khau.\n";
        return;
    }
    string otp = generateOTP(user.username);
    cout << "OTP: " << otp << endl;
    if (!verifyOTP(user.username)) {
        cout << "OTP sai. Huy thao tac.\n";
        return;
    }
    
   
    backupUsersBeforeChange();
    
    cout << "Nhap mat khau moi: "; cin >> newPass;
    user.passwordHash = bcryptHash(newPass);
    if (bcryptHash(newPass) == user.passwordHash) {
    cout << "Mat khau moi khong duoc trung voi mat khau cu.\n";
    return;
}
    user.passwordGenerated = false;
    saveUsersToFile();
    cout << "Doi mat khau thanh cong.\n";
}

void transferPoints(User& sender) {
    string receiverUser;
    int amount;
    cout << "Nhap ten nguoi nhan: "; cin >> receiverUser;
    User* receiver = findUser(receiverUser);
    if (!receiver) {
        cout << "Nguoi nhan khong ton tai.\n";
        return;
    }
    cout << "Nhap so diem can chuyen: "; cin >> amount;
    if (sender.wallet < amount) {
        cout << "Khong du so du.\n";
        return;
    }
    string otp = generateOTP(sender.username);
    cout << "OTP: " << otp << endl;
    if (!verifyOTP(sender.username)) {
        cout << "Xac nhan OTP that bai. Huy giao dich.\n";
        return;
    }
    
  
    backupUsersBeforeChange();
    
    sender.wallet -= amount;
    receiver->wallet += amount;
    time_t now = time(0);
    transactions.push_back({sender.username, receiver->username, amount, ctime(&now), "Success"});
    saveUsersToFile();
    cout << "Chuyen diem thanh cong!\n";
}

void viewTransactions(const User& user) {
    cout << "Wallet ID: " << user.walletId << "\n";
    for (auto& t : transactions) {
        if (t.sender == user.username || t.receiver == user.username) {
            cout << t.sender << " -> " << t.receiver << " | " << t.amount << " diem | " << t.timestamp << "\n";
        }
    }
}

void deleteAccount() {
    string userToDelete;
    cout << "Nhap ten tai khoan can xoa: "; cin >> userToDelete;
    User* target = findUser(userToDelete);
    if (!target) {
        cout << "Khong tim thay tai khoan.\n";
        return;
    }
    cout << "Tai khoan can xoa - Wallet ID: " << target->walletId << ", Username: " << target->username << "\n";
    string otp = generateOTP(userToDelete);
    cout << "OTP da gui: " << otp << endl;
    if (verifyOTP(userToDelete)) {
        
        backupUsersBeforeChange();
        
        
        usedWalletIds.erase(target->walletId);
        
        users.erase(remove_if(users.begin(), users.end(), [&](User u){ return u.username == userToDelete; }), users.end());
        saveUsersToFile();
        cout << "Da xoa tai khoan.\n";
    } else {
        cout << "OTP sai. Khong xoa tai khoan.\n";
    }
}

void userMenu(User& user) {
    while (user.passwordGenerated) {
        cout << "Hay doi mat khau tu dong sinh truoc khi tiep tuc.\n";
        changePassword(user);

        if (user.passwordGenerated) {
            cout << "Ban chua doi mat khau thanh cong. Khong the tiep tuc.\n";
            return; 
        }
    }
    int choice;
    do {
        cout << "\nWallet ID: " << user.walletId << "\n";
        cout << "1. Xem vi\n2. Doi mat khau\n3. Chinh sua thong tin\n4. Chuyen diem\n5. Xem lich su giao dich\n6. Thoat\nLua chon: ";
        cin >> choice;
        if (choice == 1) cout << "So du: " << user.wallet << " diem\n";
        else if (choice == 2) changePassword(user);
        else if (choice == 3) updateUserInfo(user);
        else if (choice == 4) transferPoints(user);
        else if (choice == 5) viewTransactions(user);
    } while (choice != 6);
}

void adminMenu(User& admin) {
    int choice;
    do {
        cout << "\nAdmin Wallet ID: " << admin.walletId << "\n";
        cout << "1. Xem danh sach tai khoan\n2. Tao tai khoan moi\n3. Sua thong tin tai khoan khac\n4. Xoa tai khoan\n5. phuc hoi du lieu\n6. Thoat\nLua chon: ";
        cin >> choice;
        if (choice == 1)
            for (auto& u : users)
                cout << "Wallet ID: " << u.walletId << " - " << u.username << " - " << u.role << " - " << u.wallet << " diem\n";
        else if (choice == 2) {
          
            
            User u;
            u.walletId = generateUniqueWalletId(); 
            cout << "Wallet ID moi: " << u.walletId << "\n";
            cout << "Nhap ten tai khoan: "; cin >> u.username;
            if (findUser(u.username)) {
                cout << "Tai khoan da ton tai!\n";
                
                usedWalletIds.erase(u.walletId);
                continue;
            }
            string pwdChoice;
            cout << "Tao mat khau tu dong? (y/n): "; cin >> pwdChoice;
            if (pwdChoice == "y") {
                string tempPass = generateRandomPassword();
                cout << "Mat khau tam thoi cua nguoi dung: " << tempPass << endl;
                u.passwordHash = bcryptHash(tempPass);
                u.passwordGenerated = true;
            } else {
                string pass;
                cout << "Nhap mat khau: "; cin >> pass;
                u.passwordHash = bcryptHash(pass);
            }
            cout << "Nhap ho ten: "; cin.ignore(); getline(cin, u.fullName);
            cout << "Email: "; getline(cin, u.email);
            cout << "SDT: "; getline(cin, u.phone);
            cout << "Vai tro (ADMIN/USER): "; cin >> u.role;
            u.wallet = 5000;
            backupUsersBeforeChange();
            
            users.push_back(u);
            time_t now = time(0);
            transactions.push_back({"SYSTEM", u.username, u.wallet, ctime(&now), "Initial Funding"});
			saveUsersToFile();  
            cout << "Da tao tai khoan moi.\n";
        }
        else if (choice == 3) {
            string uname; cout << "Nhap ten tai khoan: "; cin >> uname;
            User* u = findUser(uname);
            if (u) updateUserInfo(*u);
            else cout << "Khong tim thay tai khoan.\n";
        }
        else if (choice == 4) deleteAccount();
        else if (choice == 5) {
            restoreFromBackup();
            cout << "Vui long dang nhap lai de cap nhat du lieu phuc hoi.\n";
            return;
        } else if (choice == 6) {
            break;
        }
    } while (choice != 6); 
}

void loginSystem() {
    string u, p;
    cout << "Ten dang nhap: "; cin >> u;
    cout << "Mat khau: "; cin >> p;
    for (auto& user : users) {
        if (user.username == u && user.passwordHash == bcryptHash(p)) {
            cout << "Dang nhap thanh cong!\n";
            cout << "Wallet ID: " << user.walletId << "\n";
            if (user.role == "ADMIN") adminMenu(user);
            else userMenu(user);
            saveUsersToFile(); saveTransactions();
            return;
        }
    }
    cout << "Dang nhap that bai.\n";
}

void dangKyTaiKhoan() {
    
    
    User u;
    u.walletId = generateUniqueWalletId(); 
    cout << "=== DANG KY TAI KHOAN MOI ===\n";
    cout << "Wallet ID cua ban: " << u.walletId << "\n";
    cout << "Nhap ten tai khoan: "; cin >> u.username;
    if (findUser(u.username)) {
        cout << "Tai khoan da ton tai!\n";
       
        usedWalletIds.erase(u.walletId);
        return;
    }

    string pass;
    cout << "Nhap mat khau: "; cin >> pass;
    u.passwordHash = bcryptHash(pass);
    u.passwordGenerated = false;

    cin.ignore();
    cout << "Nhap ho ten: "; getline(cin, u.fullName);
    cout << "Email: "; getline(cin, u.email);
    cout << "SDT: "; getline(cin, u.phone);

    do {
        cout << "Nhap vai tro (ADMIN/USER): ";
        getline(cin, u.role);
        if (u.role != "ADMIN" && u.role != "USER") {
            cout << "Vai tro khong hop le. Vui long nhap lai.\n";
        }
    } while (u.role != "ADMIN" && u.role != "USER");
    
    backupUsersBeforeChange();

    u.wallet = 5000;
    users.push_back(u);
    time_t now = time(0);
    transactions.push_back({"SYSTEM", u.username, u.wallet, ctime(&now), "Initial Funding"});
    saveUsersToFile();
    cout << "Dang ky thanh cong! Ban co the dang nhap ngay bay gio.\n";
}

int main() {
    srand(time(0));

    loadUsersFromFile();
    loadTransactions();
    
    bool systemCreated = false;
    if (!findUser("SYSTEM")) {
        User system;
        system.walletId = generateUniqueWalletId();
        system.username = "SYSTEM";
        system.passwordHash = bcryptHash("admin");
        system.role = "SYSTEM";
        system.fullName = "He thong";
        system.email = "system@admin.com";
        system.phone = "0000000000";
        system.wallet = 100000000;
        system.passwordGenerated = false;
        users.push_back(system);
        systemCreated = true;
    }

    if (systemCreated) {
        saveUsersToFile();
    }

    int choice;
    do {
        cout << "\n=== HE THONG QUAN LY VI DIEM ===\n";
        cout << "1. Dang nhap\n2. Dang ky\n3. Thoat\n";
        cout << "Lua chon: ";
        cin >> choice;
        if (choice == 1) loginSystem();
        else if (choice == 2) dangKyTaiKhoan();
        else if (choice != 3) cout << "Lua chon khong hop le. Vui long thu lai.\n";
    } while (choice != 3);

    return 0;
}
