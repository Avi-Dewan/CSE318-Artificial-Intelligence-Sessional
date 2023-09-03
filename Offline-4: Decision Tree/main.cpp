#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include<cmath>
#include<random>
#include "car.h"

using namespace std;

class Node
{
private:
   
public:
    Node *parent;
    vector<Node*> children;
    bool isLeaf = false;
    string leafValue;
    string nextAttribute;
    string nodeValue;

    Node() {
        isLeaf = false;
    };
    ~Node();

    void printTree(int t) {
        if(isLeaf) {
            cout << string(t, '\t') << leafValue << endl;
        } else {
            cout << string(t, '\t') << nodeValue << " " << nextAttribute << endl;

            for(Node *n: children) {
                n->printTree(t+1);
            }

        }
    }

    string predict(Car c) {
        if(isLeaf) {
            return leafValue;
        }
        else {
            
            if(nextAttribute == "buying") {
                for(Node *n: children) {
                    if(n->nodeValue == c.buying) return n->predict(c);
                }
            } else if (nextAttribute == "maint") {
                for(Node *n: children) {
                    if(n->nodeValue == c.maint) return n->predict(c);
                }
            } else if (nextAttribute == "doors") {
                for(Node *n: children) {
                    if(n->nodeValue == c.doors) return n->predict(c);
                }
            } else if (nextAttribute == "persons") {
                for(Node *n: children) {
                    if(n->nodeValue == c.persons) return n->predict(c);
                }
            } else if (nextAttribute == "lug_boot") {
                for(Node *n: children) {
                    if(n->nodeValue == c.lug_boot) return n->predict(c);
                }
            } else if (nextAttribute == "safety") {
                for(Node *n: children) {
                    if(n->nodeValue == c.safety) return n->predict(c);
                }
            } 
            else {
                leafValue = "unacc";
                return leafValue;
            }
            
        }

        leafValue = "unacc";
        return leafValue;
    }
};

class DecisionTree
{
private:
    Node * root;

public:
    DecisionTree(vector<Car> datas);
    Node* DecisionTreeLearning(vector<Car> datas, vector<string> attributes, string parentMajorityClass);
    double entropy(vector<Car> datas);
    double gain(vector<Car> datas, string attribute, double eBS);
    void printTree();

    string predict(Car c);
    ~DecisionTree();
};

DecisionTree::DecisionTree(vector<Car> datas)
{
    vector<string> attributes = {"buying", "maint", "doors", "persons", "lug_boot", "safety" };
  
    root = DecisionTreeLearning(datas,attributes, "none");
}

DecisionTree::~DecisionTree()
{
}

string DecisionTree::predict(Car c) {
    return root->predict(c);
}

void DecisionTree::printTree() {

    cout << "root" << endl;

    cout << root->nextAttribute << endl;


    for(Node *n: root->children) {
        cout << " " << n->nodeValue << endl;
        n->printTree(2);
    }
}

double DecisionTree::entropy(vector<Car> datas) {
    int unacc_count = 0,
        acc_count = 0, 
        good_count = 0, 
        vGood_count = 0,
        total_Count = datas.size();

    for(Car c: datas) {
        if(c.classValue == "unacc") unacc_count++;
        else if(c.classValue == "acc") acc_count++;
        else if(c.classValue == "good") good_count++;
        else if(c.classValue == "vgood") vGood_count++;
    }

    double ent = 0, prob= 0;

    if(unacc_count != 0) {
        prob = unacc_count*1.0/total_Count;
        ent += ( prob * log2(prob)); 
    } 

    if(acc_count != 0) {
        prob = acc_count*1.0/total_Count;
        ent += ( prob * log2(prob)); 
    }

    if(good_count != 0) {
        prob = good_count*1.0/total_Count;
        ent += ( prob * log2(prob)); 
    }

    if(vGood_count != 0) {
        prob = vGood_count*1.0/total_Count;
        ent += ( prob * log2(prob)); 
    }

    ent *= -1;

    return ent;
}

double DecisionTree::gain(vector<Car> datas, string attribute, double eBS) {

    double eAS = 0;

    if(attribute == "buying") {
        vector<Car> vHighs, highs, meds, lows;

        for(Car c: datas ) {
            if(c.buying == "low") lows.push_back(c);
            else if(c.buying == "med") meds.push_back(c);
            else if(c.buying == "high") highs.push_back(c);
            else if(c.buying == "vhigh") vHighs.push_back(c);
        } 

        eAS = ((lows.size()*1.0/datas.size()) * entropy(lows) )+
                  ((meds.size()*1.0/datas.size()) * entropy(meds)) +
                  ((highs.size()*1.0/datas.size()) * entropy(highs)) +
                  (vHighs.size()*1.0/datas.size()) * entropy(vHighs);

        return (eBS - eAS);

    } else if (attribute == "maint") {
         vector<Car> vHighs, highs, meds, lows;

        for(Car c: datas ) {
            if(c.maint == "low") lows.push_back(c);
            else if(c.maint == "med") meds.push_back(c);
            else if(c.maint == "high") highs.push_back(c);
            else if(c.maint == "vhigh") vHighs.push_back(c);   
        } 

        eAS = (lows.size()*1.0/datas.size()) * entropy(lows) +
                  (meds.size()*1.0/datas.size()) * entropy(meds) +
                  (highs.size()*1.0/datas.size()) * entropy(highs) +
                  (vHighs.size()*1.0/datas.size()) * entropy(vHighs);
          
        return (eBS - eAS);

    }  else if (attribute == "doors") {
         vector<Car> twos, threes, fours, fives;

        for(Car c: datas ) {
            if(c.doors == "2") twos.push_back(c);
            else if(c.doors == "3") threes.push_back(c);
            else if(c.doors == "4") fours.push_back(c);
            else if(c.doors == "5more") fives.push_back(c);
        } 

        eAS = (twos.size()*1.0/datas.size()) * entropy(twos) +
                  (threes.size()*1.0/datas.size()) * entropy(threes) +
                  (fours.size()*1.0/datas.size()) * entropy(fours) +
                  (fives.size()*1.0/datas.size()) * entropy(fives);
        
        return (eBS - eAS);

    } else if (attribute == "persons") {
         vector<Car> twos, fours, more;

        for(Car c: datas ) {
            if(c.persons == "2") twos.push_back(c);
            else if(c.persons == "4") fours.push_back(c);
            else if(c.persons == "more") more.push_back(c);
        } 

        eAS = (twos.size()*1.0/datas.size()) * entropy(twos) +
                  (fours.size()*1.0/datas.size()) * entropy(fours) +
                  (more.size()*1.0/datas.size()) * entropy(more);
        
        return (eBS - eAS);

    } else if (attribute == "lug_boot") {
         vector<Car>  bigs, meds, smalls;

        for(Car c: datas ) {
            if(c.lug_boot == "small") smalls.push_back(c);
            else if(c.lug_boot == "med") meds.push_back(c);
            else if(c.lug_boot == "big") bigs.push_back(c); 
        } 

        eAS = (smalls.size()*1.0/datas.size()) * entropy(smalls) +
                  (meds.size()*1.0/datas.size()) * entropy(meds) +
                  (bigs.size()*1.0/datas.size()) * entropy(bigs);
            
        return (eBS - eAS);

    } else if (attribute == "safety") {
         vector<Car>  highs, meds, lows;

        for(Car c: datas ) {
            if(c.safety == "low") lows.push_back(c);
            else if(c.safety == "med") meds.push_back(c);
            else if(c.safety == "high") highs.push_back(c);   
        } 

        eAS = ((lows.size()*1.0/datas.size()) * entropy(lows) )+
                 ( (meds.size()*1.0/datas.size()) * entropy(meds)) +
                  ((highs.size()*1.0/datas.size()) * entropy(highs));
          
        return (eBS - eAS);

    } 

    return 0;  
}

Node* DecisionTree::DecisionTreeLearning(vector<Car> examples, vector<string> attributes, string parentMajorityClass) {
    
    Node *node = new Node();

    if(examples.size() == 0) {
        node->isLeaf = true;
        node->leafValue = parentMajorityClass;
        return node;
    }

    int unacc_count = 0,
        acc_count = 0, 
        good_count = 0, 
        vGood_count = 0,
        total_Count = examples.size();
    

    for(Car c: examples) {
        if(c.classValue == "unacc") unacc_count++;
        else if(c.classValue == "acc") acc_count++;
        else if(c.classValue == "good") good_count++;
        else if(c.classValue == "vgood") vGood_count++;
    }

    int maxCount = max( unacc_count, max(acc_count, max(good_count, vGood_count)));

    string currentMajorityClass;

    if(maxCount == unacc_count) currentMajorityClass = "unacc";
    else if(maxCount == acc_count) currentMajorityClass = "acc";
    else if(maxCount == good_count) currentMajorityClass = "good";
    else currentMajorityClass = "vgood";
    
    
    if(unacc_count == total_Count) {
        node->isLeaf = true;
        node->leafValue = "unacc";
        return node;
    } else if(acc_count == total_Count) {
        node->isLeaf = true;
        node->leafValue = "acc";
        return node;
    } else if(good_count == total_Count) {
        node->isLeaf = true;
        node->leafValue = "good";
        return node;
    } else if(vGood_count == total_Count) {
        node->isLeaf = true;
        node->leafValue = "vgood";
        return node;
    }

    if(attributes.size() == 0) {
        node->isLeaf = true;
        node->leafValue = currentMajorityClass;
        return node;
    }

    double Entropy_bs = entropy(examples);

    string maxAttribute;
    double maxGain = 0;

    if(attributes.size() == 1) {
        maxAttribute = attributes[0];
    } else {
        for(string atr: attributes) {
            double g = gain(examples, atr, Entropy_bs);
            if(g > maxGain) {
                maxGain = g;
                maxAttribute = atr;
            }
        }
    }
 
    vector<string> newAttributes;
    node->nextAttribute = maxAttribute;

    if(maxAttribute == "buying") {

        vector<Car> vHighs, highs, meds, lows;
        for(Car c: examples ) {
            if(c.buying == "low") lows.push_back(c);
            else if(c.buying == "med") meds.push_back(c);
            else if(c.buying == "high") highs.push_back(c);
            else if(c.buying == "vhigh") vHighs.push_back(c);
        } 

        newAttributes.clear();
        for(string s: attributes) {
            if(s != maxAttribute) {
                newAttributes.push_back(s);
            }
        }

        Node *n1 = DecisionTreeLearning(lows, newAttributes, currentMajorityClass);
        Node *n2 = DecisionTreeLearning(meds, newAttributes, currentMajorityClass);
        Node *n3 = DecisionTreeLearning(highs, newAttributes, currentMajorityClass);
        Node *n4 = DecisionTreeLearning(vHighs, newAttributes, currentMajorityClass);

        n1->nodeValue = "low";
        n2->nodeValue = "med";
        n3->nodeValue = "high";
        n4->nodeValue = "vhigh";


        node->children.push_back(n1);
        node->children.push_back(n2);
        node->children.push_back(n3);
        node->children.push_back(n4);

    } else if (maxAttribute == "maint") {
         
        vector<Car> vHighs, highs, meds, lows;
        for(Car c: examples ) {
            if(c.maint== "low") lows.push_back(c);
            else if(c.maint== "med") meds.push_back(c);
            else if(c.maint== "high") highs.push_back(c);
            else if(c.maint== "vhigh") vHighs.push_back(c);   
        } 

        newAttributes.clear();
        for(string s: attributes) {
            if(s != maxAttribute) {
                newAttributes.push_back(s);
            }
        }


        Node *n1 = DecisionTreeLearning(lows, newAttributes, currentMajorityClass);
        Node *n2 = DecisionTreeLearning(meds, newAttributes, currentMajorityClass);
        Node *n3 = DecisionTreeLearning(highs, newAttributes, currentMajorityClass);
        Node *n4 = DecisionTreeLearning(vHighs, newAttributes, currentMajorityClass);

        n1->nodeValue = "low";
        n2->nodeValue = "med";
        n3->nodeValue = "high";
        n4->nodeValue = "vhigh";

        node->children.push_back(n1);
        node->children.push_back(n2);
        node->children.push_back(n3);
        node->children.push_back(n4);

    }  else if (maxAttribute == "doors") {

        vector<Car> twos, threes, fours, fives;
        for(Car c: examples ) {
            if(c.doors == "2") twos.push_back(c);
            else if(c.doors == "3") threes.push_back(c);
            else if(c.doors == "4") fours.push_back(c);
            else if(c.doors == "5more") fives.push_back(c);
        } 

        newAttributes.clear();
        for(string s: attributes) {
            if(s != maxAttribute) {
                newAttributes.push_back(s);
            }
        }

        Node *n1 = DecisionTreeLearning(twos, newAttributes, currentMajorityClass);
        Node *n2 = DecisionTreeLearning(threes, newAttributes, currentMajorityClass);
        Node *n3 = DecisionTreeLearning(fours, newAttributes, currentMajorityClass);
        Node *n4 = DecisionTreeLearning(fives, newAttributes, currentMajorityClass);

        n1->nodeValue = "2";
        n2->nodeValue = "3";
        n3->nodeValue = "4";
        n4->nodeValue = "5more";

        node->children.push_back(n1);
        node->children.push_back(n2);
        node->children.push_back(n3);
        node->children.push_back(n4);

    } else if (maxAttribute == "persons") {
        
        vector<Car> twos, fours, more;
        for(Car c: examples ) {
            if(c.persons == "2") twos.push_back(c);
            else if(c.persons == "4") fours.push_back(c);
            else if(c.persons == "more") more.push_back(c);
        } 

        newAttributes.clear();

        for(string s: attributes) {
            if(s != maxAttribute) {
                newAttributes.push_back(s);
            }
        }

        Node *n1 = DecisionTreeLearning(twos, newAttributes, currentMajorityClass);
        Node *n2 = DecisionTreeLearning(fours, newAttributes, currentMajorityClass);
        Node *n3 = DecisionTreeLearning(more, newAttributes, currentMajorityClass);

        n1->nodeValue = "2";
        n2->nodeValue = "4";
        n3->nodeValue = "more";

        node->children.push_back(n1);
        node->children.push_back(n2);
        node->children.push_back(n3);

    } else if (maxAttribute == "lug_boot") {
        vector<Car>  bigs, meds, smalls;

        for(Car c: examples ) {
            if(c.lug_boot == "small") smalls.push_back(c);
            else if(c.lug_boot == "med") meds.push_back(c);
            else if(c.lug_boot == "big") bigs.push_back(c); 
        } 

        newAttributes.clear();

        for(string s: attributes) {
            if(s != maxAttribute) {
                newAttributes.push_back(s);
            }
        }

        Node *n1 = DecisionTreeLearning(smalls, newAttributes, currentMajorityClass);
        Node *n2 = DecisionTreeLearning(meds, newAttributes, currentMajorityClass);
        Node *n3 = DecisionTreeLearning(bigs, newAttributes, currentMajorityClass);

        n1->nodeValue = "small";
        n2->nodeValue = "med";
        n3->nodeValue = "big";

        node->children.push_back(n1);
        node->children.push_back(n2);
        node->children.push_back(n3);

    } else if (maxAttribute == "safety") {

        vector<Car>  highs, meds, lows;
        for(Car c: examples ) {
            if(c.safety == "low") lows.push_back(c);
            else if(c.safety == "med") meds.push_back(c);
            else if(c.safety == "high") highs.push_back(c);   
        }

        newAttributes.clear();

        for(string s: attributes) {
            if(s != maxAttribute) {
                newAttributes.push_back(s);
            }
        }

        Node *n1 = DecisionTreeLearning(lows, newAttributes, currentMajorityClass);
        Node *n2 = DecisionTreeLearning(meds, newAttributes, currentMajorityClass);
        Node *n3 = DecisionTreeLearning(highs, newAttributes, currentMajorityClass);

        n1->nodeValue = "low";
        n2->nodeValue = "med";
        n3->nodeValue = "high";

        node->children.push_back(n1);
        node->children.push_back(n2);
        node->children.push_back(n3);

    } 


    return node;

}

double runTest(vector<Car> Data, int test_id) {

    cout << "\t\t\t Test - " << test_id << endl;
    // Create a random number generator engine
    random_device rd;  // Random device to seed the generator
    mt19937 generator(rd()); // Mersenne Twister engine

    // Define a random number distribution for the range [1, 10]
    uniform_int_distribution<int> distribution(1, 10);

    vector<Car> train, test;
    int r;

    for(Car c: Data) {
        r = distribution(generator);

        if(r <= 8) train.push_back(c);
        else test.push_back(c); 
    }

    DecisionTree decisionTree(train);


    cout << "\n\t\tTotal Data = " << Data.size() << endl;
    cout << "Train Data = " << train.size() << "\t\t" << "Test Data = " << test.size() << endl; 

    int train_unacc_count = 0,
        train_acc_count = 0, 
        train_good_count = 0, 
        train_vGood_count = 0,
        test_unacc_count = 0,
        test_acc_count = 0, 
        test_good_count = 0, 
        test_vGood_count = 0;


    for(Car c: train) {
        if(c.classValue == "unacc") train_unacc_count++;
        else if(c.classValue == "acc") train_acc_count++;
        else if(c.classValue == "good") train_good_count++;
        else if(c.classValue == "vgood") train_vGood_count++;
    }

    int correctPrediction_unacc = 0,
        correctPrediction_acc = 0, 
        correctPrediction_Good = 0, 
        correctPrediction_vGood = 0;

    for(Car c: test) {
        if(c.classValue == "unacc") {
            test_unacc_count++;
            if(decisionTree.predict(c) == c.classValue) correctPrediction_unacc++;
        } else if(c.classValue == "acc") {
            test_acc_count++;
            if(decisionTree.predict(c) == c.classValue) correctPrediction_acc++;
        } else if(c.classValue == "good") {
            test_good_count++;
            if(decisionTree.predict(c) == c.classValue) correctPrediction_Good++;
        }
        else if(c.classValue == "vgood") {
            test_vGood_count++;
            if(decisionTree.predict(c) == c.classValue) correctPrediction_vGood++;
        }

    }

    cout << "\nClass\tTrain\tTest\tAccuracy" << endl;
    cout << "unacc\t" << train_unacc_count << "\t\t" << test_unacc_count << "\t\t" << (correctPrediction_unacc*100.0/test_unacc_count) << "%" << endl;
    cout << "acc\t\t" << train_acc_count << "\t\t" << test_acc_count << "\t\t" << (correctPrediction_acc*100.0/test_acc_count) << "%" << endl;
    cout << "good\t" << train_good_count << "\t\t" << test_good_count << "\t\t" << (correctPrediction_Good*100.0/test_good_count) << "%" << endl;
    cout << "vGood\t" << train_vGood_count << "\t\t" << test_vGood_count << "\t\t" << (correctPrediction_vGood*100.0/test_vGood_count) << "%" << endl;


    cout << endl;

    int accurate = correctPrediction_unacc +
                    correctPrediction_acc +
                    correctPrediction_Good +
                    correctPrediction_vGood;



    double accuracy = accurate*100.0/test.size();

    cout << "Total Accuracy = " << accuracy << " % \n\n" << endl;

    return accuracy;
}



int main() {

    ifstream file("./car_dataset/car.data");

    if (!file.is_open()) {
        cerr << "Failed to open the file: " << endl;
        return 1;
    }

    ofstream outputFile("report.txt");

    //  ofstream outputFile2("tree.txt");

    if (!outputFile.is_open()) {
        cerr << "Failed to open the output file." << endl;
        return 1;
    }
    // Redirect cout to the output file

    streambuf* coutBuf = cout.rdbuf();
    cout.rdbuf(outputFile.rdbuf());


    // Define variables to store data
    string col1, col2, col3, col4, col5, col6, col7;
    vector<Car> car_data;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        if (getline(iss, col1, ',') &&
            getline(iss, col2, ',') &&
            getline(iss, col3, ',') &&
            getline(iss, col4, ',') &&
            getline(iss, col5, ',') &&
            getline(iss, col6, ',') &&
            getline(iss, col7, ',') ) {

            Car car(col1, col2, col3, col4, col5, col6, col7);

            car_data.push_back(car);

            
        } else {
            cerr << "Error parsing line: " << line << endl;
        }
    }

    // Close the input file
    file.close();

    // decisionTree.printTree(); 


    vector<double> accuracyList;
    double acc, sum = 0 ;

    for(int i =1; i <= 20; i++) {
        acc = runTest(car_data, i); 
        accuracyList.push_back(acc);
        sum += acc;
    }

    //avaerage accuracy

    double average = sum / accuracyList.size();

    // Calculate the standard deviation
    double variance = 0.0;
    for (double value : accuracyList) {
        double diff = value - average;
        variance += diff * diff;
    }

    variance /= accuracyList.size();

    double standardDeviation = sqrt(variance);

    cout << endl;

    cout << "Average accuracy: " << average << " % " << endl;
    cout << "Standard Deviation: " << standardDeviation << endl;
    

    cout.rdbuf(coutBuf);

    // Close the output file
    outputFile.close();


    return 0;
}
