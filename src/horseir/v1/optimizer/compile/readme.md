
## run.sh: executing compiled horseir

see usage

    ./run.sh

## debug: logging files

generate a log file (10 runs)

     (make release && ./run.sh 14 10) &> debug/q14.log

## opt: storing auto-generated optimized code 


## exp: hand-tuning code from opt code

- manually profile, test, and debug horseir code
- need to modify `loadcode.h` to update header files


