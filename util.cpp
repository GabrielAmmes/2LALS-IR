#include "util.h"

using namespace std;

/*
--------------------------------------------------------------------
Entrada: path->Caminho para um arquivo .pla.
Função: Inicializa uma sop para cada saída do arquivo .pla.
Saída: Vetor com uma sop para cada saída.
*/
sop readPLA(string path){
    ifstream file;
    string line;
    unsigned inputs = 0, outputs = 0;
    long long unsigned base = 3, one = 1;
    file.open(path.c_str(),ifstream::binary);
    if(file.fail()){
        cout << "File not found" << endl;
        return sop();
    }
    file.seekg(file.beg);
    getline(file,line);
    while(line.size()==0){
        getline(file,line);
    }
    while(line.front() == '.' || line.front() == '#'){
        if(wordSelector(line,1) == ".i"){
            inputs = stoi(wordSelector(line,2));
        }
        else if(wordSelector(line,1) == ".o"){
            outputs = stoi(wordSelector(line,2));
        }
        getline(file,line);
        while(line.size()==0){
            getline(file,line);
        }
    }
    vector<long long unsigned> cubes;
    while(!file.eof() && line.front()!='.'){
        string in_string = line.substr(0,inputs);
        string out_string = line.substr(inputs+1,outputs);
        long long unsigned out_value = stoull(out_string);
        long long unsigned cube = 0;
        for(unsigned i = 0; i < outputs; i++){
            if(out_string.at(i) == '1'){
                cube = (one<<((2*inputs)+outputs-i-1))|cube;
            }
            else if(out_string.at(i) != '0'){
                cout << "Error in readPLA" << endl; 
            }
        }
        for(unsigned i = 0; i < inputs; i++){
            if(in_string.at(i) == '0'){
                cube = ((base & 2)<<(2*(inputs - i - 1)))|cube;
            }
            else if(in_string.at(i) == '1'){
                cube = ((base & 1)<<(2*(inputs - i - 1)))|cube;
            }
            else{
                cube = (base<<(2*(inputs - i - 1)))|cube;
            }
        }
        cubes.push_back(cube);
        getline(file,line);
        while(line.size()==0){
            getline(file,line);
        }
    }
    file.close();

    sop ret(cubes,inputs,outputs);

    return ret;
}

/*
--------------------------------------------------------------------
Entrada: line-> string base com palavra separadas por espaços,
         word_index-> index da palavra desejada (primeira é 1) 
Função: Seleciona a palavra na posição word_index da string line
Saída: Palavra na posição word_index.
*/
string wordSelector(string line, int word_index){
    int i=1;
    line.erase(0,line.find_first_not_of(" "));
    while(i<word_index)
    {
        line.erase(0,line.find_first_not_of(' '));
        line.erase(0,line.find_first_of(' ')+1);
        line.erase(0,line.find_first_not_of(' '));
        i++;
    }
    if(line.find(' ')!=string::npos)
        line.erase(line.find_first_of(' '),line.size());    
    return line;
}

/*
--------------------------------------------------------------------
Entrada: cube-> um cubo, nInput-> número de entradas
Função: Imprime o cubo de uma função de nInput variáveis, cada
literal pode ser direto xi, negado !xi ou não aparecer no cubo. 
Saída:
*/
void printLit(long long unsigned cube, unsigned nInput, unsigned nOutput){
    long long unsigned base = 3, v = 0, one = 1;
    string s = "";
        for(unsigned i = 0; i < nInput; i++){
        v = ((cube>>(2*i))&base);
        if(v == 1){//lit direto
            s = "x" +  to_string(nInput - i - 1) + s;
        }
        else if(v==2){
            s = "!x" + to_string(nInput - i - 1) + s;
        }
        else if(v!=3){
            cout << "ERROR EM printLit" << endl;
        }
    }
    for(unsigned i = 0; i < nOutput; i++){
        v = ((cube>>(i+2*nInput))&one);
        if(v == 1){//lit direto
            s = "y" +  to_string(nOutput - i - 1) + s;
        }
    }
    cout << s;
}

/*
--------------------------------------------------------------------
Entrada: cube-> um cubo, nInput-> número de entradas
Função: Imprime o cubo de uma função de nInput variáveis como é
armazenado (bit a bit).
Saída:
*/
void printBit(long long unsigned value, unsigned nInput, unsigned nOutput){
    unsigned nBits = 2*nInput + nOutput;
    long long unsigned one = 1, v = 0;
    for(unsigned i = 0; i < nBits; i++){
        v = (value & (one << (nBits - i - 1))) >> (nBits - i - 1);
        cout << v;
    }
}

/*
--------------------------------------------------------------------
Entrada: v1, v2-> vetores de cubos
Função: Obtem a união de dois vetores, ou seja, junta dois vetores
evitando repetições.
Saída: Vetor resultante da união de v1 e v2.
*/
vector<long long unsigned> getUnion(vector<long long unsigned>& v1, vector<long long unsigned>& v2){
    vector<long long unsigned> res(v1.size()+v2.size());
    vector<long long unsigned>::iterator it;
    it = set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),res.begin());
    res.resize(it-res.begin());
    return res;
}

vector<pair<unsigned long long, vector<unsigned>>> 
getUnion(const vector<pair<unsigned long long, vector<unsigned>>>& v1, 
                const vector<pair<unsigned long long, vector<unsigned>>>& v2){
    vector<pair<unsigned long long, vector<unsigned>>> res(v1.size()+v2.size());
    vector<pair<unsigned long long, vector<unsigned>>>::iterator it;
    it = set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),res.begin(),LeafComparator());
    res.resize(it-res.begin());
    return res;
}

/*
--------------------------------------------------------------------
Entrada: v1, v2-> vetores de cubos
Função: Obtem a itersecção de dois vetores, ou seja, os cubos que
estão em ambos os vetores.
Saída: Vetor resultante da intersecção de v1 e v2.
*/
vector<long long unsigned> getIntersection(const vector<long long unsigned>& v1, const vector<long long unsigned>& v2){
    vector<long long unsigned> res(v1.size()+v2.size());
    vector<long long unsigned>::iterator it;
    it = set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),res.begin());
    res.resize(it-res.begin());
    return res;
}

vector<pair<unsigned long long, vector<unsigned>>> 
getIntersection(const vector<pair<unsigned long long, vector<unsigned>>>& v1, 
                const vector<pair<unsigned long long, vector<unsigned>>>& v2){
    vector<pair<unsigned long long, vector<unsigned>>> res(v1.size()+v2.size());
    vector<pair<unsigned long long, vector<unsigned>>>::iterator it;
    it = set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),res.begin(),LeafComparator());
    res.resize(it-res.begin());
    return res;
}

/*
--------------------------------------------------------------------
Entrada: cube-> um cubo, nInput-> número de entradas
Função: Conta quantos literais tem o cubo.
Saída: Número de literais do cubo.
*/
unsigned getLiteralCount(long long unsigned cube, unsigned nInput, unsigned nOutput){
    long long unsigned base = 3, one = 1;
    unsigned ret=0;
    for(unsigned i = 0; i < nInput; i++){
        if(((cube>>(2*i))&base)!=base){
            ret++;
        }
    }
    for(unsigned i = 0; i < nOutput; i++){
        if(((cube>>(2*nInput + i))&one)==one){
            ret++;
        }
    }
    return ret;
}

/*
--------------------------------------------------------------------
Entrada: path_in->Caminho para um arquivo .PLA
Função: Conta quantos literais tem em um PLA.
Saída: Número de literais do PLA.
*/
unsigned getPLALiteralCount(string path_in){
    unsigned ret = 0;
    ifstream file;
    string line;
    unsigned inputs, outputs;
    file.open(path_in.c_str(),ifstream::binary);
    if(file.fail()){
        cout << "File not found" << endl;
        return 0;
    }
    file.seekg(file.beg);
    getline(file,line);
    while(line.size()==0){
        getline(file,line);
    }
    while(line.front() == '.' || line.front() == '#'){
        if(wordSelector(line,1) == ".i"){
            inputs = stoi(wordSelector(line,2));
        }
        else if(wordSelector(line,1) == ".o"){
            outputs = stoi(wordSelector(line,2));
        }
        getline(file,line);
        while(line.size()==0){
            getline(file,line);
        }
    }
    while(!file.eof() && line.front()!='.'){
        string in_string = line.substr(0,inputs);
        string out_string = line.substr(inputs+1,outputs);
        for(unsigned i = 0; i < inputs; i++){
            if(in_string.at(i) == '0' || in_string.at(i) == '1'){
                ret++;
            }
        }
        for(unsigned i = 0; i < outputs; i++){
            if(out_string.at(i) == '1'){
                ret++;
            }
        }
        getline(file,line);
        while(line.size()==0){
            getline(file,line);
        }
    }
    file.close();
    return ret;
}

pair<double, unsigned> getER(string path1, string path2){
    sop s1 = readPLA(path1);
    sop s2 = readPLA(path2);
    return calcErrorRate(s1,s2);
}

pair<double, unsigned> calcErrorRate(sop& s1, sop& s2){
    if(s1.getnInput() != s2.getnInput() || s1.getnOutput() != s2.getnOutput()){
        cout << "Both circuits must have the same input and output values" << endl;
        return make_pair(0.0,0);
    }
    unsigned long long one = 1;
    unsigned long long inputMask = (one<<(2*s1.getnInput()))-1;
    unordered_set<unsigned long long> min_errors;
    for(pair<unsigned long long, vector<unsigned long long>> min : *s1.getMinterms()){
        if(!s2.containsMinterm(min.first)){
            min_errors.insert(min.first&inputMask);
        }
    }
    for(pair<unsigned long long, vector<unsigned long long>> min : *s2.getMinterms()){
        if(!s1.containsMinterm(min.first)){
            min_errors.insert(min.first&inputMask);
        }
    }
    double er = min_errors.size()/pow(2,s1.getnInput());
    return make_pair(er,min_errors.size());
}

unsigned getLiteralSum(vector<long long unsigned>& cubes, unsigned nInput, unsigned nOutput){
    unsigned sum = 0;
    for(unsigned long long cube : cubes){
        sum = sum + getLiteralCount(cube,nInput,nOutput);
    }
    return sum;
}

bool containsAll(unordered_set<unsigned long long>& c1, vector<unsigned long long>& c2){
    for(unsigned long long value : c2){
        if(c1.count(value) == 0){
            return false;
        }
    }
    return true;
}

bool containsAll(unordered_set<unsigned long long>& c1, unordered_set<unsigned long long>& c2){
    for(unsigned long long value : c2){
        if(c1.count(value) == 0){
            return false;
        }
    }
    return true;
}

bool wasInserted(vector<unsigned long long>& cubesToInsert, unsigned long long cube){
    unsigned i;
    for(i = 0; i < cubesToInsert.size(); i++){
        if(cubesToInsert[i] == cube){
            break;
        }
    }
    if(i<cubesToInsert.size()){
        cubesToInsert.erase(cubesToInsert.begin()+i);
        return true;
    }
    return false;
}

bool compareSop(sop &s1, sop &s2){
    bool ret = true;
    auto s1_cubes = *s1.getCubes();
    auto s2_cubes = *s2.getCubes();
    auto s1_minterms = *s1.getMinterms();
    auto s2_minterms = *s2.getMinterms();
    for(auto cube : s1_cubes){
        if(s2_cubes.count(cube.first) > 0){
            for(unsigned long long COM : cube.second){
                if(!contains(s2_cubes[cube.first],COM)){
                    cout << "COM de um cubo de s1 não é igual em s2" << endl;
                    ret = false;
                }
            }
        }
        else{
            cout << "Cubo de s1 não está em s2" << endl;
            ret = false;
        }
    }
    for(auto cube : s2_cubes){
        if(s1_cubes.count(cube.first) > 0){
            for(unsigned long long COM : cube.second){
                if(!contains(s1_cubes[cube.first],COM)){
                    cout << "COM de um cubo de s2 não é igual em s1" << endl;
                    ret = false;
                }
            }
        }
        else{
            cout << "Cubo de s2 não está em s1" << endl;
            ret = false;
        }
    }
    for(auto minterm : s1_minterms){
        if(s2_minterms.count(minterm.first) > 0){
            for(unsigned long long cube : minterm.second){
                if(!contains(s2_minterms[minterm.first],cube)){
                    cout << "Cobertura de um mintermo de s1 não é igual em s2" << endl;
                    ret = false;
                }
            }
        }
        else{
            cout << "Mintermo de s1 não está em s2" << endl;
            ret = false;
        }
    }
    for(auto minterm : s2_minterms){
        if(s1_minterms.count(minterm.first) > 0){
            for(unsigned long long cube : minterm.second){
                if(!contains(s1_minterms[minterm.first],cube)){ 
                    cout << "Cobertura de um mintermo de s2 não é igual em s1" << endl;
                    ret = false;
                }
            }
        }
        else{
            cout << "Mintermo de s2 não está em s1" << endl;
            ret = false;
        }
    }
    return ret;
}

bool contains(vector<unsigned long long> v, unsigned long long c){
    for(unsigned long long vv : v){
        if(vv == c){
            return true;
        }
    }
    return false;
}

unsigned writeModifiedSOP(sop &s, tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>> bt, string path){
    sop orig = s;
    for(unsigned long long cube : get<2>(bt)){
        s.removeCube(cube);
    }
    s.insertCubes(get<1>(bt));
    s.printPLACubes(path);
    int lit = s.getSopLiteralCount();
    for(unsigned long long cube : get<1>(bt)){
        s.removeCube(cube);
    }
    s.insertCubes(get<2>(bt));
    return lit;
}

void augmentSCT3(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees){
    for(auto& t1 : trees){
        for(auto& t2 : trees){
            unsigned t1_size = t1.first.size(), t2_size = t2.first.size();
            if(t1_size > t2_size){
                vector<unsigned long long> inters = getIntersection(t1.first,t2.first);
                if(inters.size() == t2_size){
                    t1.second.insert(t1.second.end(),t2.second.begin(),t2.second.end());
                }
            }
            else{
                break;
            }
        }
    }
}


tuple<unsigned long long, double, double, double, unsigned long long, double> calcErrorMagnitude(unordered_map<unsigned long long,unsigned long long>& s1_output,unordered_map<unsigned long long,unsigned long long>& s2_output, unsigned nOutput){
    vector<unsigned long long> errors;
    vector<double> errors_relative;
    unsigned long long WCE = 0, WBF = 0, one = 1;
    double MAE = 0.0, MRE = 0.0, MSE = 0.0, WRE = 0.0;
    for(pair<unsigned long long, unsigned long long> s1_out : s1_output){
        unsigned long long o2=0;
        if(s2_output.count(s1_out.first)>0){
            o2 = s2_output[s1_out.first];
        }
        if(o2 != s1_out.second){
            auto errors_tuple = getMagnitudeError(s1_out.second,o2,nOutput);
            errors.push_back(get<0>(errors_tuple));
            errors_relative.push_back(get<1>(errors_tuple));
            if(get<2>(errors_tuple) > WBF){
                WBF = get<2>(errors_tuple);            
            }
        }
    }
    for(pair<unsigned long long, unsigned long long> s2_out : s2_output){
        if(s1_output.count(s2_out.first)==0){
            auto errors_tuple = getMagnitudeError(0,s2_out.second,nOutput);
            errors.push_back(get<0>(errors_tuple));
            errors_relative.push_back(get<1>(errors_tuple));
            if(get<2>(errors_tuple) > WBF){
                WBF = get<2>(errors_tuple);           
            }
        }
    }
    for(unsigned long long error : errors){
        if(error > WCE){
            WCE = error;
        }
        MAE = MAE + error;
        MSE = MSE + (error*error);
    }
    for(double error : errors_relative){
        if(error > WRE){
            WRE = error;
        }
        MRE = MRE + error;
    }
    MAE = MAE/errors.size();
    MSE = MSE/errors.size();
    MRE = MRE/(double)errors.size();
    return make_tuple(WCE,MAE,WRE,MRE,WBF,MSE);
}


tuple<unsigned long long, double, unsigned long long> getMagnitudeError(unsigned long long o1, unsigned long long o2, unsigned nOutput){
    unsigned long long error = 0, bitFlip = 0, one = 1;
    if(o1 > o2){
        error = o1 - o2;
    }
    else{
        error = o2 - o1;
    }
    double error_relative = (double)((double)error)/((double)max(one,o1));
    unsigned long long bitDiff = o1 ^ o2;
    for (int j = 0; j < nOutput; j++){
        bitFlip = bitFlip + ((bitDiff>>j)&one);
    }
    return make_tuple(error,error_relative,bitFlip);
}