#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
bool correct_num(std::vector<std::vector<int>>&kontrola){  //kontroluje jestli v kazde bunce je nejake cislo
    if(kontrola.size()!=9){
        return false;
    }
    for(int i=0;i<9;i++){
        if(kontrola.at(i).size()!=9){
            return false;
        }
    }
    return true;
}
vector<vector<int>>parse_for_vector(string& soubor){   //otevre soubor a nacte z nej cisla
    int cislo;
    vector<int>numbers;
    vector<vector<int>>cisla;
    cisla.resize(9);
    string slovo;
    ifstream in(soubor);
    if(!in.is_open()){
        cout<<"ERROR! CANT OPEN A FILE!"<<endl;
    }
    while(in>>slovo){
        cislo=stoi(slovo);
        numbers.push_back(cislo);
    }
    in.close();
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cisla.at(i).push_back(numbers.at(9*i+j));
        }
    }
    return cisla;
}
