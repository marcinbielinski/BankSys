#include <iostream>
#include <utility>
#include <vector>
#include <numeric>
#include <algorithm>

struct Account {
    int cash;

    Account(std::string fn, std::string ln, int money)
            : first_name(std::move(fn)), last_name(std::move(ln)), cash(money) {}


    static std::string getFirstName(const Account& account)
    {
        return account.first_name;
    }

    static std::string getLastName(const Account& account)
    {
        return account.last_name;
    }

    static std::string getID(const Account &account)
    {
        return account.id;
    }

    static std::string getEmail(const Account& account)
    {
        return account.email;
    }

    static int getCash(const Account& account)
    {
        return account.cash;
    }

private:
    std::string first_name;
    std::string last_name;
    std::string id{first_name + " " + last_name};
    std::string email{first_name + "." + last_name + "@gmail.com"};
};

class Bank {
public:
    std::vector<Account> the_bank;

    std::vector<int> reserve;

    void addAccount(std::string fn, std::string sur, int money) {
        the_bank.emplace_back(Account(std::move(fn), std::move(sur), money));
    }

    void delAccount(std::string accountID) {
        the_bank.erase(std::remove_if(
                begin(the_bank), end(the_bank),
                [&accountID](auto &acc) { return Account::getID(acc) == accountID; }), end(the_bank));
    }

    int accountBalance(std::string accountID) {
        auto result = std::find_if(begin(the_bank), end(the_bank),
                                   [&accountID](auto &acc) { return Account::getID(acc) == accountID; });

        (result != std::end(the_bank))
        ? std::cout << "The cash of this account: " <<
                    Account::getID(*result) << " is: " << Account::getCash(*result) << '\n'
        : std::cout << "This account doesn't exist." << std::endl;

        return Account::getCash(*result);
    }

    void transferCash(std::string accountID1, std::string accountID2, int amount)
    {
        auto sender = std::find_if(begin(the_bank), end(the_bank),
                                   [&accountID1](auto& acc) { return Account::getID(acc) == accountID1;});

        (sender != end(the_bank))
        ? std::cout << "Attempting the wire transfer... Please wait. \n"
        : std::cout << "The account you're trying to wire from doesn't exist." << std::endl;

        auto recipient = std::find_if(begin(the_bank), end(the_bank),
                                      [&accountID2](auto& acc) { return Account::getID(acc) == accountID2;});

        (recipient != end(the_bank))
        ? std::cout << "Successfully wired: " << amount << " from: " << Account::getID(*sender)
                                                        << " to: "   << Account::getID(*recipient)  << '\n'
        : std::cout << "This account doesn't exist." << std::endl;

        sender->cash -= amount;
        recipient->cash += amount;
    }

    int bankReserve() {
        int result;
        for (auto &elem : the_bank) {
            reserve.emplace_back(Account::getCash(elem));
        }
        result = std::accumulate(begin(reserve), end(reserve), 0);

        std::cout << "The Bank reserve is : " << result << std::endl;
        return result;
    }

    void printBank() {
        for (const auto &elem : the_bank) {
            std::cout << "Name: " << Account::getFirstName(elem) << " Surname: " << Account::getLastName(elem)
                      << " Money: " << Account::getCash(elem) << " E-mail: " << Account::getEmail(elem) << std::endl;
        }
    }

private:
    static void chargeMe(const Account& account, int amount)
    {
        Account::getCash(account) -= amount;
    }
    static void addMe(const Account& account, int amount)
    {
        Account::getCash(account) += amount;
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

    bnk.transferCash("Ojciec Mateusz", "JanPawel Drugi", 50);

    bnk.accountBalance("Ojciec Mateusz");
    bnk.accountBalance("JanPawel Drugi");

    return 0;
}
