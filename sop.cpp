#include "util.h"

using namespace std;

/*
--------------------------------------------------------------------
Entrada:
Função: Construtor vazio da classe.
Saída:
*/
sop::sop(){
    this->nInput = 0;
    this->nOutput = 0;
}


/*
--------------------------------------------------------------------
Entrada: cubes->Vetor de cubos da sop, inputs->Número de entrada.
Função: Construtor da classe.
Saída:
*/
sop::sop(vector<long long unsigned>& cubes_vector, unsigned inputs, unsigned outputs) 
: nInput(inputs), nOutput(outputs){
    unordered_map<long long unsigned,vector<unsigned long long>> cubes_map;
    for(unsigned long long cube : cubes_vector){
        vector<unsigned long long> COMs;
        cubes_map[cube] = COMs;
    }
    this->cubes=cubes_map;
    fillMinterms();
}


/*
--------------------------------------------------------------------
Entrada: other->outra sop.
Função: Construtor de cópia.
Saída:
*/
sop::sop(const sop& other)
 : cubes(other.cubes), minterms(other.minterms), nInput(other.nInput), nOutput(other.nOutput)
{
}

/*
--------------------------------------------------------------------
Entrada: other->outra sop.
Função: Overload do operador de assinalamento
Saída:
*/
sop& sop::operator=(const sop& other){
   if(this!=&other){
        this->cubes = other.cubes;
        this->minterms = other.minterms;
        this->nInput = other.nInput;
        this->nOutput = other.nOutput;
    }
    return *this;
}

/*
--------------------------------------------------------------------
Entrada:
Função: Destrutor da classe.
Saída:
*/
sop::~sop(){
}


/*
--------------------------------------------------------------------
Entrada: cube->Um cubo.
Função: Obtem os mintermos cobertos por um cubo.
Saída: Vetor de mintermos cobertos pelo cubo.
*/
vector<long long unsigned> sop::getCubeMinterms(long long unsigned cube){
    long long unsigned base = 3, nDC, one = 1;
    vector<unsigned> pDC;
    vector<long long unsigned> ret, minterms;
    for(unsigned i = 0; i<this->nInput;i++){
        if(((cube>>(2*i))&base)==base){
            pDC.push_back(i);
        }
    }
    nDC = one << pDC.size();
    for(long long unsigned i = 0; i < nDC; i++){
        long long unsigned minterm = cube;
        for (unsigned j = 0; j < pDC.size(); j++){
            long long unsigned value = ((one << j) & i);
            if(value == 0){
                minterm = minterm ^ (one << 2*pDC[j]+1);
            }
            else{
                minterm = minterm ^ (one << 2*pDC[j]);
            }
        }
        minterms.push_back(minterm);
    }
    vector<unsigned long long> outMask;
    unsigned long long inputMask = (one<<2*this->nInput)-1;
    for(unsigned i = 0; i<this->nOutput;i++){
        if(((cube>>(i+2*this->nInput))&one)==one){
            unsigned long long om = inputMask | (one<<(i+2*this->nInput));
            outMask.push_back(om);

        }
    }
    if(outMask.size()==1){
        return minterms;
    }
    
    for(unsigned long long m : minterms){
        for(unsigned long long mask : outMask){
            ret.push_back(m&mask);
        }
    }
    return ret;
}

/*
--------------------------------------------------------------------
Entrada: 
Função: Preenche o map de mintermos considerando os cubos da sop.
Saída:
*/
void sop::fillMinterms(){
    unordered_map<long long unsigned,vector<unsigned long long>> mintermsMap;
    for(auto cube : this->cubes){
        for(long long unsigned minterm : getCubeMinterms(cube.first)){
            if(mintermsMap.count(minterm)==0){
                vector<unsigned long long> c;
                c.push_back(cube.first);
                mintermsMap[minterm] = c;
            }
            else{
                mintermsMap[minterm].push_back(cube.first);
            }
        }
    }
    for(auto min : mintermsMap){
        if(min.second.size() == 1){
            this->cubes[min.second[0]].push_back(min.first);
        }
    }
    this->minterms = mintermsMap;
}

/*
--------------------------------------------------------------------
Entrada: 
Função: Getter do Map de minterms.
Saída: Ponteiro para o map de minterms.
*/
unordered_map<long long unsigned,vector<unsigned long long>>* sop::getMinterms(){
    return &this->minterms;
}

/*
--------------------------------------------------------------------
Entrada: 
Função: Getter do vetor de cubos.
Saída: Ponteiro para o vetor de cubos.
*/
unordered_map<long long unsigned,vector<unsigned long long>>* sop::getCubes(){
    return &this->cubes;
}

/*
--------------------------------------------------------------------
Entrada: 
Função: Getter do número de entradas.
Saída: Número de entradas.
*/
unsigned sop::getnInput(){
    return this->nInput;
}

unsigned sop::getnOutput(){
    return this->nOutput;
}

/*
--------------------------------------------------------------------
Entrada: minterm->Um mintermo
Função: Verifica se a sop cobre o mintermo
Saída: Boolean referente a verificação.
*/
bool sop::containsMinterm(long long unsigned minterm){
    if(this->minterms.count(minterm) == 0){
        return false;
    }
    else{
        return true;
    }
}

/*
--------------------------------------------------------------------
Entrada: cube->Um cubo
Função: Obtem os mintermos que são cobertos pelo cubo e não são cibertos
pela SOP.
Saída: Vetor dos mintermos que são cobertos pelo cubo e não são cibertos
pela SOP.
*/
vector<long long unsigned> sop::getMintermsDiff(long long unsigned cube){
    vector<long long unsigned> covered, ret;
    covered = getCubeMinterms(cube);
    for(long long unsigned m : covered){
        if(!containsMinterm(m)){
            ret.push_back(m);
        }
    }
    return ret;
}

/*
--------------------------------------------------------------------
Entrada: cube->Um cubo
Função: Obtem os mintermos que são cobertos pelo cubo e pela SOP.
Saída: Vetor dos mintermos que são cobertos pelo cubo e pela SOP.
*/
vector<long long unsigned> sop::getMintermsIntersec(long long unsigned cube){
    vector<long long unsigned> covered, ret;
    covered = getCubeMinterms(cube);
    for(long long unsigned m : covered){
        if(containsMinterm(m)){
            ret.push_back(m);
        }
    }
    return ret;
}

/*
--------------------------------------------------------------------
Entrada: 
Função: Imprime os elementos da sop.
Saída: 
*/
void sop::printSop(){
    cout << "nInput " << this->nInput << endl;
    cout << "nOutput " << this->nOutput << endl;
    for(auto cube_pair : this->cubes){
        cout << "Cube ";
        printLit(cube_pair.first, this->nInput, this->nOutput);
        cout << endl;
        cout << "COMs ";
        for(unsigned long long c : cube_pair.second){
            printLit(c, this->nInput, this->nOutput);
            cout << " ";
        }
        cout << endl;
    }
    for(pair<long long unsigned,vector<unsigned long long>> minterm : this->minterms){
        cout << "Minterm ";
        printLit(minterm.first, this->nInput, this->nOutput);
        cout << endl;
        cout << "Covered ";
        for(unsigned long long m : minterm.second){
            printLit(m, this->nInput, this->nOutput);
            cout << " ";
        }
        cout << endl;
    }
}

/*
--------------------------------------------------------------------
Entrada: sop_vector-> vetor de sop para ser escrita no arquivo .pla.
path->Caminho para escrita do arquivo .pla.
Função: Imprime os mintermos das sops em um arquivo .pla.
Saída:
*/
void sop::printPLAMin(string out_path){
    ofstream write;
    write.open(out_path.c_str(),ios::out|ios::trunc);
    if(write.fail()){
        cout << "Output path not found." << endl;
        return;
    }
    long long unsigned one = 1, out = 0, base = 3;
    unsigned nInput = this->nInput;
    unsigned nOut = this->nOutput;
    write << ".i " << nInput << endl;
    write << ".o " << nOut << endl;
    
    for(pair<long long unsigned,vector<long long unsigned>> min : this->minterms){
        for(unsigned min_index = 0; min_index < nInput; min_index++){
            long long unsigned value = (min.first>>(2*(nInput - min_index -1)))&base;
            if(value == 1){
                write << "1";
            }
            else if(value == 2){
                write << "0";
            }
            else if(value == 3){
                write << "-";
            }
            else{
                cout << "ERROR in printPLA";
            }
        }
        write << " ";
        for(unsigned out_index = 0; out_index < nOut; out_index++){
            long long unsigned value = (min.first>>(nOut - out_index - 1 +(2*nInput)))&one;
            if(value == 1){
                write << "1";
            }
            else if(value == 0){
                write << "0";
            }
        }
        write << endl;
    }
    write << ".e";
    write.close();
}

/*
--------------------------------------------------------------------
Entrada: sop_vector-> vetor de sop para ser escrita no arquivo .pla.
path->Caminho para escrita do arquivo .pla.
Função: Imprime os mintermos das sops em um arquivo .pla.
Saída:
*/
void sop::printPLACubes(string out_path){
    ofstream write;
    write.open(out_path.c_str(),ios::out|ios::trunc);
    if(write.fail()){
        cout << "Output path not found."<< endl;
        return;
    }
    map<long long unsigned,long long unsigned> cube_output;
    long long unsigned one = 1, out = 0, base = 3;
    unsigned out_index=0, nInput = this->nInput;
    unsigned nOut = this->nOutput;
    write << ".i " << nInput << endl;
    write << ".o " << nOut << endl;
    for(auto cube_pair : this->cubes){
        unsigned long long cube = cube_pair.first;
        for(unsigned cube_index = 0; cube_index < nInput; cube_index++){
            long long unsigned value = (cube>>(2*(nInput - cube_index -1)))&base;
            if(value == 1){
                write << "1";
            }
            else if(value == 2){
                write << "0";
            }
            else if(value == 3){
                write << "-";
            }
            else{
                cout << "ERROR in printPLA";
            }
        }
        write << " ";
        for(out_index = 0; out_index < nOut; out_index++){
            long long unsigned value = (cube>>(nOut - out_index - 1 +(2*nInput)))&one;
            if(value == 1){
                write << "1";
            }
            else if(value == 0){
                write << "0";
            }
        }
        write << endl;
    }
    write << ".e";
    write.close();
}


vector<unsigned long long> sop::removeCube(unsigned long long cube){
    vector<unsigned long long> EM;
    if(this->cubes.count(cube)==0){
        cout << "Removendo cubo que não existe" << endl;
    }
    for(unsigned long long m : getCubeMinterms(cube)){
        if(this->minterms.count(m)>0){
            unsigned removeId = 0;
            for(unsigned long long c : this->minterms[m]){
                if(c == cube){
                    break;
                }
                removeId++;
            }
            this->minterms[m].erase(this->minterms[m].begin() + removeId);
            if(this->minterms[m].size() == 1){
                this->cubes[this->minterms[m][0]].push_back(m);//esse
            }
            if(this->minterms[m].size() == 0){
                EM.push_back(m);
                this->minterms.erase(m);
            }
        }
        else{
            cout << "não tinha o min" << endl;
        }
    }
    this->cubes.erase(cube);

    return EM; 
}

void sop::insertCubes(vector<unsigned long long> cubesToInsert){
    for(unsigned long long cube : cubesToInsert){
        if(this->cubes.count(cube)>0){
            cout << "cube ja existe no insert" << endl;
        }
        vector<unsigned long long> min;
        this->cubes[cube] = min;
        for(unsigned long long m : getCubeMinterms(cube)){
            if(this->minterms.count(m) > 0){
                if(this->minterms[m].size()==1){
                    removeCOM(m,this->minterms[m][0]);
                }
            }
            else{
                vector<unsigned long long> c;
                this->minterms[m] = c;
                this->cubes[cube].push_back(m);
            }
            this->minterms[m].push_back(cube);
        }
    }
}

void sop::removeCOM(unsigned long long min, unsigned long long cube){
    unsigned removeId = 0;
    if(this->cubes[cube].size() > 0){
        if(this->cubes[cube].size() == 1){
            cout << "Vai dar ruim no REMOVECOM" << endl;
        }
        for(unsigned long long m : this->cubes[cube]){
            if(m == min){
                break;
            }
            removeId++;
        }
        if(removeId == this->cubes[cube].size()){
            cout << "não tinha o min?" << endl;
        }
        this->cubes[cube].erase(this->cubes[cube].begin() + removeId);
    }
}

bool sop::verifySop(){
    bool ret = true;
    for(auto cube_pair : this->cubes){
        unsigned long long cube = cube_pair.first;
        if(this->cubes[cube].size() == 0){
            cout << "cubo sem mintermos essenciais" << endl;
            ret = false;
        }
        for(unsigned long long COM : this->cubes[cube]){
            if(this->minterms.count(COM) == 0){
                cout << "COM não esta nos mintermos" << endl;
                ret = false;
            }
            else{
                if(this->minterms[COM].size() == 0){
                    cout << "Mintermo não coberto" << endl;  
                    ret = false;     
                }
                else if(this->minterms[COM].size() > 1){
                    cout << "COM não é COM" << endl;     
                    ret = false;  
                }
                else if(this->minterms[COM][0] != cube){
                    cout << "COM é COM mas não é coberto pelo cube" << endl;
                    ret = false;
                }
            }
        }
    }
    for(auto minterm_pair : this->minterms){
        unsigned long long minterm = minterm_pair.first;
        if(this->minterms[minterm].size() == 0){
            cout << "Mintermo não coberto" << endl;  
            ret = false;
        }
        else{
            for(unsigned long long cube : this->minterms[minterm]){
                if(this->cubes.count(cube) == 0){
                    cout << "Cube não esta nos cubos" << endl;
                    ret = false;
                }
                if(this->minterms[minterm].size() == 1){
                    bool contains = false;
                    for(unsigned long long COM : this->cubes[cube]){
                        if(COM == minterm){
                            contains = true;
                        }
                    }
                    if(!contains){
                        cout << "Minterm é COM mas não está na lista do cubo" << endl;
                        ret = false;
                    }
                }
            }
        }

    }
    if(ret){
        cout << "SOP OK!" << endl;
    }
    return ret;
}

/*
--------------------------------------------------------------------
Entrada: cube->Um cubo.
Função: Obtem os mintermos cobertos por um cubo.
Saída: Vetor de mintermos cobertos pelo cubo.
*/
void sop::getCubeMinterms(long long unsigned cube, vector<long long unsigned>& minterms){
    long long unsigned base = 3, nDC, one = 1;
    vector<unsigned> pDC;
    for(unsigned i = 0; i<this->nInput;i++){
        if(((cube>>(2*i))&base)==base){
            pDC.push_back(i);
        }
    }
    nDC = one << pDC.size();

    vector<unsigned long long> outMask;
    unsigned long long inputMask = (one<<2*this->nInput)-1;
    for(unsigned i = 0; i<this->nOutput;i++){
        if(((cube>>(i+2*this->nInput))&one)==one){
            unsigned long long om = inputMask | (one<<(i+2*this->nInput));
            outMask.push_back(om);
        }
    }

    for(long long unsigned i = 0; i < nDC; i++){
        long long unsigned minterm = cube;
        for (unsigned j = 0; j < pDC.size(); j++){
            long long unsigned value = ((one << j) & i);
            if(value == 0){
                minterm = minterm ^ (one << 2*pDC[j]+1);
            }
            else{
                minterm = minterm ^ (one << 2*pDC[j]);
            }
        }
        for(unsigned long long mask : outMask){
            minterms.push_back(minterm&mask);
        }
    }
}

bool sop::getEIC(long long unsigned cube, vector<unsigned long long>& EIC, vector<unsigned long long>& minterms, unsigned noe){
    long long unsigned base = 3, nDC, one = 1;
    vector<unsigned> pDC;
    bool noMinterm = true;
    unordered_set<unsigned long long> min_set;

    for(unsigned i = 0; i<this->nInput;i++){
        if(((cube>>(2*i))&base)==base){
            pDC.push_back(i);
        }
    }
    nDC = one << pDC.size();

    vector<unsigned long long> outMask;
    unsigned long long mask_in = (one<<2*this->nInput)-1;
    for(unsigned i = 0; i<this->nOutput;i++){
        if(((cube>>(i+2*this->nInput))&one)==one){
            unsigned long long om = mask_in | (one<<(i+2*this->nInput));
            outMask.push_back(om);
        }
    }
    for(long long unsigned i = 0; i < nDC; i++){
        long long unsigned minterm = cube;
        for (unsigned j = 0; j < pDC.size(); j++){
            long long unsigned value = ((one << j) & i);
            if(value == 0){
                minterm = minterm ^ (one << 2*pDC[j]+1);
            }
            else{
                minterm = minterm ^ (one << 2*pDC[j]);
            }
        }
        for(unsigned long long mask : outMask){
            unsigned long long m = minterm&mask;
            minterms.push_back(m);
            if(this->minterms.count(m) == 0){
                min_set.insert(m&mask_in);
            }
            else{
                noMinterm = false;
            }
        }
        if(min_set.size()>noe){
            noMinterm = false;
            break;
        }
    }
    for(unsigned long long min : min_set){
        EIC.push_back(min);
    }
    return noMinterm;
}

tuple<int,vector<unsigned long long>,vector<unsigned long long>> sop::estimateLiteralReductionLast(vector<pair<long long unsigned,vector<unsigned>>>& leaves, vector<unsigned long long>& lastInserted, vector<unsigned long long>& lastRemoved){
    unordered_set<unsigned long long> leavesMinterms;
    unordered_set<unsigned long long> cubesCandidates;
    unordered_set<unsigned long long> COMsToCover;
    vector<vector<unsigned long long>> leavesCOMs(leaves.size());
    unordered_set<unsigned long long> EIC;
    vector<unsigned long long> cubesToRemove;
    unordered_map<unsigned long long,unsigned> COMsCount;
    unsigned long long one = 1;
    unsigned long long in_mask = (one<<this->nInput)-1;
    unsigned leaf_id=0;
    int redLit = 0;
    for(auto& leaf_pair : leaves){
        for(unsigned long long m : getCubeMinterms(leaf_pair.first)){
            if(this->minterms.count(m)>0){
                leavesMinterms.insert(m);
                leavesCOMs[leaf_id].push_back(m);
                for(unsigned long long cube : this->minterms[m]){
                    cubesCandidates.insert(cube);
                }
            }
        }
        leaf_id++;
    }
    for(unsigned long long cube : cubesCandidates){
        vector<unsigned long long> COMs = this->cubes[cube];
        if(containsAll(leavesMinterms, COMs)){
            removeCube(cube);
            cubesToRemove.push_back(cube);
            COMsToCover.insert(COMs.begin(), COMs.end());
        }
    }
    bool problem = false, hasProblem = false;
    vector<unsigned> toInsertId;
    for(unsigned i = 0; i<leavesCOMs.size(); i++){
        bool inserted = false;
        for(unsigned long long m : leavesCOMs[i]){
            if(COMsToCover.count(m)!=0){
                COMsToCover.erase(m);
                inserted = true;
            }
        }
        if(inserted){
            toInsertId.push_back(i);
            for(unsigned long long m : leavesCOMs[i]){
                if(COMsCount.count(m)==0){
                    if(this->minterms.count(m)>0){
                        COMsCount[m]=this->minterms[m].size();
                    }
                    else{
                        COMsCount[m]=0;
                    }
                }
                COMsCount[m]++;
            }
        }
        if(COMsToCover.size()==0){
            break;
        }
    }
    bool hasRedundant = true;
    unsigned toInsertIdSize = toInsertId.size();
    while(hasRedundant){
        int redundantCubeID = -1;
        int litRedundant = -1;
        unsigned red_id = 0, ti_id = 0;
        double best_metric = 0.0;
        for(unsigned i : toInsertId){
            bool redundant = true;
            double postCOM = 0.0;
            for(unsigned long long m : leavesCOMs[i]){
                if(COMsCount[m] == 0){
                    redundant = false;
                    break;
                }
                if(COMsCount[m] == 1){
                    redundant = false;
                    break;
                }
                if(COMsCount[m] == 2){
                    postCOM = postCOM + 1.0;
                }
            }
            if(redundant){
                int litc = getLiteralCount(leaves[i].first,this->nInput,this->nOutput);
                if(litc > litRedundant){
                    litRedundant = litc;
                    redundantCubeID = red_id;
                    ti_id = i;
                }
            }
            red_id++;
        }
        if(redundantCubeID > -1){
            for(unsigned long long m : leavesCOMs[ti_id]){
                if(COMsCount[m] < 2){
                    cout << "TA ERRADO" << endl;
                }
                COMsCount[m]--;
            }
            toInsertId.erase(toInsertId.begin()+redundantCubeID);
        }
        else{
            hasRedundant = false;
        }
    }
    vector<unsigned long long> cubesToInsert;
    for(unsigned j : toInsertId){
        cubesToInsert.push_back(leaves[j].first);
    }
    if(cubesToInsert.size() > cubesToRemove.size()){
        insertCubes(cubesToRemove);
        vector<unsigned long long> v1, v2;
        return make_tuple(0,v1,v2);
    }
    insertCubes(cubesToRemove);
    sort(cubesToInsert.begin(), cubesToInsert.end());
    cubesToInsert = getUnion(cubesToInsert,lastInserted);
    sort(cubesToRemove.begin(), cubesToRemove.end());
    vector<unsigned long long> cubesTR;
    for(unsigned long long ctr : cubesToRemove){
        if(!wasInserted(cubesToInsert,ctr)){
            cubesTR.push_back(ctr);
        }
    }
    cubesToRemove = getUnion(cubesTR,lastRemoved);
    redLit = redLit - getLiteralSum(cubesToInsert,this->nInput,this->nOutput);
    redLit = redLit + getLiteralSum(cubesToRemove,this->nInput,this->nOutput);
    return make_tuple(redLit, cubesToInsert, cubesToRemove);
}

vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> sop::combAndEstimateBest(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees, unsigned max_error, vector<unsigned long long>& lastInserted, vector<unsigned long long>& lastRemoved, unordered_set<unsigned long long>& last_EIC){
    tuple<int,vector<unsigned long long>,vector<unsigned long long>> ret;
    vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> best(3);
    map<int,vector<unsigned long long>,greater<int>> order;
    for(auto& t : trees){
        ret = estimateLiteralReductionLast(t.second,lastInserted,lastRemoved);
        if(get<0>(ret) > get<0>(best[t.first.size()])){
            last_EIC.insert(t.first.begin(),t.first.end());
            best[t.first.size()] = make_tuple(get<0>(ret),get<1>(ret),get<2>(ret),last_EIC);
            last_EIC.erase(t.first[0]);
            if(t.first.size()==2){
                last_EIC.erase(t.first[1]);
            }
        }
        if(t.first.size() == 1){
            order[get<0>(ret)] = t.first;
        }
    }
    double s1_size = order.size()*0.25, s2_size = order.size()*0.8; 
    unsigned s1_count = 0, s2_count = 0;
    auto t_it = next(trees.begin(),order.size());
    trees.erase(t_it,trees.end());
    unsigned estimateCount = 0;
    for(auto& s1 : order){
        if(s1_count > s1_size){
            break;
        }
        vector<unsigned long long> sct_comp = s1.second;
        s2_count=0;
        for(auto& s2 : order){
            if(s2_count > s2_size){
                break;
            }
            vector<unsigned long long> sct_first = s2.second;
            vector<long long unsigned> rootCubes = getUnion(sct_comp,sct_first);
            if(trees.count(rootCubes)==0 && rootCubes.size()==2){
                auto leaves = getUnion(trees[sct_comp],trees[sct_first]);
                trees[rootCubes] = leaves;
                ret = estimateLiteralReductionLast(leaves,lastInserted,lastRemoved);
                if(get<0>(ret) > get<0>(best[rootCubes.size()])){
                    last_EIC.insert(rootCubes.begin(),rootCubes.end());
                    best[rootCubes.size()] = make_tuple(get<0>(ret),get<1>(ret),get<2>(ret),last_EIC);
                    last_EIC.erase(rootCubes[0]);
                    if(rootCubes.size()==2){
                        last_EIC.erase(rootCubes[1]);
                    }
                }
            }
            s2_count++;
        }
        s1_count++;
    }
    return best;
}

vector<unsigned long long> sop::removableCubes(vector<unsigned long long>& minterms){
    unordered_map<unsigned long long,int> cubes_count;
    for(unsigned long long minterm : minterms){
        if(containsMinterm(minterm)){
            if(this->minterms[minterm].size()==1){
                unsigned long long cube = this->minterms[minterm][0];
                if(cubes_count.count(cube) == 0){
                    cubes_count[cube] = this->cubes[cube].size(); 
                }
                cubes_count[cube]--;
            }
        }
    }
    vector<unsigned long long> ret;
    for(auto cube : cubes_count){
        if(cube.second == 0){
            ret.push_back(cube.first);
        }
    }
    return ret;
}

vector<unsigned long long> sop::ALSRemove2(int noe, unordered_set<unsigned long long>& EIC){
    vector<unsigned long long> removedCube;
    int error = noe;
    unsigned long long one = 1, mask_in = (one<<(2*this->nInput))-1;
    unordered_map<unsigned long long,unordered_set<unsigned long long>> EICCubes;
    unordered_map<unsigned long long,tuple<double,unsigned,unordered_set<unsigned long long>,unsigned long long>> cubesToKey;
    set<tuple<double,unsigned,unordered_set<unsigned long long>,unsigned long long>, TupleRemoveComparator> bestCubes;
    unsigned long long best_cube = 0;
    double best_lit = 0.0;
    vector<unsigned long long> best_min;
    for(auto cube_pair : this->cubes){
        unsigned long long cube = cube_pair.first;
        unordered_set<unsigned long long> c_eic = cubeError2(cube,EIC);
        unsigned lit_count = getLiteralCount(cube,this->nInput,this->nOutput);
        double metric = 0.0;
        if(c_eic.size()!=0){
            metric = ((double)lit_count)/((double)c_eic.size());
        }
        else{
            metric = 2*(this->nInput+this->nOutput);
        }
        auto tupleKey = make_tuple(metric,lit_count,c_eic,cube);
        for(unsigned long long e : c_eic){
            if(EICCubes.count(e)==0){
                unordered_set<unsigned long long> v;
                EICCubes[e]=v;
            }
            EICCubes[e].insert(cube);
        }
        cubesToKey[cube]=tupleKey;
        bestCubes.insert(tupleKey);
    }
    unsigned i = 0;
    while(error > 0){
        auto best_it = bestCubes.begin();
        auto bestKey = (*best_it);
        best_cube = get<3>(bestKey);
        auto besEIC = get<2>(bestKey);
        vector<unsigned long long> cubesToZero;
        while(error < besEIC.size()){
            cubesToZero.push_back(best_cube);
            best_it++;
            if(best_it == bestCubes.end()){
                best_cube = 0;
                break;
            }
            bestKey = (*best_it);
            best_cube = get<3>(bestKey);
            besEIC = get<2>(bestKey);
        }
        if(best_cube!=0){
            for(unsigned i = 0; i < cubesToZero.size(); i++){
                bestCubes.erase(cubesToKey[cubesToZero[i]]);
                get<0>(cubesToKey[cubesToZero[i]]) = 0.0;
                bestCubes.insert(cubesToKey[cubesToZero[i]]);
            }
            for(auto removeEIC : get<2>(cubesToKey[best_cube])){
                if(EICCubes[removeEIC].size() == 1){
                    EICCubes.erase(removeEIC);
                }
                else{
                    EICCubes[removeEIC].erase(best_cube);
                }
            }
            if(this->cubes.count(best_cube)==0){
                cout << "Removendo cubo que não existe" << endl;
            }
            for(unsigned long long m : getCubeMinterms(best_cube)){
                if(this->minterms.count(m)>0){
                    unsigned removeId = 0;
                    for(unsigned long long c : this->minterms[m]){
                        if(c == best_cube){
                            break;
                        }
                        removeId++;
                    }
                    this->minterms[m].erase(this->minterms[m].begin() + removeId);
                    if(this->minterms[m].size() == 1){
                        this->cubes[this->minterms[m][0]].push_back(m);
                        unsigned long long new_eic = m&mask_in, cube = this->minterms[m][0];
                        if(contains(removedCube,cube)){
                            cout << "não era pra conter" << endl;
                        }
                        if(cube == best_cube){
                            cout << "não era pra ser igual" << endl;
                        }
                        if(EIC.count(new_eic)==0){
                            if(EICCubes.count(new_eic)==0){
                                unordered_set<unsigned long long> v;
                                EICCubes[new_eic]=v;
                            }
                            EICCubes[new_eic].insert(cube);
                            unordered_set<unsigned long long> c_eic = get<2>(cubesToKey[cube]);
                            bestCubes.erase(cubesToKey[cube]);
                            c_eic.insert(new_eic);
                            get<0>(cubesToKey[cube]) = ((double)get<1>(cubesToKey[cube]))/((double)c_eic.size());
                            get<2>(cubesToKey[cube]) = c_eic;
                            bestCubes.insert(cubesToKey[cube]);
                        }
                    }
                    if(this->minterms[m].size() == 0){
                        unsigned long long new_eic = m&mask_in;
                        EIC.insert(new_eic);
                        for(unsigned long long cube : EICCubes[new_eic]){
                            if(cubesToKey.count(cube) == 0){
                            }
                            else{
                                if(contains(removedCube,cube)){
                                    cout << "vai mesmo" << endl;
                                }
                                bestCubes.erase(cubesToKey[cube]);//pode dar ruim aqui
                                get<2>(cubesToKey[cube]).erase(new_eic);
                                if(get<2>(cubesToKey[cube]).size()>0){
                                    get<0>(cubesToKey[cube]) = ((double)get<1>(cubesToKey[cube]))/((double)get<2>(cubesToKey[cube]).size());
                                }
                                else{
                                    get<0>(cubesToKey[cube]) = 2*(this->nInput+this->nOutput);
                                }
                                bestCubes.insert(cubesToKey[cube]);
                            }
                        }
                        this->minterms.erase(m);
                    }
                }
                else{
                    cout << "não tinha o min" << endl;
                }
            }
            this->cubes.erase(best_cube);
            bestCubes.erase(cubesToKey[best_cube]);
            cubesToKey.erase(best_cube);
            error = error - besEIC.size();
            removedCube.push_back(best_cube);
        }
        else{
            break;
        }
    }
    return removedCube;
}

unordered_set<unsigned long long> sop::cubeError2(unsigned long long cube, unordered_set<unsigned long long>& EIC){
    unordered_set<unsigned long long> ret;
    unsigned long long one = 1, mask_in = (one<<(2*this->nInput))-1;
    if(this->cubes[cube].size()==0){
        cout << "cubo com 0 mintermos essenciais no cube error" << endl;
    }
    for(unsigned long long min : this->cubes[cube]){
        if(EIC.count(min&mask_in) == 0){
            ret.insert(min&mask_in);
        }
    }
    return ret;
}

unordered_set<long long unsigned> sop::expandCubes(){
    long long unsigned base = 3, one = 1, mask = 0;
    unordered_set<long long unsigned> ret;
    for(auto& cube_pair : this->cubes){
         unsigned long long cube = cube_pair.first;
        vector<long long unsigned> toInsert;
        for(unsigned i = 0; i < this->nInput; i++){
            mask = base << 2*i;
            if(((cube & mask)>>2*i)!=base){
                ret.insert(cube | mask);
            }
        }
        for(unsigned i = this->nInput*2; i < this->nInput*2+this->nOutput; i++){
            mask = one << i;
            if((cube & mask)>0){
                toInsert.push_back(cube ^ mask);
            }
        }
        if(toInsert.size()>1){
            for(unsigned long long c : toInsert){
                ret.insert(c);
            }
        }
    }
    return ret;
}

void sop::generateExpandSCT(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees, unsigned noe){
    unsigned long long one = 1;
    unsigned nInput = this->nInput, nOutput = this->nOutput;
    unsigned hasseSize = 0;
    bool toExpand = true;
    unordered_set<unsigned long long> cubesExpanded = expandCubes();
    for(unsigned long long cube : cubesExpanded){
        vector<unsigned long long> minterms;
        vector<unsigned long long> EIC;
        getEIC(cube,EIC,minterms,noe);
        if(EIC.size() <= noe && EIC.size()>0){
            vector<unsigned long long> cubesToRemove = removableCubes(minterms);
            unsigned removedLit = getLiteralSum(cubesToRemove,nInput,nOutput);
            unsigned cubeLit = getLiteralCount(cube, nInput, nOutput);
            if(cubeLit <= removedLit){
                vector<unsigned> values = {minterms.size(),cubesToRemove.size(),cubeLit};
                sort(EIC.begin(),EIC.end());
                if(trees.count(EIC) == 0){
                    vector<pair<unsigned long long,vector<unsigned>>> v;
                    v.push_back(make_pair(cube,values));
                    trees[EIC] = v;    
                }
                else{
                    trees[EIC].push_back(make_pair(cube,values));   
                }
            }
        }
    }
}

vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> sop::fixLastEICs(map<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>, RootComparator>& trees, vector<unsigned long long>& lastInserted, vector<unsigned long long>& lastRemoved, unordered_set<unsigned long long>& EIC){
    vector<tuple<int,vector<unsigned long long>,vector<unsigned long long>,unordered_set<unsigned long long>>> ret(2);
    vector<pair<vector<unsigned long long>,vector<pair<unsigned long long,vector<unsigned>>>>> cubesZero;
    map<vector<unsigned long long>,unsigned> toRemoveRelations;
    for(auto& tree : trees){
        unsigned i = 1, removeCod = 0;
        for(auto root : tree.first){
            if(EIC.count(root)!=0){
                if(tree.first.size() == 1){
                    removeCod = 3;
                }
                else{
                    removeCod += i;
                }
            }
            i++;
        }
        if(removeCod > 0 && removeCod < 3){
            toRemoveRelations[tree.first] = removeCod-1;
        }
        else if(removeCod == 3){
            cubesZero.push_back(tree);
        }
    }

    for(auto& cz : cubesZero){
        auto ret_est = estimateLiteralReductionLast(cz.second,lastInserted,lastRemoved);
        if(get<0>(ret_est) > get<0>(ret[0])){
            ret[1] = ret[0];
            ret[0] = make_tuple(get<0>(ret_est),get<1>(ret_est),get<2>(ret_est),EIC);
        }
        else if(get<0>(ret_est) > get<0>(ret[1])){
            ret[1] = make_tuple(get<0>(ret_est),get<1>(ret_est),get<2>(ret_est),EIC);
        }
    }

    if(get<0>(ret[0])==0){
        for(auto& trr : toRemoveRelations){
            vector<unsigned long long> root = trr.first;
            root.erase(root.begin()+trr.second);
            if(trees.count(root)==0){
                trees[root] = trees[trr.first];
            }
            else{
                trees[root] = getIntersection(trees[root],trees[trr.first]);
            }
            trees.erase(trr.first);
        }
    }
    return ret;
}
unordered_map<unsigned long long,unsigned long long> sop::getMagnitude(){
    unordered_map<unsigned long long,unsigned long long> ret;
    unsigned long long one = 1;
    unsigned long long mask_in = (one<<(2*this->nInput))-1;
    unsigned long long mask_out = ((one<<((2*this->nInput) + this->nOutput))-1) ^ mask_in;
    for(auto m_pair : this->minterms){
        unsigned long long minterm = m_pair.first;
        unsigned long long input_combination = mask_in&minterm;
        unsigned long long output_value = (mask_out&minterm)>>(2*this->nInput);
        if(ret.count(input_combination)==0){
            ret[input_combination] = output_value;
        }
        else{
            ret[input_combination] = ret[input_combination] | output_value;
        }
    }
    return ret;
}

unsigned sop::getSopLiteralCount(){
    unsigned sum = 0;
    for(auto cube : this->cubes){
        sum = sum + getLiteralCount(cube.first,this->nInput,this->nOutput);
    }
    return sum;
}