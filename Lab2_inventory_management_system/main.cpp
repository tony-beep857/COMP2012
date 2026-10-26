#include "inventory.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;


const int NUM_TESTCASES = 6;
void run_test(int i);

int main(int argc, char** argv) {
    // flush cout after every output, so that nothing is lost if the program crashes
    cout << unitbuf;
    if (argc == 1) {
        for (int i = 1; i <= NUM_TESTCASES; i++)
            run_test(i);
    } else {
        // test a particular test case
        string s{argv[1]};

        int to_test = stoi(s);
        run_test(to_test);
    }
    return 0;
}

void test1(), test2(), test3(), test4(), test5(), test6();

// rudimentary method as students still haven't learnt function pointers or templates yet
void run_test(int i) {
    cout << "Beginning of test " << i << "\n";
    switch (i) {
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        case 4:
            test4();
            break;
        case 5:
            test5();
            break;
        case 6:
            test6();
            break;
        default:
            cout << "test " << i << " does not exist.\n";
            return;
    }
    cout << "End of test " << i << "\n";
}

// usage: prints a message if the condition is false
#define ASSERT(boolean, message) do { if (!(boolean)) cout << "Assertion fail in main.cpp line " << __LINE__ << ": " << message << "\n"; } while (false)

// returns true if the slots of inv are correctly linked in both directions, and their number is inv.get_count()
bool is_consistent(const Inventory& inv) {
    if (inv.get_head()->get_prev() != nullptr || inv.get_tail()->get_next() != nullptr)
        return false;
    int count = 0;
    const Slot* prev = inv.get_head();
    for (const Slot* s = prev->get_next(); s != inv.get_tail(); s = s->get_next()) {
        if (s == nullptr || s->get_prev() != prev || count > inv.get_capacity())
            return false;
        prev = s;
        count++;
    }
    return inv.get_tail()->get_prev() == prev && count == inv.get_count();
}

/**
 * @brief Slot constructors: deep copy of the name, and linking right after prev
 */
void test1() {
    char name[] = "Iron Ore";
    Slot* s1 = new Slot(name, 64, nullptr);
    name[0] = 'X'; // changing the given name must not change the slot's name

    ASSERT(strcmp(s1->get_name(), "Iron Ore") == 0, "Wrong name for s1 (is the name deep copied?)");
    ASSERT(s1->get_count() == 0, "Count not set to 0 for s1");
    ASSERT(s1->get_max_items() == 64, "Wrong max items for s1");
    ASSERT(s1->get_prev() == nullptr, "s1 was created with prev nullptr, but its prev is not nullptr");
    ASSERT(s1->get_next() == nullptr, "s1 was created with prev nullptr, but its next is not nullptr");

    // create a slot at the end: s1 -> s2
    Slot* s2 = new Slot("Copper Ore", 50, s1);
    ASSERT(s1->get_next() == s2, "s1's next is not s2 after creating s2 after s1");
    ASSERT(s2->get_prev() == s1, "s2's prev is not s1 after creating s2 after s1");
    ASSERT(s2->get_next() == nullptr, "s2 is the last slot, but its next is not nullptr");

    // create a slot in the middle: s1 -> s3 -> s2
    Slot* s3 = new Slot("Coal", 50, s1);
    ASSERT(s1->get_next() == s3, "s1's next is not s3 after creating s3 after s1");
    ASSERT(s3->get_prev() == s1, "s3's prev is not s1 after creating s3 after s1");
    ASSERT(s3->get_next() == s2, "s3's next is not s2 after creating s3 between s1 and s2");
    ASSERT(s2->get_prev() == s3, "s2's prev is not s3 after creating s3 between s1 and s2");

    // copy the contents of s2 to a new slot after s3: s1 -> s3 -> s4 -> s2
    s2->fill(20);
    Slot* s4 = new Slot(*s2, s3);
    ASSERT(strcmp(s4->get_name(), "Copper Ore") == 0, "Wrong name for s4, copied from s2");
    ASSERT(s4->get_name() != s2->get_name(), "s4's name is not deep copied from s2 (both point to the same char array)");
    ASSERT(s4->get_count() == 20, "Wrong count for s4, copied from s2");
    ASSERT(s4->get_max_items() == 50, "Wrong max items for s4, copied from s2");
    ASSERT(s3->get_next() == s4, "s3's next is not s4 after creating s4 after s3");
    ASSERT(s4->get_prev() == s3, "s4's prev is not s3 after creating s4 after s3");
    ASSERT(s4->get_next() == s2, "s4's next is not s2 after creating s4 between s3 and s2");
    ASSERT(s2->get_prev() == s4, "s2's prev is not s4 after creating s4 between s3 and s2");
    ASSERT(s2->get_count() == 20, "s2's count changed after copying it");

    delete s1;
    delete s3;
    delete s4;
    delete s2;
}

/**
 * @brief Slot destructor: unlinking from the middle, the start and the end of a list
 */
void test2() {
    // s1 -> s2 -> s3 -> s4 -> s5
    Slot* s1 = new Slot("a", 10, nullptr);
    Slot* s2 = new Slot("b", 10, s1);
    Slot* s3 = new Slot("c", 10, s2);
    Slot* s4 = new Slot("d", 10, s3);
    Slot* s5 = new Slot("e", 10, s4);

    // delete a middle slot: s1 -> s2 -> s4 -> s5
    delete s3;
    ASSERT(s2->get_next() == s4, "s2's next is not s4 after deleting s3");
    ASSERT(s4->get_prev() == s2, "s4's prev is not s2 after deleting s3");

    // delete the first slot: s2 -> s4 -> s5
    delete s1;
    ASSERT(s2->get_prev() == nullptr, "s2's prev is not nullptr after deleting the first slot s1");

    // delete the last slot: s2 -> s4
    delete s5;
    ASSERT(s4->get_next() == nullptr, "s4's next is not nullptr after deleting the last slot s5");
    ASSERT(s2->get_next() == s4, "s2's next is not s4 anymore");
    ASSERT(s4->get_prev() == s2, "s4's prev is not s2 anymore");

    // creating then deleting a slot leaves the list as it was: s2 -> s4
    Slot* temp = new Slot("temp", 10, s2);
    delete temp;
    ASSERT(s2->get_next() == s4, "s2's next is not s4 after creating then deleting a slot between s2 and s4");
    ASSERT(s4->get_prev() == s2, "s4's prev is not s2 after creating then deleting a slot between s2 and s4");

    // delete the last 2 slots
    delete s2;
    ASSERT(s4->get_prev() == nullptr, "s4's prev is not nullptr after deleting s2");
    delete s4;
}

/**
 * @brief Inventory constructor and destructor
 */
void test3() {
    Inventory i1{10};

    ASSERT(i1.get_count() == 0, "A newly created inventory does not have 0 slots");
    ASSERT(i1.get_capacity() == 10, "Wrong capacity for a newly created inventory");
    ASSERT(strcmp(i1.get_head()->get_name(), HEAD_NAME) == 0, "Wrong name for head dummy slot");
    ASSERT(strcmp(i1.get_tail()->get_name(), TAIL_NAME) == 0, "Wrong name for tail dummy slot");
    ASSERT(i1.get_head()->get_next() == i1.get_tail(), "m_head of a newly created inventory does not point to m_tail");
    ASSERT(i1.get_tail()->get_prev() == i1.get_head(), "m_tail of a newly created inventory does not point to m_head");
    ASSERT(i1.get_head()->get_prev() == nullptr, "m_head's prev is not nullptr");
    ASSERT(i1.get_tail()->get_next() == nullptr, "m_tail's next is not nullptr");

    // an inventory created with new and destroyed with delete
    Inventory* i2 = new Inventory{5};
    ASSERT(i2->get_head()->get_next() == i2->get_tail(), "m_head of a newly created inventory does not point to m_tail");
    delete i2;
}

/**
 * @brief add and take
 */
void test4() {
    Inventory i1{3};

    ASSERT(i1.add("Iron Ore", 64, 30) == 0, "Non-zero returned despite the new slot holding all 30 Iron Ores");
    ASSERT(i1.get_count() == 1, "Count not set from 0 to 1 after adding 1 slot");
    ASSERT(i1.add("Copper Ore", 50, 70) == 20, "Incorrect leftover when adding 70 Copper Ores with max_items = 50");
    ASSERT(i1.add("Coal", 50, 5) == 0, "Non-zero returned despite the new slot holding all 5 Coals");
    // Iron Ore 30, Copper Ore 50, Coal 5
    ASSERT(i1.get_count() == 3, "Count not set to 3 after adding 3 slots");
    ASSERT(i1.add("Stone", 64, 1) == -1, "Adding to a full inventory does not return -1");
    ASSERT(i1.get_count() == 3, "Count changed after adding to a full inventory");
    ASSERT(is_consistent(i1), "The slots are not correctly linked after adding 3 slots");

    // slots are added at the end, in order
    ASSERT(strcmp(i1.get_ith_const(0)->get_name(), "Iron Ore") == 0, "Slot 0 is not Iron Ore");
    ASSERT(strcmp(i1.get_ith_const(1)->get_name(), "Copper Ore") == 0, "Slot 1 is not Copper Ore");
    ASSERT(strcmp(i1.get_ith_const(2)->get_name(), "Coal") == 0, "Slot 2 is not Coal");
    ASSERT(i1.get_ith_const(1)->get_count() == 50, "Slot 1 (Copper Ore) not filled to 50");
    ASSERT(i1.get_ith_const(1)->get_max_items() == 50, "Wrong max items for slot 1 (Copper Ore)");

    ASSERT(i1.take(3, 1) == -1, "Taking from out-of-bounds slot 3 does not return -1");
    ASSERT(i1.take(-1, 1) == -1, "Taking from out-of-bounds slot -1 does not return -1");

    // take without emptying the slot
    ASSERT(i1.take(1, 20) == 0, "Non-zero returned when taking 20 from 50 Copper Ores");
    ASSERT(i1.get_ith_const(1)->get_count() == 30, "Slot 1 (Copper Ore) does not hold 30 items after taking 20 from 50");
    ASSERT(i1.get_count() == 3, "Count changed although no slot became empty");

    // empty a middle slot: Iron Ore 30, Coal 5
    ASSERT(i1.take(1, 40) == 10, "Incorrect leftover when taking 40 from 30 Copper Ores");
    ASSERT(i1.get_count() == 2, "Count not decreased from 3 to 2 after emptying slot 1");
    ASSERT(strcmp(i1.get_ith_const(1)->get_name(), "Coal") == 0, "Slot 1 is not Coal after removing Copper Ore");
    ASSERT(is_consistent(i1), "The slots are not correctly linked after removing a middle slot");

    // empty the first slot exactly: Coal 5
    ASSERT(i1.take(0, 30) == 0, "Non-zero returned when taking exactly all 30 Iron Ores");
    ASSERT(i1.get_count() == 1, "Count not decreased from 2 to 1 after emptying slot 0");
    ASSERT(is_consistent(i1), "The slots are not correctly linked after removing the first slot");

    // empty the last slot: the inventory is empty
    ASSERT(i1.take(0, 5) == 0, "Non-zero returned when taking exactly all 5 Coals");
    ASSERT(i1.get_count() == 0, "Count not set to 0 after emptying the inventory");
    ASSERT(i1.get_head()->get_next() == i1.get_tail(), "m_head does not point to m_tail after emptying the inventory");
    ASSERT(i1.get_tail()->get_prev() == i1.get_head(), "m_tail does not point to m_head after emptying the inventory");

    // the inventory can be used again, and is destroyed with slots in it
    ASSERT(i1.add("Wood", 100, 10) == 0, "Non-zero returned when adding to an emptied inventory");
    ASSERT(i1.add("Stone", 64, 64) == 0, "Non-zero returned when adding exactly enough Stone to fill a slot");
    ASSERT(i1.get_count() == 2, "Count not set to 2 after adding 2 slots to an emptied inventory");
    ASSERT(is_consistent(i1), "The slots are not correctly linked after adding slots to an emptied inventory");
}

/**
 * @brief Inventory copy constructor
 */
void test5() {
    Inventory original{5};
    original.add("Iron Ore", 64, 30);
    original.add("Copper Ore", 50, 45);
    original.add("Coal", 50, 5);

    Inventory backup{original};
    ASSERT(backup.get_count() == 3, "The copy does not have the same count as the original");
    ASSERT(backup.get_capacity() == 5, "The copy does not have the same capacity as the original");
    ASSERT(backup.get_head() != original.get_head(), "The copy shares its head dummy slot with the original");
    ASSERT(backup.get_tail() != original.get_tail(), "The copy shares its tail dummy slot with the original");
    ASSERT(is_consistent(backup), "The slots of the copy are not correctly linked");
    // compare the slots one by one
    for (int i = 0; i < 3; i++) {
        const Slot* s = original.get_ith_const(i);
        const Slot* c = backup.get_ith_const(i);
        ASSERT(c != s, "Slot " << i << " of the copy is the original's slot itself, not a copy");
        ASSERT(strcmp(c->get_name(), s->get_name()) == 0, "Slot " << i << " of the copy has the wrong name");
        ASSERT(c->get_name() != s->get_name(), "Slot " << i << " of the copy shares its name with the original (not deep copied)");
        ASSERT(c->get_count() == s->get_count(), "Slot " << i << " of the copy has the wrong count");
        ASSERT(c->get_max_items() == s->get_max_items(), "Slot " << i << " of the copy has the wrong max items");
    }

    // changing the copy does not change the original
    backup.fill(0, 30);
    backup.fill(1, 5);
    ASSERT(original.get_ith_const(0)->get_count() == 30, "Filling the copy changed the original's slot 0");
    ASSERT(original.get_ith_const(1)->get_count() == 45, "Filling the copy changed the original's slot 1");

    // destroying a copy does not affect the original
    {
        Inventory temp{original};
        temp.fill(1, 5);
    } // temp is destroyed here
    ASSERT(original.get_count() == 3, "Destroying a copy changed the original's count");
    ASSERT(strcmp(original.get_ith_const(1)->get_name(), "Copper Ore") == 0, "Destroying a copy changed the original's slot 1");
    ASSERT(original.get_ith_const(1)->get_count() == 45, "Destroying a copy changed the original's slot 1 count");

    // copying an empty inventory
    Inventory empty{2};
    Inventory empty_copy{empty};
    ASSERT(empty_copy.get_count() == 0, "The copy of an empty inventory is not empty");
    ASSERT(empty_copy.get_capacity() == 2, "The copy of an empty inventory does not have the same capacity");
    ASSERT(empty_copy.get_head()->get_next() == empty_copy.get_tail(), "In the copy of an empty inventory, m_head does not point to m_tail");
}

/**
 * @brief A complete scenario: backing up an inventory before a boss fight, then restoring it, uses everything
 */
void test6() {
    Inventory* inventory = new Inventory{4};
    inventory->add("Sword", 1, 1);
    inventory->add("Health Potion", 16, 10);
    ASSERT(inventory->add("Arrow", 64, 100) == 36, "Incorrect leftover when adding 100 Arrows with max_items = 64");
    // Sword 1, Health Potion 10, Arrow 64

    Inventory* backup = new Inventory{*inventory};

    // the boss fight: use all potions and arrows, then pick up loot
    ASSERT(inventory->take(1, 10) == 0, "Non-zero returned when taking all 10 Health Potions");
    ASSERT(inventory->take(1, 70) == 6, "Incorrect leftover when taking 70 Arrows out of 64");
    ASSERT(inventory->get_count() == 1, "Only the Sword should be left after using all potions and arrows");
    ASSERT(inventory->add("Dragon Scale", 8, 3) == 0, "Non-zero returned when adding 3 Dragon Scales");
    ASSERT(inventory->add("Gold Coin", 999, 500) == 0, "Non-zero returned when adding 500 Gold Coins");
    ASSERT(inventory->add("Dragon Egg", 1, 1) == 0, "Non-zero returned when adding 1 Dragon Egg");
    ASSERT(inventory->add("Bone", 64, 12) == -1, "Adding to a full inventory does not return -1");
    ASSERT(is_consistent(*inventory), "The slots are not correctly linked after the boss fight");

    // the player lost: the inventory is replaced by a copy of the backup, and the backup is deleted
    delete inventory;
    inventory = new Inventory{*backup};
    delete backup;
    ASSERT(inventory->get_count() == 3, "The restored inventory does not have 3 slots");
    ASSERT(is_consistent(*inventory), "The slots of the restored inventory are not correctly linked");
    ASSERT(strcmp(inventory->get_ith_const(0)->get_name(), "Sword") == 0, "Slot 0 of the restored inventory is not the Sword");
    ASSERT(inventory->get_ith_const(1)->get_count() == 10, "The restored inventory does not have 10 Health Potions");
    ASSERT(inventory->get_ith_const(2)->get_count() == 64, "The restored inventory does not have 64 Arrows");

    // the restored inventory can still be used
    ASSERT(inventory->add("Bread", 16, 4) == 0, "Non-zero returned when adding 4 Bread to the restored inventory");
    ASSERT(inventory->add("Bone", 64, 12) == -1, "Adding to the full restored inventory does not return -1");
    ASSERT(is_consistent(*inventory), "The slots of the restored inventory are not correctly linked after adding a slot");
    delete inventory;
}
