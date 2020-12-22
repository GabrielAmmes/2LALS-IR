# 2LALS-IR
 Source code for the paper: "A Two-Level Approximate Logic Synthesis Combining Cube Insertion and Removal", submitted for TCAD on 22/12/20.
 
## BUILD 2LALS-IR
```
make
```
## BUILD ESPRESSO
```
cd espresso
make 
cd ..
 ```

## RUNNING
### MAIN COMMAND
```
./main -t 0 -i PATH -o PATH -[en|er] ERROR
```

 - -i - Input Path
 - -o - Output Path/ Second Input
 - -er - Error Rate - Max percentual of erroneous input combination allowed. [UNS]
 - -en - Number of Errors - Max number of erroneous input combination allowed. [UNS]

### EXAMPLE
```
./main -t 0 -i data/iwls/sao2.pla -o app/ -er 5
```
 Approximate the sao2.pla benchmark with 5% ER threshold and stores in app folder.

 ### OTHER COMMANDS
 #### Literal Count:
```
./main -t 1 -i PATH
```
 #### Cubes Count:
```
./main -t 2 -i PATH
```
 #### Error Rate Count:
```
./main -t 3 -i PATH -o PATH
```
 #### Help:
```
./main -h
```
 
 ### SCRIPTS
 run_table1.sh: Execute the method on benchmarks in folder benchmarks/iwls/ with a fixed EN of 16 and stores the approximated circuits in app/table1. Results presented on paper's Table 1.
 
 run_table2.sh: Execute the method on benchmarks in folder benchmarks/table2/ with a percentege ER of 1, 3 and 5, and stores the approximated circuits in app/table2. Results presented on paper's Table 2.
 
 run_all.sh: Execute the method on benchmarks in folder benchmarks/iwls/ with a percentege ER of 1, 3 and 5, and stores the approximated circuits in app/all.
 


