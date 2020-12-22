#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <deque>
#include <tuple>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <math.h>
#include <chrono>
#include <iterator>
using namespace std;

struct LeafComparator{
    bool operator () (const pair<unsigned long long,vector<unsigned>> &l1,
    const pair<unsigned long long,vector<unsigned>> &l2){
        if(l1.second[0] != l2.second[0]){
            return l1.second[0] > l2.second[0];
        }
        else if(l1.second[1] != l2.second[1]){
            return l1.second[1] > l2.second[1];
        }
        else if(l1.second[2] != l2.second[2]){
            return l1.second[2] < l2.second[2];
        }
        else{
            return l1.first > l2.first; 
        }
    }
};

struct RootComparator{
    bool operator () (const vector<unsigned long long> &r1,
    const vector<unsigned long long> &r2) const{
        if(r1.size() != r2.size()){
            return r1.size() < r2.size();
        }
        else{
            return r1 < r2; 
        }
    }
};

struct TupleComparator{
    bool operator () (const tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>> &r1,
    const tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>> &r2) const{
        return get<0>(r1) > get<0>(r2); 
    }
};

struct TupleRemoveComparator{
    bool operator () (const tuple<double,unsigned,unordered_set<unsigned long long>,unsigned long long> &r1,
    const tuple<double,unsigned,unordered_set<unsigned long long>,unsigned long long> &r2) const{
        if(get<0>(r1) != get<0>(r2)){
            return get<0>(r1) > get<0>(r2); 
        }
        else{
            return get<3>(r1) > get<3>(r2); 
        }
    }
};

class sop {
    //Cubos da SOP
    unordered_map<long long unsigned,vector<unsigned long long>> cubes;
    //Mintermos cobertos pela SOP
    unordered_map<long long unsigned,vector<unsigned long long>> minterms;
    //Número de entradas
    unsigned nInput;
    unsigned nOutput;

public:
    sop();
    sop(vector<long long unsigned>& cubes_vector, unsigned inputs, unsigned outputs);
    sop(const sop& orig);
    sop& operator=(const sop& other);
    ~sop();
    unordered_map<long long unsigned,vector<unsigned long long>>* getCubes();
    unordered_map<long long unsigned,vector<unsigned long long>>* getMinterms();
    unsigned getnInput();
    unsigned getnOutput();
    bool containsMinterm(long long unsigned minterm);
    vector<long long unsigned> getCubeMinterms(long long unsigned cube);
    vector<long long unsigned> getMintermsDiff(long long unsigned cube);
    double errorRate(vector<long long unsigned>& min);
    vector<long long unsigned> getMintermsIntersec(long long unsigned cube);
    void printSop();
    void printPLAMin(string out_path);
    void printPLACubes(string out_path);
    void printPLACubesDC(string out_path, unordered_set<unsigned long long> EIC);
    vector<unsigned long long> removeCube(unsigned long long cube);
    void insertCubes(vector<unsigned long long> cube);
    void removeCOM(unsigned long long min, unsigned long long cube);
    bool verifySop();
    bool getEIC(long long unsigned cube, vector<unsigned long long>& EIC, vector<unsigned long long>& minterms, unsigned noe);
    void getCubeMinterms(long long unsigned cube, vector<unsigned long long>& minterms);
    vector<unsigned long long> removableCubes(vector<unsigned long long>& minterms);
    unordered_set<unsigned long long> cubeError2(unsigned long long cube, unordered_set<unsigned long long>& EIC);
    vector<unsigned long long> ALSRemove2(int noe, unordered_set<unsigned long long>& EIC);
    unordered_set<long long unsigned> expandCubes();
    vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> combAndEstimateBest(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees, unsigned max_error, vector<unsigned long long>& lastInserted, vector<unsigned long long>& lastRemoved, unordered_set<unsigned long long>& last_EIC);
    void generateExpandSCT(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees, unsigned noe);
    tuple<int,vector<unsigned long long>,vector<unsigned long long>> estimateLiteralReductionLast(vector<pair<long long unsigned,vector<unsigned>>>& leaves, vector<unsigned long long>& lastInserted, vector<unsigned long long>& lastRemoved);
    vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> fixLastEICs(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees, vector<unsigned long long>& lastInserted, vector<unsigned long long>& lastRemoved, unordered_set<unsigned long long>& EIC);
    unordered_map<unsigned long long,unsigned long long> getMagnitude();
    unsigned getSopLiteralCount();
private:
    void fillMinterms();
};

sop readPLA(string path);
string wordSelector(string line, int word_index);
void printLit(long long unsigned cube, unsigned nInput, unsigned nOutput);
void printBit(long long unsigned value, unsigned nInput, unsigned nOutput);
vector<long long unsigned> getUnion(vector<long long unsigned>& v1, vector<long long unsigned>& v2);
vector<pair<unsigned long long, vector<unsigned>>> getUnion(const vector<pair<unsigned long long, vector<unsigned>>>& v1,const vector<pair<unsigned long long, vector<unsigned>>>& v2);
vector<long long unsigned> getIntersection(const vector<long long unsigned>& v1, const vector<long long unsigned>& v2);
vector<pair<unsigned long long, vector<unsigned>>> getIntersection(const vector<pair<unsigned long long, vector<unsigned>>>& v1, const vector<pair<unsigned long long, vector<unsigned>>>& v2);
unsigned getLiteralCount(long long unsigned cube, unsigned nInput, unsigned nOutput);
unsigned getLiteralSum(vector<long long unsigned>& cubes, unsigned nInput, unsigned nOutput);
bool containsIntersection(vector<long long unsigned>* v1, vector<long long unsigned>* v2);
unsigned getPLALiteralCount(string path_in);
pair<double, unsigned> calcErrorRate(sop& s1, sop& s2);
pair<double, unsigned> getER(string path1, string path2);
bool containsAll(unordered_set<unsigned long long>& c1, vector<unsigned long long>& c2);
bool containsAll(unordered_set<unsigned long long>& c1, unordered_set<unsigned long long>& c2);
bool wasInserted(vector<unsigned long long>& cubesToInsert, unsigned long long cube);
bool compareSop(sop &s1, sop &s2);
unsigned writeModifiedSOP(sop &s, tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>> bt, string path);
void writeModifiedSOPDC(sop s, tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>> bt, string path);
bool contains(vector<unsigned long long> v, unsigned long long c);
void augmentSCT3(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees);
tuple<unsigned long long, double, double, double, unsigned long long, double> calcErrorMagnitude(unordered_map<unsigned long long,unsigned long long>& s1_output,unordered_map<unsigned long long,unsigned long long>& s2_output, unsigned nOutput);
tuple<unsigned long long, double, unsigned long long> getMagnitudeError(unsigned long long o1, unsigned long long o2, unsigned nOutput);
#endif

