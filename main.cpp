    
#include "util.h"

using namespace std;

unsigned global_sct = 0, global_est = 0;

void printHelp(){
    cout << "Proposed two-level ALS method:" << endl <<
    "\t./main -t 0 -i PATH -o PATH -[en|er] ERROR" << endl <<
    "Literal Count:" << endl <<
    "\t./main -t 1 -i PATH" << endl << 
    "Cubes Count:" << endl << 
    "\t./main -t 2 -i PATH" << endl << 
    "Error Rate Count:" << endl << 
    "\t./main -t 3 -i PATH -o PATH" << endl << endl <<
    "-i - Input Path" << endl << 
    "-o - Output Path/ Second Input" << endl <<
    "-er - Error Rate - " << 
    "Max percentual of erroneous input combination allowed. [UNS]" << endl << 
    "-en - Number of Errors - " << 
    "Max number of erroneous input combination allowed. [UNS]" << endl << 
    "OTHER COMMANDS" << endl << 
    "-h - Help" << endl << endl <<
    "Example: " << endl <<
    "./main -t 0 -i data/iwls/sao2.pla -o app/ -er 5" << endl <<
    "Approximate the sao2.pla benchmark with 5% ER threshold and stores in app folder." << endl;
}

auto getCMD(int argc, char** argv){
    string in = "", out = "", name="";
    unsigned noType = 4;
    unsigned er=0, en=0, type=noType, write = 2;
    unsigned i = 1, gh=0, ge=0, ex=0;
    while(i < argc){
        string s(argv[i]);
        if(s == "-t"){
            i++;
            if(i>=argc){
                cout << "Must have a value after -t" << endl;
                type = noType;
                break;    
            }
            type = stoi(argv[i]);
            if(type >= noType){
                type = noType;
            }
        }
        else if(s == "-i"){
            i++;
            if(i>=argc){
                cout << "Must have a path after -i" << endl;
                type = noType;
                break;    
            }
            in = argv[i];
        }
        else if(s == "-o"){
            i++;
            if(i>=argc){
                cout << "Must have a path after -o" << endl;
                type = noType;
                break;    
            }
            out = argv[i];
        }
        else if(s == "-er"){
            i++;
            if(i>=argc){
                cout << "Must have a value after -er" << endl;
                type = noType;
                break;    
            }
            er = stoi(argv[i]);
        }
        else if(s == "-en"){
            i++;
            if(i>=argc){
                cout << "Must have a value after -en" << endl;
                type = noType;
                break;    
            }
            en = stoi(argv[i]);
        }
        else if(s == "-h"){
            type = noType;
            break;
        }
        else{
            cout << "invalid option " << s << endl;
            type = noType;
            break;

        }
        i++;
    }
    if(in == "" && type!=noType){
        cout << "Must have an input path" << endl;
        type = noType;
    }
    if(type == 0){
        name = in;
        if(name.find("/")){
            name.erase(0,name.find_last_of('/')+1);
        }
        if(name.find(".")){
            name.erase(name.find_last_of('.'),name.size());
        }
        if(er>0 && en>0){
            cout << "EN value used" << endl;
            er=0;
        }
        if(er==0 && en==0){
            cout << "Must have an error value > 0" << endl;
            type = noType;
        }
    }
    if(type == 3 && out==""){
        cout << "Must have an input/output path" << endl;
        type = noType;
    }
    if(type == noType){
        printHelp();
    }
    return make_tuple(type,in,out,er,en,name);
}


int main(int argc, char **argv)
{
    auto start = chrono::steady_clock::now();
    string in, out, name;
    double percentual = 0.0;
    unsigned type, en, er, gh=2;
    unsigned long long one = 1, cube;
    tie(type,in,out,er,en,name) = getCMD(argc,argv);
    if(type == 4){
        return 0;
    }
    unsigned noe = 0, limit = gh;
    sop s = readPLA(in);
    if(s.getnInput() == 0){
         return 0;
    }
    string errorString;
    if(er > 0){
        noe = er*(1<<s.getnInput())/100;
        percentual = er;
        errorString = to_string(er);
    }
    else{
        noe = en;
        percentual = en*100;
        percentual = percentual/(1<<s.getnInput());
        errorString = to_string(en);
    }
    if(type == 1){
        cout << getPLALiteralCount(in) << "\t";
        return 0;
    }
    else if(type == 2){
        sop s1 = readPLA(in);
        cout << s1.getCubes()->size() << "\t";
        return 0;
    }
    else if(type == 3){
        sop s1 = readPLA(in);
        sop s2 = readPLA(out);
        auto cer = calcErrorRate(s,s2);
        cout << cer.first << " " << cer.second << endl;
        return 0;
    }
    unsigned originalLiteral = s.getSopLiteralCount();
    cout << "Circuit: " << name << endl;
    cout << "Number of allowed errors: " << noe << endl;
    cout << "Percentage of allowed errors: " << percentual << endl;
    cout << "Original Circuit Literal Count: " << originalLiteral << endl;
    vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> best(noe+1), best_rem(noe+1), ret(3);
    vector<vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>>> solutions(noe+1), solutions_rem(noe+1);
    tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>> ret_rem;
    unsigned nInput = s.getnInput(), nOutput = s.getnOutput();
    unsigned exec_count = 1;
    int best_i = -1, best_j = -1;
    bool sat = false;
    unsigned sat_id = noe+1;
    solutions[0].push_back(best[0]);
    for(unsigned i = 0; i < noe; i++){
        if(gh!=0 ){
            sort(solutions[i].begin(),solutions[i].end(),TupleComparator());
            if(solutions[i].size()>gh){
                limit = gh;
            }
            else{
                limit = solutions[i].size();
            }
        }
        else{
            limit = solutions[i].size();
        }
        if(limit == 0 && solutions[i+1].size()==0){
            sat_id = i-1;
           // cout << "sat " << sat_id << endl;
            break;
        }
        unsigned j = 0;
        vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> actual_tuples;
        if(limit>0){
            actual_tuples.push_back(solutions[i][0]);
        }
        if(limit == 2){
            actual_tuples.push_back(solutions[i][1]);
        }
        while(j<limit){
            for(unsigned long long cube : get<2>(actual_tuples[j])){
                s.removeCube(cube);
            }
            s.insertCubes(get<1>(actual_tuples[j]));
            unsigned error = 2;
            if(i==(noe-1)){
                error = 1;
            }
            map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator> trees;
            s.generateExpandSCT(trees,error);
            augmentSCT3(trees);
            auto zeroRoot = s.fixLastEICs(trees, get<1>(actual_tuples[j]),get<2>(actual_tuples[j]), get<3>(actual_tuples[j]));
            if(get<0>(zeroRoot[0])>0){
                for(unsigned long long cube : get<1>(actual_tuples[j])){
                    s.removeCube(cube);
                }
                s.insertCubes(get<2>(actual_tuples[j]));
                if(j == 0){
                    if(get<0>(zeroRoot[1])>0){
                        actual_tuples = zeroRoot;
                        solutions[i][1] = actual_tuples[1];
                    }
                    else{
                        actual_tuples[j] = zeroRoot[0];
                    }  
                }
                else{
                    actual_tuples[j] = zeroRoot[0];
                }
                solutions[i][j] = actual_tuples[j];
            }
            else{
                for(auto& sct_pair : trees){
                    sort(sct_pair.second.begin(),sct_pair.second.end(),LeafComparator());
                }
                ret = s.combAndEstimateBest(trees,error,get<1>(actual_tuples[j]),get<2>(actual_tuples[j]),get<3>(actual_tuples[j]));
                if(get<0>(ret[1]) > 0){
                    solutions[i+1].push_back(ret[1]);
                }
                if(error == 2){
                    if(get<0>(ret[2]) > 0){
                        solutions[i+2].push_back(ret[2]);
                    }
                }
                for(unsigned long long cube : get<1>(actual_tuples[j])){
                    s.removeCube(cube);
                }
                s.insertCubes(get<2>(actual_tuples[j]));
                j++;
            }
        }
    }
    for(unsigned i = 0; i < sat_id; i++){
        for(unsigned j = 0; j < solutions[i].size(); j++){
            auto bestTuple = solutions[i][j];
            if(get<0>(bestTuple) > get<0>(best[i])){
                best[i] = bestTuple;
            }
        }
    }
    auto bestResult = best[0];
    auto bestResult_rem = best[0];
    for(unsigned i = 0; i < sat_id; i++){
        auto bestTuple = best[i];
        if(get<0>(bestTuple) > get<0>(bestResult)){
            bestResult = bestTuple;
        }
    }
    auto end_insert = chrono::steady_clock::now();
    chrono::duration<double> time_insert = end_insert - start;
    auto start_remove = chrono::steady_clock::now();
    vector<unsigned> removePositions(noe+1);
    if(sat_id == noe+1){
        limit = noe;
    }
    else{
        limit = sat_id;
    }
    for(unsigned i = 1; i < limit; i++){
        for(unsigned j = 0; j < solutions[i].size(); j++){
            for(unsigned long long cube : get<2>(solutions[i][j])){
                s.removeCube(cube);
            }
            s.insertCubes(get<1>(solutions[i][j]));
            ret_rem = solutions[i][j];
            auto cubesToRemove = s.ALSRemove2(noe-i,get<3>(ret_rem));
            sort(cubesToRemove.begin(), cubesToRemove.end());
            vector<unsigned long long> cubesTR;
            for(unsigned long long ctr : cubesToRemove){
                if(!wasInserted(get<1>(ret_rem),ctr)){
                    cubesTR.push_back(ctr);
                }
            }
            cubesToRemove = getUnion(cubesTR,get<2>(ret_rem));
            get<2>(ret_rem) = cubesToRemove;
            get<0>(ret_rem) = getLiteralSum(get<2>(ret_rem),s.getnInput(),s.getnOutput()) - getLiteralSum(get<1>(ret_rem),s.getnInput(),s.getnOutput());
            if(get<0>(ret_rem) > get<0>(best_rem[i])){
                removePositions[i] = j;
                best_rem[i] = ret_rem;
            }
            for(unsigned long long cube : get<1>(ret_rem)){
                s.removeCube(cube);
            }
            s.insertCubes(get<2>(ret_rem));
        }
    }
    vector<unsigned long long> CI;
    unordered_set<unsigned long long> EIC;
    auto cubesToRemove = s.ALSRemove2(noe,EIC);
    int redLit = getLiteralSum(cubesToRemove,s.getnInput(),s.getnOutput());
    best_rem[0] = make_tuple(redLit,CI,cubesToRemove,EIC);
    s.insertCubes(cubesToRemove);
    bestResult_rem = best_rem[0];
    for(unsigned i = 0; i < sat_id; i++){
        if(get<0>(best_rem[i]) > get<0>(bestResult_rem)){
            bestResult_rem = best_rem[i];
            best_i = i;
            best_j = removePositions[i];
        }
    }
    string basePath = out + name;
    string ipath = basePath + "_1.pla";
    vector<unsigned> literals(4);
    vector<string> names = {"_1.pla","_2.pla","_1_esp.pla","_2_esp.pla"};
    literals[0] = writeModifiedSOP(s,bestResult,ipath);
    ipath = basePath + "_2.pla";
    literals[1] = writeModifiedSOP(s,bestResult_rem,ipath);
    string command = "./espresso/espresso " + basePath + names[0] +" > " + basePath + names[2];
    system(command.c_str());
    ipath = basePath + "_1_esp.pla";
    literals[2] = getPLALiteralCount(ipath);
    command = "./espresso/espresso " + basePath + names[1] +" > " + basePath + names[3];
    system(command.c_str());
    ipath = basePath + "_2_esp.pla";
    literals[3] = getPLALiteralCount(ipath);
    unsigned best_lit = literals[0], best_id = 0;
    for(unsigned i = 1; i < 4; i++){
        if(literals[i] < best_lit){
            best_lit = literals[i];
            best_id = i;
        }
    }
    double ratio = (double)best_lit/originalLiteral;
    cout << "Approximated Circuit Literal Count: " << best_lit << endl;
    cout << "Ratio Approximated Literals/Original Literals: " << ratio << endl;
    for(unsigned i = 0; i < 4; i++){
        if(i == best_id){
            command = "mv " + basePath + names[i] + " " + basePath + "_" + errorString + "_app.pla";
            system(command.c_str());
        }
        else{
            command = "rm " + basePath + names[i];
            system(command.c_str());
        }
    }
    auto end = chrono::steady_clock::now();
    chrono::duration<double> time = end - start;
    cout << "Run-time: " << time.count() << " seconds" << endl << endl;
}
