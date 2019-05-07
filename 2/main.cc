#include <map>
#include <memory>
#include <iostream>
#include <vector>

#include <cassert>

using namespace std;

/**
 * given code
 */

size_t get_hash(size_t previous, size_t other) {
  return hash< string >{}(to_string(previous) + to_string(other));
}

/**
 * end of given code
 */


typedef size_t Hash;

struct Block {
  Hash previousHash;
  Hash lastTransaction;
};

typedef size_t AccountID;

struct Account {
  const AccountID id;
  size_t balance;

  Account(const AccountID & i, const size_t b = 1024) : id(i), balance(b) {
    assert(0 < id);
  }

  bool operator != (const Account & a) const {
    return id != a.id;
  }
};

ostream & operator << (ostream & o, const Account & a) {
  o << a.id << " " << a.balance;
  return o;
}

typedef shared_ptr< Account > AccountPointer;
typedef weak_ptr< Account > AccountWeakPointer;

typedef map< AccountID, AccountPointer > Accounts;
typedef map< Hash, AccountWeakPointer > Tips;


/**
 * Rather than storing and subsequently following blocks,
 * the strategy here was to have accounts with balances into a map of the latest valid tips.
 * This approach prioritizes latency.
 */

int main(void) {
  vector< Block > blocks;
  Accounts accounts;
  Tips tips;

  while (true) {
    size_t h, l;
    cin >> h >> l;
    if ( ! cin.good()) {
      break;
    }

    const Block b{h, l};

    //genesis
    if (0 == h) {
      const Accounts::const_iterator iterator = accounts.find(b.lastTransaction);
      if (accounts.cend() == iterator) {
        AccountPointer account = make_shared< Account >(b.lastTransaction);
        const Hash hash = get_hash(b.previousHash, b.lastTransaction);
        tips[hash] = account;
        accounts[account->id] = move(account);

      } else {
        /*
         * ignoring repeated genesis block
         */

        cerr << "this account \"" << b.lastTransaction
          << "\" was previously created." << endl;
      }

    } else {
      blocks.emplace_back(b);
    }
  }

  for (const auto & b : blocks) {
    const Tips::const_iterator receiverIterator = tips.find(b.previousHash),
          senderIterator = tips.find(b.lastTransaction);

    if (tips.cend() != receiverIterator && tips.cend() != senderIterator) {
      assert(receiverIterator != senderIterator);

      const AccountPointer sender = senderIterator->second.lock(),
            receiver = receiverIterator->second.lock();

      if (sender && receiver) {
        // sender different than receiver.
        assert(*sender != *receiver);

        // sender has a balance of at least 2 coins.
        assert(1 < sender->balance);

        // may loose precision.
        sender->balance /= 2;
        receiver->balance += sender->balance;

        //new tip for receiver
        {
          const Hash hash = get_hash(b.previousHash, b.lastTransaction);
          tips.erase(receiverIterator);
          tips[hash] = receiver;
        }

        //new tip for sender
        {
          const Hash hash = get_hash(b.lastTransaction, b.previousHash);
          tips.erase(senderIterator);
          tips[hash] = sender;
        }
      }

    } else {
      /*
       * ignoring invalid regular block
       */
      cerr << "invalid block" << endl;
    }
  }

  for (const auto & item : accounts) {
    cout << *item.second << endl;
  }

  return 0;
}
