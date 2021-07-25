#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

struct Account {
    Account(std::string fn, std::string ln, int money)
            : first_name(std::move(fn)), last_name(std::move(ln)), cash(money) {}


    const std::string& getFirstName()
    {
        return first_name;
    }

    const std::string& getLastName()
    {
        return last_name;
    }

    const std::string& getID()
    {
        return id;
    }

    const std::string& getEmail()
    {
        return email;
    }

    int getCash()
    {
        return cash;
    }

    void addCash(int money)
    {
        cash += money;
    }

    bool subtractCash(int money)
    {
        if (cash >= money)
        {
            cash -= money;
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    std::string first_name;
    std::string last_name;
    std::string id{first_name + " " + last_name};
    std::string email{first_name + "." + last_name + "@gmail.com"};
    int cash;
};

class Bank {
public:
    std::vector<Account> the_bank;

    void addAccount(std::string fn, std::string sur, int money) {
        the_bank.emplace_back(Account(std::move(fn), std::move(sur), money));
    }

    void delAccount(std::string accountID) {
        the_bank.erase(std::remove_if(
                begin(the_bank), end(the_bank),
                [&accountID](auto& acc) {
                    return acc.getID() == accountID; }), end(the_bank));
        std::cout << "Removing account of id: " << accountID << std::endl;

    }

    int accountBalance(std::string accountID) {
        auto result = std::find_if(begin(the_bank), end(the_bank),
                                   [&accountID](auto& acc) { return acc.getID() == accountID; });

        (result != std::end(the_bank))
        ? std::cout << "The cash of this account: " <<
                                                    result->getID() << " is: " << result->getCash() << '\n'
        : std::cout << "This account doesn't exist." << std::endl;

        return result->getCash();
    }

//    Account* findAccount(const std::string& accountID)
//    {
//        auto findAcc = std::find_if(begin(the_bank), end(the_bank),
//                                  [&accountID](auto& acc) { return acc.getID() == accountID;});
//        if (findAcc != end(the_bank))
//            return &*findAcc;
//        else
//        {
//            std::cout << "There is no such account in our base." << std::endl;
//            return nullptr;
//        }
//    }

    Account* findAccount(const std::string& accountID)
    {
        for (int elem = 0; elem <= the_bank.size(); elem++)
        {
            if (the_bank[elem].getID() == accountID)
            {
                return &the_bank[elem];
            }
        }
        return nullptr;
    }
    void transferCash(const std::string& accountID1, const std::string& accountID2, int amount)
    {
        auto sender = findAccount(accountID1);

        auto recipient = findAccount(accountID2);

        if (sender != nullptr && recipient != nullptr)
        {
            if (sender->subtractCash(amount))
            {
                recipient->addCash(amount);
                std::cout << "Wire of: " << amount << "$ from: " << sender->getID() << " to: " << recipient->getID()
                          << " was successful. Have a nice day!" << std::endl;
            }
            else
            {
                std::cout << "This account: " << sender->getID() << " doesn't have enough funds." << std::endl;
            }
        }
        else
        {
            std::cout << "Sorry, either sender or recipient is not in our base." << std::endl;
        }
    }

    int bankReserve() {
        int result {0};
        for (auto &elem : the_bank) {
            result += elem.getCash();
        }
        std::cout << "The Bank reserve is : " << result << std::endl;
        return result;
    }

    void printBank() {
        for (auto& elem : the_bank) {
            std::cout << "Name: " << elem.getFirstName() << " Surname: " << elem.getLastName()
                      << " Money: " << elem.getCash() << " E-mail: " << elem.getEmail() << std::endl;
        }
    }


};

class Interface
{
public:
    Bank& user_bank;

    void greetings() {}
};

int main() {
    Bank bnk;

    bnk.addAccount("Marcin", "Bielinski", 100);
    bnk.addAccount("Ojciec", "Mateusz", 150);
    bnk.addAccount("JanPawel", "Drugi", 300);

    bnk.printBank();

    bnk.accountBalance("Marcin Bielinski");

    bnk.delAccount("Marcin Bielinski");

    bnk.printBank();

    bnk.bankReserve();

    bnk.transferCash("Ojciec Mateusz", "JanPawel Drugi", 100);

    bnk.accountBalance("Ojciec Mateusz");
    bnk.accountBalance("JanPawel Drugi");

    return 0;
}
