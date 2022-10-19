#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<chrono>
using namespace std;

class CLM_miner{
    public:
        CLM_miner(int item_num) :n(item_num)
        {
            column.resize(n*n+n, 0);//define clm size
            clm.resize(n, column);
        }
    
        void transaction_to_graph(string &transaction){
            istringstream is (transaction);
            int nn;
            vector<int> trans_vec;
            while(is >> nn){
                trans_vec.push_back(nn);
            }

            for(int node1=0; node1<trans_vec.size(); node1++){
                int node = trans_vec[node1]-1;
                clm[node][node*n+node] += 1;
                for(int node2=node1+1; node2<trans_vec.size(); node2++){
                    build_completely_linked_matrix(node1, node2, trans_vec);
                }
            }
        }

        void build_completely_linked_matrix(int &node1, int &node2, vector<int> &transaction){
            int index_a = transaction[node1] - 1;
            int index_b = transaction[node2] - 1;
            clm[index_a][index_b*n+index_b] += 1;
            for(int i=node2+1; i<transaction.size(); i++){
                int index_c = transaction[i];
                clm[index_a][(index_b*n+index_b)+index_c] += 1;
            }
        }

        vector<vector<int>> clm_miner_algo(vector<vector<int>> &clm, int &minsup){
            vector<vector<int>> max_frequent_itemset;
            vector<int> itemset;
            for(int major_col=0; major_col<clm[0].size(); major_col+=n+1){
                for(int row=0; row<clm.size(); row++){
                    //cout << row << " " << major_col << endl;
                    if(clm[row][major_col] >= minsup){
                        itemset.push_back(row+1);
                        if(row != major_col%n){
                            itemset.push_back((major_col%n)+1);
                        }
                        int start = major_col + 1;
                        int endloop = major_col + n;
                        for(start; start<=endloop; start++){
                            if(clm[row][start] >= minsup){
                                itemset.push_back(start - major_col);
                            }    
                        }

                        max_frequent_itemset.push_back(itemset);

                        free_vector(itemset);
                    }
                }
            }

            sort(max_frequent_itemset.begin(), max_frequent_itemset.end(), sort_by_length);

            vector<vector<int>>::iterator it = max_frequent_itemset.begin();
            for(int i=0; i<max_frequent_itemset.size(); i++){
                for(int j=max_frequent_itemset.size()-1; j>=0; j--){
                    if(max_frequent_itemset[i].size()==max_frequent_itemset[j].size()){
                        break;
                    }

                    if(includes(max_frequent_itemset[j].begin(), max_frequent_itemset[j].end(), max_frequent_itemset[i].begin(), max_frequent_itemset[i].end())){
                        //cout << i << " " << true << endl;
                        max_frequent_itemset.erase(it+i);
                        i--;
                        break;
                    }
                }
            }

            return max_frequent_itemset;
        }

        static bool sort_by_length(vector<int> &vec1, vector<int> &vec2){
            return vec1.size() < vec2.size();
        }

        vector<vector<int>> get_clm(){
            return clm;
        }
        
        template <typename T>
        void free_vector(T &vec){
            T().swap(vec);
        }
        
        void print_clm(vector<vector<int>> &clm){
            for(int i=0; i<clm.size(); i++){
                for(int j=0; j<clm[i].size(); j++){
                    cout << clm[i][j] << " ";
                }
                cout << endl;
            }
        }

        void print_maximalFI(vector<vector<int>> &maxFI_vec){
            for(int i=0; i<maxFI_vec.size(); i++){
                for(int j=0; j<maxFI_vec[i].size(); j++){
                    cout << maxFI_vec[i][j] << " ";
                }
                cout << endl;
            }
        }
    
    private:
        int n;
        vector<int> column;
        vector<vector<int>> clm;
};


int main(int argc, char *argv[])
{
    int n = atoi(argv[2]);
    int minsup;
    cout << "input minimum support count: ";
    cin >> minsup;

    CLM_miner c(n);
    auto clock_start = std::chrono::high_resolution_clock::now();
    ifstream myfile;
    string line;
    myfile.open(argv[1]);
    if(myfile.is_open()){
        while(getline(myfile, line)){
            stringstream ss ( line );
            while(getline(ss ,line, ',')){
                c.transaction_to_graph(line);
		}
        }
    }
    
    vector<vector<int>> clm = c.get_clm();

    //c.print_clm(clm); //print clm
    
    //auto clock_start = std::chrono::high_resolution_clock::now();
    vector<vector<int>> max_frequent_itemset = c.clm_miner_algo(clm, minsup);
    auto clock_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> miner_time = clock_end - clock_start;
    cout << miner_time.count() << " seconds" << endl;

    if(max_frequent_itemset.size() == 0){
        cout << "No maximal frequent itemset on support " << minsup;
    }
    else{
        cout << "the number of maximal frequent itemset on support " << minsup << " : " << max_frequent_itemset.size() << " itemsets" << endl;
    }

    //c.free_vector(clm);
    
    c.print_maximalFI(max_frequent_itemset);

    return 0;
}
