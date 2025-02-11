#include "model/model.h"
#include "search_algorithms/search.h"

#include <chrono>

int main(int argc, char* argv[]){
    // Information about data
    std::string file;
    int q = 0;
    int n = 0;
    // Gauge transformation
    bool gt = false;

    // Search method
    bool log_file = false;
    bool exhaustive = false;
    bool greedy = false;
    bool div_and_conq = false;

    // Process user input
    std::string arg;
    for (int i = 0; i < argc; ++i) {
        arg = argv[i];
        
        // Filename
        if (arg == "-f"){
            file = argv[i+1];
        }
        // Number of variables
        if (arg == "-n"){
            n = std::stoi(argv[i+1]);
        }
        // Number of states
        if (arg == "-q"){
            q = std::stoi(argv[i+1]);
        }
        // Log files to store the steps in the search process
        if (arg == "-l"){
            log_file = true;
        }
        // Gauge transformation
        if (arg == "-gt"){
            gt = true;
        }
        // Search method
        if (arg == "-es"){
            exhaustive = true;
        }
        if (arg == "-gs"){
            greedy = true;
        }
        if (arg == "-dc"){
            div_and_conq = true;
        }
    }
    // Number of states and number of variables are mandatory
    if (!q){
        std::cout << "Argument for number of states (-q) is missing." << std::endl;
        return 0;
    }
    if (!n){
        std::cout << "Argument for number of variables (-n) is missing." << std::endl;
        return 0;
    }
    if (n > 128){
        std::cout << "Too many variables. Maximum system size is 128." << std::endl;
        return 0;
    }

    // File should be located in the input folder
    std::string path = "../input/" + file + ".dat";

    // Construct mcm model
    mcm model = create_model(q, n, log_file);
    // Read in data
    std::vector<std::vector<__uint128_t>> data;
    data = data_processing(path, n, model.n_ints);
    if(data.size() == 0){return 1;}
    // Add the data to the model
    model.data = data;
    model.N = data.size();

    // Create output file in the output folder
    std::ofstream outputFile("../output/" + file + "_output.dat");

    // Gauge transformation
    if (gt){
        auto start = std::chrono::high_resolution_clock::now();
        find_best_basis(model, 4);
        transform_data(model.data, model.best_basis, q, n, model.n_ints);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::vector<int> state(n);

        // Write the best basis to the output file
        outputFile << "############################# \n";
        outputFile << "# Search for the best basis # " << '\n';
        outputFile << "############################# \n \n";

        outputFile << "Duration: " << duration.count() / 1000 << "s \n" << '\n';
        outputFile << "Best basis:" << "\n\n";
        // Iterate over all n operators
        for (int i = 0; i < n; ++i){
            // Convert representation from n_ints 128bit integers to n integers between 0 and q-1
            std::fill(state.begin(), state.end(), 0);
            std::vector<__uint128_t> op = model.best_basis[i];
            for (int j = 0; j < n; ++j){
                int element = 1;
                for (int k = 0; k < model.n_ints; ++k){
                    if (op[k] & 1){
                        state[j] += element;
                    }
                    element <<= 1;
                    op[k] >>=1;
                }
            }
            for (auto element : state){
                outputFile << element;
            }
            outputFile << '\n';
        }
        outputFile << '\n';
    }

    // Exhaustive search
    if (exhaustive){
        auto start = std::chrono::high_resolution_clock::now();
        exhaustive_search(model);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        outputFile << "##################### \n";
        outputFile << "# Exhaustive search # " << '\n';
        outputFile << "##################### \n\n";

        outputFile << "Duration: " << duration.count() / 1000 << "s \n" << '\n';
        outputFile << "Number of equivalent best MCMs found : " << model.best_mcm.size() << "\n\n";
        outputFile << "Best MCM(s): " << std::endl;
        outputFile << "\n";
        for (int i = 0; i < model.best_mcm.size(); ++i){
            print_partition_to_file(outputFile, model.best_mcm[i]);
            outputFile << "\n";
        }
        outputFile << "Best log-evidence: " << model.best_evidence << "\n" << '\n';
    }

    // Greedy search
    if (greedy){
        model.best_mcm.clear();
        if(log_file){
            model.greedy_file = new std::ofstream("../output/" + file + "_greedy_search.dat");
        }

        auto start = std::chrono::high_resolution_clock::now();
        greedy_search(model);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        if(log_file){
            model.greedy_file->close();
        }
        outputFile << "################# \n";
        outputFile << "# Greedy search # \n";
        outputFile << "################# \n\n";

        outputFile << "Duration: " << duration.count() / 1000 << "s \n" << '\n';    

        outputFile << "Best MCM: " << std::endl;
        outputFile << "\n";
        print_partition_to_file(outputFile, model.best_mcm[0]);
        outputFile << "\n";
        outputFile << "Best log-evidence: " << model.best_evidence << "\n" <<std::endl;
    }

    // Divide and conquer
    if (div_and_conq){
        model.best_mcm.clear();
        if(log_file){
            model.divide_and_conquer_file = new std::ofstream("../output/" + file + "_divide_and_conquer.dat");
        }

        auto start = std::chrono::high_resolution_clock::now();
        divide_and_conquer(model);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        if(log_file){
            model.divide_and_conquer_file->close();
        }
        outputFile << "###################### \n";
        outputFile << "# Divide and conquer # \n";
        outputFile << "###################### \n\n";

        outputFile << "Duration: " << duration.count() / 1000 << "s \n" << '\n'; 

        outputFile << "Best MCM: " << std::endl;
        outputFile << "\n";
        print_partition_to_file(outputFile, model.best_mcm[0]);
        outputFile << "\n";
        outputFile << "Best log-evidence: " << model.best_evidence << "\n" << std::endl;
    }

    // Close output file
    outputFile << "Search done" << std::endl;
    outputFile.close();
    return 0;
}

