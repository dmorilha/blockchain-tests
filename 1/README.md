# Blockchain

Write a validating parser for blockchain data given on standard input that prints a valid blockchain to standard output.


Block consists of two numbers of type `size_t`: 1) hash of previous block and 2) payload.


Block's hash is calculated as `std::hash<size_t>` of bitwise xor of previous hash and payload.


First block in chain - genesis block - has 0 as hash of previous block and only one will ever be submitted through standard input.


Block is only valid if it's hash of previous block is equal to another valid block hash, except for genesis block.


Input consists of ASCII formatted lines with two numbers per line separated by whitespace. First number on each line is block's hash of previous block and second number is block's payload.


Output must follow same format as input but consist of only valid blocks in the order specified by the valid blockchain starting from genesis block.


Example input:

```
1 321
0 123
123 456
```


Example output:

```
0 123
123 456
```

Explanation: Block "1 321" is not a valid block because a block with hash 1 doesn't exist. Block "0 123" is genesis block which is always valid. Block "123 456" refers to genesis block and is thus valid.
