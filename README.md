# Hash table

## Contents
1. [Goals](#goals)
2. [Brief theory](#brief-theory)
    - [What is a hash table?](#what-is-a-hash-table)
    - [What hash table do we use?](#what-hash-table-do-we-use)
3. [Implementation details](#implementation-details)
4. [Project structure](#project-structure)
5. [Downloading project](#downloading-and-using-project)
6. [My tests results](#my-tests-results)
    - [Hash functions tests](#hash-functions-tests)

## Goals
1. **Implement** a hash table that can receive different hash functions.
2. **Compare** hash functions and choose the most efficient.
3. **Explore** several code fragments within different compiler optimization flags.
4. **Analyze** code via [perf](https://perf.wiki.kernel.org/index.php/Main_Page) utility and find code bottlenecks.
5. **Optimize** bottlenecks by other decisions in code architecture.
6. **Optimize** code bottlenecks with intrinsic functions and inline assembly.

## Brief theory
### What is a hash table?
[Hash table](https://en.wikipedia.org/wiki/Hash_table) is a data structure that implements an associative array, also called a dictionary, which is an abstract data type that maps *keys* to *values*.
A hash table uses a hash function to compute an *index* into an array of *buckets*, from which the desired value can be found. When there are several values in one bucket, it is called *collision*.

### What hash table do we use?
For hash tables, the main goal is to choose a proper *hash function* and find the most efficient and quick ways to solve *collisions*. During this project we are using a [separate chaining](https://en.wikipedia.org/wiki/Hash_table#Separate_chaining) method based on a doubly-linked list.

![Separate chaining](/img/chained_hash_table.jpg)

As for the hash functions' choice, there will be tests for some of them to choose more suitable one.

## Implementation details
Detailed code description can be found in [documentation](). This paragraph is about some choices, their pros and cons.
1. Doubly-linked list used instead of a single-linked to gain delete and pushback functions complexity $\mathcal{O}(1)$. It uses more memory, but gives much better performance.
2. Lists are based on a separated in memory sequence of nodes.
3. I wanted to make a universal hash table implementation for abstract data types, so it uses `void*` types and receives data sizes. Although this method is universal, it uses more allocations and more complex comparisons than with a fixed data type.
4. Hash table **does not** perform a rehash when loading factor is too high. It uses a constant number of buckets.
5. `Calloc()` and `malloc()` functions are both used. `malloc()` function is used when data is being initialized immediately after, and `calloc()` is used when memory might stay uninitialized for some time (like array allocation).

## Project structure
Despite C language limitations, I wanted to make this project more OOP-like. It means that the project can be separated into several modules, which can be changed without recompiling other modules.

![Modules](/img/modules.jpg)

The project uses my [library]() for splitting text into strings depending on given separator function - I use it to split huge text into words, skipping punctuation and spaces. I also decided to make it case-sensitive, so "Coffee" and "coffee" words were different.

Here is hte project directory simple structure:
```
├── img/        // Images for the readme
├── include/    // Header files, interfaces
├── lib/        // Text separation library
├── output/     // Text results for testing
├── source/     // Source files
├── test/       // Tests for the hash table
├── README.md   // What you're reading right now
└── makefile    // Your best friend, does everything for you
```

## Downloading and using project
Here are the steps to download the project and run the tests:
1. Download from the [repository]() or run `git clone ` command in your terminal.
2. Open terminal in `hash_table` folder. If you downloaded the repository via `git clone`, simply run `cd hash_table/` command.
3. Place the file you would like to test the hash table on in the `hash_table` folder. Name it `text.txt`.
4. If you would like to use another name for the input file or use more buckets in the hash table for the tests, open `makefile` via any text editor and search for the 83'd and 84's row respectively and change them:
```
82  # Rum params
83  TEXT		:= text.txt     # input file name
84  HT_SIZE		:= 2000         # hash table buckets number
```

5. Run `make run_functions_test` to run the tests for the hash functions. They will make new plots for your text inside `img` folder.

6. ***Not yet implemented***

## My tests results
I ran the tests on the "Crime and punishment" text from Fyodor Dostoevsky (in English). It had around 10'000 unique words. I ran tests using a hash table with 2'000 buckets.

### Hash functions tests
My goal was to compare hash functions' distributions to see which of them can be used effectively.
Here is a list of hash functions (maybe I will expand it later):
```
1. HashFunctionZero         // simply returns 0 as bucket index
2. HashFunctionFirstASCII   // returns ASCII code of the first character in the word
3. HashFunctionStringLength // returns length of the word
4. HashFunctionSumASCII     // returns sum of all word's characters ASCII codes
5. HashFunctionDjb2         // the DJB2 algorythm
6. HashFunctionCrc32        // the CRC32 algorythm
```
Results:
![HashFunctionZero](/img/zero_index.png)
![HashFunctionFirstASCII](/img/first_ascii.png)
![HashFunctionStringLength](/img/word_length.png)
![HashFunctionSumASCII](/img/ascii_sum.png)
![HashFunctionDjb2](/img/djb2.png)
![HashFunctionCrc32](/img/crc32.png)

| Function | Dispersion |
|----------|------------|
| Zero | $5.1 \cdot 10^4$|
| FirstASCII | $2.5 \cdot 10^3$ |
| StringLength | $5.8 \cdot 10^3 $ |
| SumASCII | $6.7 \cdot 10^1 $ |
| Djb2 | $5.0$ |
| Crc32 | $5.2$ |

As we can, predictably, see, HashFunctionZero has enormous dispersion value (if you try to run tests, you will see that it also hits the working time). HashFunctionSumASCII may be used if the hash table is small enough.
The best hash functions are djb2 and crc32 - the difference between them is something debatable, it depends on the text.
