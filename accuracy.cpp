#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>

void compute_accuracy(double* as, bool* attack, int timeNum, int topN)
{
    if (topN > timeNum)
    {
        std::cerr << "Error: topN cannot be greater than timeNum." << std::endl;
        std::cerr << "topN = " << topN << std::endl;
        std::cerr << "timeNum = " << timeNum << std::endl;
        return;
    }
    int tp, fp, tn, fn;
    tp = fp = tn = fn = 0;
    std::vector<size_t> idx(timeNum);
    std::iota(idx.begin(), idx.end(), 0); // Fill idx with values from 0 to timeNum-1

    std::ofstream debugFile("debug_accuracy.txt", std::ios_base::out); // Open a debug file for writing
    if (!debugFile.is_open()) {
        std::cerr << "Failed to open debug file." << std::endl;
        return;
    }

    debugFile << "Starting compute_accuracy with topN = " << topN << std::endl;

    std::sort(idx.begin(), idx.end(), [&as](size_t i1, size_t i2) {return as[i1] > as[i2];}); // Sort indices by anomaly scores in descending order

    debugFile << "Sorted indices by anomaly score." << std::endl;
    debugFile << "Top " << topN << " indices after sorting:" << std::endl;
    for (int i = 0; i < topN; ++i) {
        debugFile << "idx[" << i << "] = " << idx[i] << ", as[" << idx[i] << "] = " << as[idx[i]] << ", attack[" << idx[i] << "] = " << attack[idx[i]] << std::endl;
    }

    for(int i = 0; i < topN; i++)
    {
        try 
        {
            if(attack[idx[i]])
                tp++;
            else
                fp++;
        } catch (const std::out_of_range& e)
        {
            std::cerr << "Out of range error: " << e.what() << std::endl;
            return;
        }
    }

    for(int i = topN; i < timeNum; i++)
    {
        if(attack[idx[i]])
            fn++;
        else
            tn++;
    }

    double precision = double(tp)/(tp+fp);
    double recall = double(tp)/(tp+fn);
    double f1 = 2*precision*recall/(precision+recall);
    double TPR = double(tp)/(tp+fn);
    double FPR = double(fp)/(fp+tn);

    // Write the final computed values to the debug file
    debugFile << "True Positives (TP): " << tp << std::endl;
    debugFile << "False Positives (FP): " << fp << std::endl;
    debugFile << "True Negatives (TN): " << tn << std::endl;
    debugFile << "False Negatives (FN): " << fn << std::endl;
    debugFile << "Precision: " << precision << std::endl;
    debugFile << "Recall: " << recall << std::endl;
    debugFile << "F1 Score: " << f1 << std::endl;
    debugFile << "True Positive Rate (TPR): " << TPR << std::endl;
    debugFile << "False Positive Rate (FPR): " << FPR << std::endl;

    // Output results to the console
    std::cout << "[TOP" << topN << "] precision: " << precision << ", recall: " << recall << std::endl;

    debugFile << "Finished compute_accuracy." << std::endl; // Mark the end of the function in the debug file
    debugFile.close();
}
