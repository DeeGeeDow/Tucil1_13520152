#include<bits/stdc++.h>
#include<ctime>

using namespace std;

const string vertical_direction[3] = {" ATAS", " ", " BAWAH"};
const string horizontal_direction[3] = {" KIRI" , " ", " KANAN"};
int n,m;
int total_count = 0;
typedef struct {
    int i;
    int j;
    short direct_i;
    short direct_j;
} ans;
queue<string> words;
vector<vector<char>> puzzle;
fstream puzzle_stream;
vector<vector<char>> solution;

void readPuzzle(){
    char tmp;
    bool wordinput = false;
    string input;

    puzzle_stream.open("../test/puzzle.txt", ios::in);
    if(puzzle_stream.is_open()){
        while(!wordinput && getline(puzzle_stream,input)){
            vector<char> line;
            for(int i=0; i<input.size(); i++){
                if(input[i] != ' '){
                    line.push_back(input[i]);
                }
            }

            if(line.empty()) wordinput = true;
            else puzzle.push_back(line);
        }
        
        while(getline(puzzle_stream, input)){
            words.push(input);
        }
        puzzle_stream.close();
    }
}

void initializeSolution(){
    solution.clear();
    for(int i=0; i<puzzle.size(); i++){
        vector<char> sol;
        for(int j=0; j<puzzle[0].size(); j++){
            sol.push_back('-');
        }
        solution.push_back(sol);
    }
}

void showSolution(ans cur, int len){
    int solution_idx = 0;
    int i_ = cur.i;
    int j_ = cur.j;
    short direct_i = cur.direct_i;
    short direct_j = cur.direct_j;

    initializeSolution();
    while(solution_idx<len){
        solution[i_][j_] = puzzle[i_][j_];
        i_ += direct_i;
        j_ += direct_j; 
        solution_idx++;
    }

    for(int i=0; i<solution.size(); i++){
        for(int j=0; j<solution[i].size(); j++){
            cout << solution[i][j] << " ";
        }
        cout << endl;
    }
}
void solvePuzzle(int ver, int hor){
    /* 
    Proses Menyelesaikan Puzzle
    Ide : Ambil satu kata, cek tiap huruf sampai menemukan huruf pertama dari kata tersebut,
          periksa empat arah apakah mungkin menemukan kata tersebut, cek huruf kedua dari arah-arah yang mungkin,
          periksa huruf-huruf selanjutnya dari arah tersebut jika huruf keduanya cocok, lakukan sampai ditemukan
    */
   std:: clock_t start, start_word;
   double duration, word_duration;

   start = clock();
    while(!words.empty()){
        string word = words.front();
        
        cout << "Processing word: " << word << "...\r" << flush;
        start_word = clock();
        int i=0;
        int j=0;
        int count = 0;
        bool found = false;
        ans current_answer;
        while(!found && i<ver){
            count++;
            if(puzzle[i][j] == word[0]){
                current_answer = {i,j,-1,-1};
                bool vertical_check[3] = {
                    word.length()<=i+1, 
                    true,
                    word.length()<=ver-i, 
                };
                bool horizontal_check[3] = {
                    word.length()<=j+1,
                    true,
                    word.length()<=hor-j 
                };
                
                while(!found && current_answer.direct_i<=1){
                    int i_ = current_answer.i;
                    int j_ = current_answer.j;
                    short direct_i = current_answer.direct_i;
                    short direct_j = current_answer.direct_j;
                    if(horizontal_check[direct_j+1] && vertical_check[direct_i+1] && !(direct_i==0 && direct_j==0)){
                        count++;
                        i_ += direct_i;
                        j_ += direct_j;
                        int word_cocok = 1;
                        while(word_cocok<word.length() && word[word_cocok]==puzzle[i_][j_]){
                            count++;
                            word_cocok++;
                            i_ += direct_i;
                            j_ += direct_j;
                        }

                        if(word_cocok == word.length()) found = true;
                    }

                    if(!found){
                        current_answer.direct_j++;
                        if(current_answer.direct_j>1){
                            current_answer.direct_i++;
                            current_answer.direct_j = -1;
                        }
                    }
                }
            }

            j++;
            if(j>=hor){
                i++;
                j%=hor;
            }
        }

        if(found){
            word_duration = (clock() - start_word)*1000 / (double) CLOCKS_PER_SEC;
            cout << "Found " << words.front() << " at (" << current_answer.i << "," << current_answer.j << ") with direction" << vertical_direction[current_answer.direct_i+1] << horizontal_direction[current_answer.direct_j+1] << "\ncount : " << count << "\nOperation takes " << word_duration << "ms" << endl;
            showSolution(current_answer, word.length());
            cout << endl;

        }
        words.pop();
        total_count += count;
    }
    duration = (clock() - start)*1000 / (double) CLOCKS_PER_SEC;
    cout << "Operation took " << duration << "ms\nTotal count : " << total_count << endl;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    readPuzzle();
    solvePuzzle(puzzle.size(), puzzle[0].size());
}