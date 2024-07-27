# Search for the best Minimally Complex Model in discrete data

A program designed to find the best Minimally Complex Model (MCM) for a given discrete dataset.
As MCMs are spin models containing spin operators of arbitrary interaction order characterized by a low model complexity, this program can detect relevant higher-order interactions within the dataset.
The program can handle datasets with up to 128 variables.

The search process is divided into two steps. The first step is to determine the best set of variables that can be used as the basis representation for the data.
For a system of size $n$, this set of variables consists of $n$ linearly independent spin operators with the lowest entropy.
As this is a computationally demanding step, it is recommended to use this step only for systems with less than 15 variables. 
The second step is to find the best partition of the set of variables, for which three algorithms are implemented: exhaustive search, greedy search and the divide and conquer approach.

### Exhaustive search

The exhaustive search involves generating all possible partitions and calculating their log evidence.
As this algorithm goes through all possible partitions, it is guaranteed to find the best one.
For a system with $n$ variables, the number of different partitions is given by the Bell number of $n$, making this approach unfeasible for large systems (n > 15).

### Greedy search

The greedy search method consists of an iterative merging procedure.
We start with the partition where every variable forms a component, i.e. an MCM with $n$ components of size 1.
Then, we calculate how much the log evidence would change if we would merge any two components.
The merging of two components that leads to the largest increase in log evidence is chosen as the starting point in the next iteration.
If none of the mergings results in an improved log evidence, the algorithm stops and the partition at the starting point of the final iteration becomes the estimated MCM.

Because it does not go through all possible partitions as in the exhaustive search, there is no guarantee of finding the best MCM.
However, by continuously selecting the merge that increases the log evidence the most, the final partition will be a good estimation of the best MCM.

### Divide and conquer

In the divide and conquer method, we start from the complete model, i.e. an MCM with 1 component of size $n$ and perform a recursive splitting process.
Sequentially, we move each variable to a new component and determine the change in log evidence.
From these $n$ different splits, we select the division that increases the log evidence the most or the one that decreases the least if no division results in an increase.
We continue to move variables from the starting component to the new one and select the best change in log evidence until all but one are moved.
Note that if the starting component has more than two variables, we can stop when all but two variables are moved because the next step is the same as just moving the remaining variable, which was checked in the first step.
After this division process, we backtrack to the partition that had the largest log evidence. If this is the starting partition, the algorithm stops.
However, if there was a split that increased the log evidence, we repeat the division process on both components separately.

## Installation

Run the script `build_and_test.sh` to compile the program and execute some [tests](https://github.com/AaronDC60/MinCompSpin_discrete/tree/main/tests) to ensure that the code works correctly:

```
sh build_and_test.sh
````

## Usage

After running the script `build_and_test.sh`, a build folder is created containing the program `mcm_discrete`, which can be executed from the command line:

```
cd build
./mcm_discrete -f filename -q val_of_q -n n_var -search_method
```

The result of the search is written to a file in the `output` folder.

### Command line arguments

* `-f filename` : path to the file containing the data relative to the `input` folder (without the `.dat`).
* `-q val_of_q` : integer that specifies the number of values each variable can take.
* `-n n_var` : number of variables in the system.
* `-search_method` : the chosen search algorithm. Options are `-es` for an exhaustive search, `-gs` for a greedy search and `-dc` for the divide and conquer approach. Multiple options are possible.
* `-gt` : (Optional) Indicates if a transformation to the best basis should be done before one of the search algorithms. Without this option, the program finds the best partition using the original $n$ variables
* `-l` : (Optional) Indicates if the intermediate steps of the search algorithm should be written to a separate file in the `output` folder. Only int the case of the greedy search and divide and conquer method.


## Example

The [input](https://github.com/AaronDC60/MinCompSpin_discrete/tree/main/input) folder contains the US Supreme Court voting dataset.
Running the program for this dataset using a basis transformation and all three search algorithms:

```
cd build
./mcm_discrete -f US_SupremeCourt_n9_N895 -q 2 -n 9 -gt -es -gs -dc
```

The result of this calculating is written to the file [`US_SupremeCourt_n9_N895_output.dat`](https://github.com/AaronDC60/MinCompSpin_discrete/blob/main/output/US_SupremeCourt_n9_N895_output.dat).
The first part contains the spin operators that form the best basis written as bitstrings:

```
Best basis:

000100100
000001010
000000011
100010000
100000100
101000000
010000010
001000001
000000100
```

For example, the first spin operator, `000100100`, is a pairwise interaction between variable 4 and 7.
The spin operators are ordered from low to high entropy.

The second part of the output file is the result of every search algorithm:

```
Exhaustive search: 0s 

Number of best MCMs found : 1

Best MCM(s): 
Component 0 : 100000000
Component 1 : 011000100
Component 2 : 000111011

Best log-evidence: -3154.42
```

For the exhaustive search, the best MCM has three components. The first component contains only the first variable.
Variables 2,3 and 7 form the second component and the third component consists of variables 4,5,6,8 and 9.
In the rare case that multiple MCMs have the same lowest log evidence, all of them will be found (only for the exhaustive search).




