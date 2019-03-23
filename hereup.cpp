#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract]] hereup : public contract {
public:
    using contract::contract;

    hereup(name receiver, eosio::name code, datastream<const char *> ds)
            : contract(receiver, code, ds),
              _companies(receiver, code.value) {}

    [[eosio::action]]
    void hi(name user) {
        print("Hello, ", name{user});
    }

    [[eosio::action]]
    void regcompany(name company_name) {
        require_auth(company_name);
        auto existing_company = _companies.find(company_name.value);
        eosio::check(existing_company == _companies.end(), "company already exists");
        _companies.emplace(get_self(), [&](auto& p) {
            p.account = company_name;
        });
    }

    [[eosio::action]]
    void regcoordinate(name company_name, double latitude, double longitude) {
        require_auth(company_name);
        auto existing_company = _companies.find(company_name.value);
        eosio::check(existing_company != _companies.end(), "company doesn't exist");

        coordinates coordinates_table(get_self(), company_name.value);
        for (auto& item : coordinates_table) {
            if ((item.latitude == latitude) && (item.longitude == longitude)) {
                eosio::check(false, "coordinate already exists");
            }
        }

        coordinates_table.emplace(get_self(), [&](auto& p) {
            p.id = coordinates_table.available_primary_key();
            p.latitude = latitude;
            p.longitude = longitude;
        });
    }

private:
    // scope: this contract
    struct [[eosio::table]] company {
        name account;

        uint64_t primary_key() const { return account.value; }
    };

    // scope: company account
    struct [[eosio::table]] coordinate {
        uint64_t id;
        long double latitude;
        long double longitude;

        uint64_t primary_key() const { return id; }
    };

    typedef eosio::multi_index<"company"_n, company> companies;
    typedef eosio::multi_index<"coordinate"_n, coordinate> coordinates;

    companies _companies;
};

EOSIO_DISPATCH(hereup, (hi)(regcompany)(regcoordinate))