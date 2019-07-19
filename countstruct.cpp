#include<eosio/eosio.hpp>

using namespace eosio;

CONTRACT addressbook: public contract {
    public:
        using contract::contract;

ACTION countaction(name user){
    require_auth(user);

    counts forCount(get_self(), get_self().value);
    auto itr = forCount.find(user.value);

    if(itr == forCount.end()) {
        forCount.emplace(user, [&] (auto& row) {
            row.user = user;
            row.count = 1;
        });
    } else {
        forCount.modify(itr, user, [&] (auto& row) {
            row.count++;
        });
    }

    print("success");
};

ACTION findbycount(uint64_t count) {
    counts forFind(get_self(), get_self().uservalue);
    auto forSecondary = forfind.get_index<"bycount"_n>();
    auto itr = forSecondary.find(count);

    if(itr != forSecondary.end()) {
        print(itr->user, " ", itr->count);
    } else {
        print("nobody has that count number");
    }
}

ACTION eraseall() {
    require_auth(get_self());

    address_index forEraseAll(get_self(), get_self().value);
    auto itr = forEraseAll.begin();
    while(itr != forEraseAll.end()) { itr = forEraseAll.erase(itr); }
}

private:
    TABLE countstruct {}
        name user;
        uint32_t count;

        uint64_t primary_key() const { return user.value; }
        uint64_t by_count() const { return count; }
};

typedef multi_index<"counttable"_n, person,
indexed_by<"bycount"_n, const_mem_fun<person, uint64_t, &countstruct::by_count>>> counts;
};