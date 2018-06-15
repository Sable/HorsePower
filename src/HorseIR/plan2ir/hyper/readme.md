## List of Queries

| ID | Verify | Status | is10 | Joins                                    | Special
|----|:------:|--------|------|------------------------------------------|-------------------------
| 1  | V      | Pass   |  Y   | 0 join                                   |
| 2  | V      | Pass*  |      | 8 joins                                  | earlyprobe, is
| 3  | V      | Pass   |  Y   | 1 join,  1 groupjoin                     |
| 4  | V      | Pass   |  Y   | 1 leftsemijoin                           |
| 5  | V      | Pass   |      | 5 joins                                  |
| 6  | V      | Pass   |  Y   | 0 join                                   |
| 7  | V      | Pass*  |      | 5 joins                                  | bnl
| 8  | V      | Pass*  |      | 7 joins                                  |
| 9  | V      | Pass*  |      | 5 joins                                  |
| 10 | V      | Pass   |      | 3 joins                                  |
| 11 | V      | Pass   |      | 5 joins                                  | bnl join
| 12 | V      | Pass   |      | 1 join                                   | lookup
| 13 | V      | Pass   |      | 1 groupjoin                              | (left-outer-join)
| 14 | V      | Pass   |  Y   | 1 join                                   |
| 15 | V      | Pass   |      | 1 join,  1 index join                    |
| 16 | V      | Pass   |  Y   | 1 join,  1 rightantijoin                 |
| 17 | V      | Pass*  |  Y   | 2 joins, 1 groupjoin                     | groupby scan, magic?
| 18 | V      | Pass   |  Y   | 2 joins, 1 rightsemijoin                 | select
| 19 | V      | Pass   |  Y   | 2 join                                   |
| 20 | V      | Pass*  |      | 2 joins, 1 leftsemijoin, 1 rightsemijoin | earlyprobe
| 21 | V      | Pass*  |      | 3 joins, 1 leftsemijoin, 1 leftantijoin  |
| 22 | V      | Pass   |  Y   | 1 join,  1 leftantijoin                  | bnl


Note (Jun 14)

- No join : 1,6
- 1  join : 4,12,13,14
- 2  joins: 16 (17,19,22)

Testing: 3,18

Note (May 7)

- Special join: `<`, `>`, `is`, `<>` (other than `=`)
    + q17/20/21
- `whichTableByName`: decide a column from which side (0: left or 1:right)

Note (May 6)

- V: viewed; P: problematic; F: fail
- Need a primitive `same` to find a pair of equal lists (list1 = list2)

Note:

- `is` is treated as '=' in table join
- \* means that general join impl. missing
- `./list.sh 2>&1 | grep -i query`
- `./list.sh 2>&1 | grep -i time`
- `./list.sh 2>&1 | grep -i todo`
- `time ./list.sh 2>&1  | grep -Ei 'todo|query'`: fetch todo list in each query


Done list

- tablescan
- group by
	+ values
	+ aggregates: leftAggregates, rightAggregates (q3/13/17)
- sort
	+ criterion: [ {value} ... ]
- groupjoin (q13)
- map
	+ values (expressions?, q14)
- temp (q15)
	+ tempscan
- lookup
	+ Is it 'or'? In q7/12
- select
	+ means 'having' clause
- join
	+ method "indexnl" (q12/14)
	+ method "hash"    (q19)
	+ method "bnl"     (q7/11/22)
- rightantijoin (q16)
- leftsemijoin  (q4/21)
- leftantijoin  (q21/22)
- rightsemijoin (q18/20)
- groupbyscan   (q17)
- earlyprobe    (q20)

Todo list

- general join in array programming
	+ q2/5/7/8/9/11/15/17/20/21/22
- program debugging
- performance tuning

## Figures

Screenshots of query plans from the HyPer's web (see dir: `fig/`)

| Figures (A)          | Figures (B)            | Figures (C)            | Figures (D)            |
| :------------------: | :--------------------: | :--------------------: | :--------------------: |
| [q1.png](fig/q1.png) | q7.png                 | q13.png                | [q19.png](fig/q19.png) |
| [q2.png](fig/q2.png) | q8.png                 | q14.png                | q20.png                |
| q3.png               | q9.png                 | q15.png                | q21.png                |
| [q4.png](fig/q4.png) | q10.png                | q16.png                | q22.png                |
| q5.png               | q11.png                | [q17.png](fig/q17.png) | |
| [q6.png](fig/q6.png) | [q12.png](fig/q12.png) | q18.png                | |

## Folders

Description of folders

    - raw/ Execution plans downloaded from hyper online
    - opt/ Optimized plans extracted from raw
    - fig/ Hyper's optimized execution plans in figures

    - gen/ Execution plans generated in HorseIR (Optimized)


## Issues Remaining

1. Handle date types in HyPer
2. Handle the enum type
3. Idea: optimization for @raze (a static analysis for removing unnecessary raze operations logically)

What is early probe?

- Currently, we treat it as a normal table scan.


 16K q8   I
 14K q2   I
 13K q21  .
 13K q9   I
 12K q7   I <-- Incomplete
 11K q5   Y
 11K q19  F <-- Fail, join condition and + or
 10K q20  .
9.7K q11  Y
9.6K q18  Y
9.2K q22  Y
9.0K q17  .
8.4K q10  Y
7.2K q16  Y
6.9K q12  Y
6.5K q3   Y
6.3K q15  Y
5.8K q14  Y
4.9K q1   Y
4.4K q4   Y
4.3K q13  Y
2.9K q6   Y


How block nested loop (bnl) join works?

- https://mariadb.com/kb/en/library/block-based-join-algorithms/#how-block-nested-loop-join-works

Bugs on HyPer web: visual issues

- q18, operatorId 7 for 'select', condition should be 'v10 > 30000'

