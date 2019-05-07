# Blocklattice

Write a validating parser for blocklattice data given in random order on standard input that prints the final balances of all accounts to standard output sorted in ascending order of account id.


Block consists of two numbers of type `size_t`: 1) hash of previous block from that account and 2) hash of last transaction from another account.


Block's hash is of type `size_t` and is calculated from above two numbers with given function `get_hash()`.


First block in chain - genesis block - has 0 as hash of previous block and random number of type `size_t` indicating genesis block's account id. Each account starts with balance of 1024 indivisible coins at genesis block.


First genesis block is always valid and a regular block is only valid if it refers to previous valid block in that account's chain and valid block from another account.


Valid block transfers half of other account's balance to block creator's account.


In case of conflicting transactions, e.g. where more than one block tries to take coins from same account or account creates more than one block referring to the same prior block from that account, only first one received on standard input is valid.


Input consists of ASCII formatted lines with two numbers per line separated by whitespace. First number on each line is block's hash of previous block and second number is block's hash of another account's chain's block from which half of balance is taken.


Example input:

```
567 789
0 123
0 321
8099924760385273564 4922773860975567173
987 654
```


Expected output:

```
123 512
321 1536
```

Explanation: Genesis blocks create accounts 123 and 321 with 1024 coins each. Blocks that have hash of previous block as 567 and 987 are invalid because they refer to blocks that don't exist. Block "809... 492..." transfers half of balance from account whose chain has a block with hash 492... to account whose chain has block with hash 809... which was account 321 created by genesis block "0 321". Hence 512 coins are transferred from account 123 to account 321 making final totals as printed to standard output.
