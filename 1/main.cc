#include <iostream>
#include <map>
#include <string>

#include <cassert>

using namespace std;

typedef size_t Hash;

struct Block {
  Hash previousHash;
  size_t payload;
};

typedef map< Hash, Block > BlockMap;

ostream & operator << (ostream & o, const Block & b) {
  o << b.previousHash << " " << b.payload;
  return o;
}

ostream & operator << (ostream & o, const BlockMap::value_type & v) {
  o << v.second << " (hash " << v.first << ")";
  return o;
}

template < >
struct std::hash< Block > {
  Hash operator () (const Block & b) const {
    return b.previousHash ^ b.payload;
  }
};

int main(void) {
  BlockMap blockMap;
  bool foundGenesis = false;

  while (true) {
    size_t h, p;
    cin >> h >> p;
    if ( ! cin.good()) {
      break;
    }
    const Block b{h, p};
    if (0 == h) {
      assert( ! foundGenesis);
      foundGenesis = true;
    }
    blockMap[b.previousHash] = b;
  }

  if ( ! blockMap.empty()) {
    const hash< Block > hasher;
    BlockMap::const_iterator iterator = blockMap.find(0);
    assert(blockMap.cend() != iterator);
    while (blockMap.cend() != iterator) {
      cout << iterator->second << endl;
      iterator = blockMap.find(hasher(iterator->second));
    }
  }

  return 0;
}
