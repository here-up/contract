#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] hereup : public contract {
public:
    using contract::contract;

    [[eosio::action]]
    void hi( name user ) {
        print( "Hello, ", name{user});
    }
};
