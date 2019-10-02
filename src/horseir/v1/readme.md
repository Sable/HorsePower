
## In progress

./horse -c -q 22

## Cache fusion patterns

- Within a query program
- Recognize the code with the same function signature
- Avoid generating duplicate code from the optimizer

## Debugging flags

PROFILE_HASH_JOIN

- Output debugging info for join's average search (i.e. total_search/total_item)

## Probe generated code

Assembly code

    gcc -S backend/h_primitive.c -ll -fopenmp -std=c99 -lm -march=native -D_POSIX_SOURCE -O3 -I"../../../libs/include/"

Search for a set of SSE4.2 instructions (see [more](https://en.wikipedia.org/wiki/SSE4#SSE4.2))

    cat -n h_primitive.s | grep -i "\s*pcm" | wc -l

Profile calgrind result files

    # flag: -O3 -g
    cg_annotate cachegrind.out.30139 --auto=yes &> /tmp/profile-q5-single.txt

