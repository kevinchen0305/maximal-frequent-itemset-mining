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
            clm.resize((n*n*n + 5*n) / 6, 0);//define clm size
        }
         
        void transaction_to_graph(string &transaction){
            istringstream is (transaction);
            int nn;
            vector<int> trans_vec;
            while(is >> nn){
                trans_vec.push_back(nn);
            }
            
            for(int node1=0; node1<trans_vec.size(); node1++){
                int i = trans_vec[node1];
                //cout << i*(-2*i*i + 3*n*i + 2 - 3*n) / 6 + i * (i + 1) / 2 - 1<< " ";
                //cout << i*(-2*i*i + (3*n+3)*i + 5 - 3*n) / 6 - 1 << " ";
                clm[i*(-2*i*i + (3*n+3)*i + 5 - 3*n) / 6 - 1] += 1;
                for(int node2=node1+1; node2<trans_vec.size(); node2++){
                    int j = trans_vec[node2] - 1;
                    //cout << j*(-2*j*j + 3*n*j + 2 - 3*n) / 6 + j * (j + 1) / 2  + (i-1) + (n-j-1)*j*(i-1) / j<< " ";
                    //cout << j*(-2*j*j + (3*n+3)*j + 5 - 3*n) / 6 + (n-j)*(i-1) << " ";
                    int second_node_index = j*(-2*j*j + (3*n+3)*j + 5 - 3*n) / 6 + (n-j)*(i-1);
                    clm[second_node_index] += 1;
                    for(int edge_node=node2+1; edge_node<trans_vec.size(); edge_node++){
                        clm[second_node_index + trans_vec[edge_node] - trans_vec[node2]] += 1;
                    }
                }
                //cout << endl;
            }
            //cout << endl;
        }

        vector<vector<int>> clm_miner_algo(vector<int> &clm, int &minsup){
            vector<vector<int>> max_frequent_itemset;
            vector<int> itemset;
            int item = 1;
            for(int major_col=0; major_col<clm.size(); major_col+=((n-item)*(item-1)+item), item++){
                int row_count = 1;
                for(int row=major_col; row_count<=item; row+=(n-item+1)){
                    if(clm[row] >= minsup){
                        //cout << row << " " << row_count << " " << item << " " << clm[row] << endl;
                        //cout << major_col << " " << row << endl;
                        if(item==row_count){
                            itemset.push_back(row_count);
                        }
                        else{
                            itemset.push_back(row_count);
                            itemset.push_back(item);

                            int start = row + 1;
                            int endloop = row + 1 + n - item;
                            int redundant_item = item + 1;
                            for(start; start<endloop; start++){
                                if(clm[start] >= minsup){
                                    itemset.push_back(redundant_item);
                                }
                                redundant_item++;
                            }
                        }

                        max_frequent_itemset.push_back(itemset);
                        
                        free_vector(itemset);
                    }
                    row_count++;
                }
            }

            sort(max_frequent_itemset.begin(), max_frequent_itemset.end(), sort_by_length);//set(max_frequent_itemset)

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

        vector<int> get_clm(){
            return clm;
        }
        
        template <typename T>
        void free_vector(T &vec){
            T().swap(vec);
        }

        void print_clm(vector<int> &clm){
            for(int i=0; i<clm.size(); i++){
                cout << clm[i] << " ";
            }
            cout << endl;
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
        vector<int> clm;
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
    
    vector<int> clm = c.get_clm();

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
