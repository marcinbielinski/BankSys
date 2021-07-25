#include <iostream>
#include <utility>
#include <vector>
#include <numeric>
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

    int& getCash()
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
            std::cout << "This account: " << id << " doesn't have enough funds." << std::endl;
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
                [&accountID](auto& acc) { return acc.getID() == accountID; }), end(the_bank));
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

    void transferCash(std::string accountID1, std::string accountID2, int amount)
    {
        auto sender = std::find_if(begin(the_bank), end(the_bank),
                                   [&accountID1](auto& acc) { return acc.getID() == accountID1;});

        (sender != end(the_bank))
        ? std::cout << "Attempting the wire transfer... Please wait. \n"
        : std::cout << "The account you're trying to wire from doesn't exist." << std::endl;

        auto recipient = std::find_if(begin(the_bank), end(the_bank),
                                      [&accountID2](auto& acc) { return acc.getID() == accountID2;});

        (recipient != end(the_bank))
        ? std::cout << "Successfully wired: " << amount << " from: " << sender->getID()
                                                        << " to: "   << recipient->getID()  << '\n'
        : std::cout << "This account doesn't exist." << std::endl;


        if (sender->subtractCash(amount))
        {
            recipient->addCash(amount);
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
