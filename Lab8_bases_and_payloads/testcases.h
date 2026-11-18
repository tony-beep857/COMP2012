#include "Base.h"
#include "BaseAnalyzer.h"
#include "Payload.h"
#include "World.h"
#include <iostream>

using std::cout;
using ItemMap = Base::ItemMap;
using ItemPair = Base::ItemPair;

const int NUM_TESTCASES = 5;

#define ASSERT_TRUE(condition, error_msg) if (!(condition)) cout << "Assertion fail at testcases.h line " << __LINE__ << ": " << error_msg << "\n";
#define ASSERT(expected, obtained, error_msg) if (expected != obtained) {ASSERT_TRUE(false, error_msg); cout << "Expected: " << expected << "; but got " << obtained << "\n"; }


template <int N>
void test() {
    cout << "Testcase " << N << " does not exist.\n";
}

struct TestStruct {
    int x;
};

/**
 * @brief MapView
 */
template<>
void test<1>() {
    TestStruct a{1}, b{3}, c{5}, d{7}, e{9};
    map<string, TestStruct *> m{
        {"One", &a}, {"Three", &b}, {"Five", &c}, {"Seven", &d}, {"Nine", &e}
    };
    MapView<TestStruct> view{m};
    
    ASSERT_TRUE(m.at("One") == &*view.find("One"), "Address of reference obtained by dereferencing MapView's iterator is not equal to the pointer in the original map.");
    ASSERT(5, view.find("Five")->x, "Incorrect value obtained when using arrow operator on MapView iterator.");
}

/**
 * @brief BaseAnalyzer
 */
template<>
void test<2>() {
    Base empty_base{"Empty Base", initializer_list<ItemPair>{}};
    // test integrity when the base is empty
    ASSERT(0, BaseAnalyzer::range_sum(empty_base, "A", "z"), "Bad output when range_sum processes an empty base");
    ItemPair p = BaseAnalyzer::max_item(empty_base);
    ASSERT(p.first, "EMPTY", "Bad output when max_item processes an empty base");

    Base typical_base{"Farm", vector<ItemPair>{
        {"Common Apple", 3},{"Uncommon Apple", 4},{"Rare Apple", 1},
        {"Common Mango", 10}, {"Uncommon Mango", 2},
        {"Common Orange", 5}, {"Rare Orange", 7},
        {"Uncommon Wheat", 4}, {"Rare Wheat", 3},
    }};
    ASSERT(13, BaseAnalyzer::range_sum(typical_base, "Common Apple", "Common Mango"), "Wrong sum from range_sum that supposedly sums Common Apple and Common Mango");
    ASSERT(18, BaseAnalyzer::range_sum(typical_base, "Common", "D"), "Wrong sum from range_sum that supposedly sums all commons");
    ItemPair maxPair = BaseAnalyzer::max_item(typical_base);
    ASSERT("Common Mango", maxPair.first, "Wrong item returned as the max pair");
    ASSERT(10, maxPair.second, "Wrong quantity returned as the max pair");

    Base repeated_max{"Ranch", vector<ItemPair>{ {"Milk", 9}, {"Egg", 9}, {"Leather", 9}}};
    ItemPair maxPair2 = BaseAnalyzer::max_item(repeated_max);
    ASSERT("Egg", maxPair2.first, "Lexiologically smallest max item not returned when there is a tie in max");
}

/**
 * @brief World constructor, destructor, MapView, create_payload, remove_payload
 */
template<>
void test<3>() {
    Base *b1 = new Base{"Mine", initializer_list<ItemPair>{ {"Stone", 40}, {"Pickaxe", 3}, {"Rare Earth", 10}, {"Ore", 50}, {"Gemstone", 5}}};
    Base *b2 = new Base{"Lumbermill", initializer_list<ItemPair>{ {"Raw Wood", 65}, {"Processed Wood", 150}, {"Sawdust", 1000}}};
    Base *b3 = new Base{"Production Complex", initializer_list<ItemPair>{}};

    World s{initializer_list<Base *>{b1, b2, b3}};

    // check create_payload
    // success
    ASSERT(true, s.create_payload("Mining Transportation Team", "Mine", "Production Complex", initializer_list<ItemPair>{{"Stone", 10}, {"Ore", 40}, {"Gemstone", 5}}, 40), "Wrong return value of create_payload when creating a valid payload");
    ASSERT(40, s.get_payloads().at("Mining Transportation Team").get_arrival_time(), "Wrong arrival time of a newly created payload");
    ASSERT(30, s.get_bases().at("Mine")["Stone"], "Wrong number of Stone in Base Mine's after successful creation of a payload that has Stone");
    // fail: duplicate ID
    ASSERT(false, s.create_payload("Mining Transportation Team", "Mine", "Lumbermill", initializer_list<ItemPair>{{"Stone", 10}}, 30), "Wrong return value of create_payload when creating an invalid (ID already exists) payload");
    ASSERT(40, s.get_payloads().at("Mining Transportation Team").get_travel_time(), "Wrong payload's travel time, which *may* have been changed after failing to create a payload of the same ID");
    ASSERT(30, s.get_bases().at("Mine")["Stone"], "Wrong number of Stone in Base Mine after failed payload creation that has Mine as from base");
    // fail: invalid from base
    ASSERT(false, s.create_payload("Farming Transportation Team", "Farmland", "Production Complex", initializer_list<ItemPair>{{"Wheat", 1}}, 50), "Wrong return value of invalid create_payload (sender_id is not one of the world's base's ID)");
    // fail: invalid to base
    ASSERT(false, s.create_payload("Lumbermill Transportation Team", "Lumbermill", "Farmland", initializer_list<ItemPair>{{"Raw Wood", 1}}, 50), "Wrong return value of invalid create_payload (receiver_id is not one of the world's base's ID)");
    ASSERT(65, s.get_bases().at("Lumbermill")["Raw Wood"], "Wrong number of Raw Wood in Base Lumbermill after a failed payload creation that starts from Lumbermill");
    // fail: insufficient items in from base
    ASSERT(false, s.create_payload("Lumbermill Transportation Team", "Lumbermill", "Production Complex", initializer_list<ItemPair>{{"Raw Wood", 500}, {"Sawdust", 500}}, 50), "Wrong return value of invalid create_payload (Base Lumbermill does not have enough raw wood)");
    ASSERT(1000, s.get_bases().at("Lumbermill")["Sawdust"], "Wrong number of Sawdust in Base Lumbermill after a failed payload creation that starts from Lumbermill");

    // successes
    s.create_payload("Lumbermill Transportation Team", "Lumbermill", "Production Complex", initializer_list<ItemPair>{{"Raw Wood", 50}, {"Sawdust", 500}}, 40);
    s.create_payload("Another Mining Transportation Team", "Mine", "Lumbermill", initializer_list<ItemPair>{{"Stone", 10}}, 20);

    unordered_set<string> my_payloads{"Mining Transportation Team", "Lumbermill Transportation Team", "Another Mining Transportation Team"};
    for (const Payload &payload: s.get_payloads())
        ASSERT_TRUE(my_payloads.find(payload.get_id()) != my_payloads.end(), "A payload, " << payload.get_id() << ", is in World s despite previous actions not creating it");

    // fail removal
    ASSERT(false, s.remove_payload("Nonexistent"), "Wrong return value when calling remove_payload on a nonexistent payload");
    // reference to a payload about to be removed
    const Payload &p = *s.get_payloads().find("Lumbermill Transportation Team");
    ASSERT(true, s.remove_payload("Lumbermill Transportation Team"), "Wrong return value when calling remove_payload an existing payload");
    ASSERT(true, p.get_deleted(), "Payload not marked as deleted after removal.");
    ASSERT_TRUE(s.get_payloads().find("Lumbermill Transportation Team") == s.get_payloads().end(), "Removed payload Lumbermill Transportation Team not removed from the m_payloads");
}

/**
 * @brief process_arrived_payloads
 */
template<>
void test<4>() {
    using Items = initializer_list<ItemPair>;
    World s{initializer_list<Base *> {
        new Base{"Airport", Items {
            {"Electronics", 150},
            {"Souvenirs", 200},
            {"Fresh Goods", 100},
        }},
        new Base{"Hong Kong Island Warehouse", Items {
            {"Electronics", 4000},
            {"Souvenirs", 1000},
            {"Stationaries", 2000},
        }},
        new Base{"Kowloon Warehouse", Items {
            {"Electronics", 3000},
            {"Fresh Goods", 1500},
            {"Clothes", 2500},
        }},
        new Base{"New Territories Warehouse", Items {
            {"Souvenirs", 2000},
            {"Fresh Goods", 5000},
            {"Books", 3500},
        }},
    }};

    // test whether elapsing with no payload crashes
    s.elapse(12);

    // test basic payload ordering
    s.create_payload("Van1", "Hong Kong Island Warehouse", "Airport", Items {
        {"Electronics", 50},
        {"Souvenirs", 30},
    }, 10);
    s.create_payload("Van2", "Kowloon Warehouse", "Airport", Items {
        {"Electronics", 50},
        {"Clothes", 100},
    }, 5);
    s.create_payload("Van3", "New Territories Warehouse", "Airport", Items {
        {"Books", 100},
        {"Souvenirs", 30},
    }, 20);
    s.create_payload("Defunct Van", "Hong Kong Island", "Airport", Items {
        {"Souvenirs", 50},
        {"Stationaries", 10},
    }, 1);
    s.remove_payload("Defunct Van");
    s.elapse(3);
    // check if Airport wrongly received contents from Defunct Van
    ASSERT(200, s.get_bases().at("Airport")["Souvenirs"], "Incorrect number of Souvenirs in Base Airport");
    ASSERT(0, s.get_bases().at("Airport")["Stationaries"], "Incorrect number of Stationaries in Base Airport");

    // correct order: Van2, Van1, Van3
    s.elapse(4);
    // check whether airport has correct item count (only Van2)
    ASSERT(100, s.get_bases().at("Airport")["Clothes"], "Incorrect number of clothes in Base Airport after arrival of Payload Van2");
    ASSERT(200, s.get_bases().at("Airport")["Electronics"], "Incorrect number of electronics in Base Airport after arrival of Payload Van2");

    // check whether Van2 is still in payloads
    ASSERT_TRUE(s.get_payloads().find("Van2") == s.get_payloads().end(), "Van2 is still in m_payloads despite its arrival");

    // this should also remove Van3
    s.elapse(13);
    
}

/**
 * @brief elapse
 */
template<>
void test<5>() {
    using Items = initializer_list<ItemPair>;
    World s{initializer_list<Base *> {
        new Base{"Warehouse", Items{{"Box", 100}}},
        new Base{"Store", Items{{"Box", 10}}},
    }};

    // Advancing time with no payload must keep the new time for later payloads.
    s.elapse(7);
    ASSERT(true, s.create_payload("Van", "Warehouse", "Store", Items{{"Box", 20}}, 10), "Wrong return value of create_payload after elapse");
    ASSERT(17, s.get_payloads().at("Van").get_arrival_time(), "Incorrect arrival time after one elapse; current time was not advanced");
    ASSERT(80, s.get_bases().at("Warehouse")["Box"], "Incorrect number of Box in Warehouse after creating a payload");

    // Five more units of time are not enough for Van to arrive, and the time must accumulate.
    s.elapse(5);
    ASSERT_TRUE(s.get_payloads().find("Van") != s.get_payloads().end(), "Payload Van was processed before its arrival time");
    ASSERT(10, s.get_bases().at("Store")["Box"], "Store received Box before Payload Van arrived");

    ASSERT(true, s.create_payload("Bike", "Warehouse", "Store", Items{{"Box", 5}}, 3), "Wrong return value of create_payload after a second elapse");
    ASSERT(15, s.get_payloads().at("Bike").get_arrival_time(), "Incorrect arrival time; elapse did not add to the current time");

    // Current time is 12. Three more units reach Bike at 15 and leave Van at 17.
    s.elapse(3);
    ASSERT_TRUE(s.get_payloads().find("Bike") == s.get_payloads().end(), "Payload Bike was not processed when elapse reached its arrival time");
    ASSERT(15, s.get_bases().at("Store")["Box"], "Incorrect number of Box in Store after Payload Bike arrived");
    ASSERT_TRUE(s.get_payloads().find("Van") != s.get_payloads().end(), "Payload Van was processed before its arrival time");

    s.elapse(2);
    ASSERT_TRUE(s.get_payloads().find("Van") == s.get_payloads().end(), "Payload Van was not processed when elapse reached its arrival time");
    ASSERT(35, s.get_bases().at("Store")["Box"], "Incorrect number of Box in Store after Payload Van arrived");
    ASSERT(75, s.get_bases().at("Warehouse")["Box"], "Incorrect number of Box in Warehouse after both payloads arrived");
}