#include<vector>
#include<string>
#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

// want to represents vocab items by integers because then various tables 
// need by the IBM model and EM training can just be represented as 2-dim 
// tables indexed by integers

// the following #defines, defs of VS, VO, S, O, and create_vocab_and_data()
// are set up to deal with the specific case of the two pair corpus
// (la maison/the house)
// (la fleur/the flower)

// S VOCAB
#define LA 0
#define MAISON 1
#define FLEUR 2
// O VOCAB
#define THE 0
#define HOUSE 1
#define FLOWER 2

#define VS_SIZE 3
#define VO_SIZE 3
#define D_SIZE 2
#define ITERATIONS 50 // 50 Iterations used to be the same as the hw_answer_output file.

vector<string> VS(VS_SIZE); // S vocab: VS[x] gives Src word coded by x 
vector<string> VO(VO_SIZE); // O vocab: VO[x] gives Obs word coded by x

vector<vector<int> > S(D_SIZE); // all S sequences; in this case 2
vector<vector<int> > O(D_SIZE); // all O sequences; in this case 2

// sets S[0] and S[1] to be the int vecs representing the S sequences
// sets O[0] and O[1] to be the int vecs representing the O sequences
void create_vocab_and_data(); 

// functions which use VS and VO to 'decode' the int vecs representing the 
// Src and Obs sequences
void show_pair(int d);
void show_O(int d); 
void show_S(int d);

int main() {
  double unnormalisedCounts[VO_SIZE][VS_SIZE];
  double translationProbability[VO_SIZE][VS_SIZE];
  create_vocab_and_data();
  // The process should kick off with a uniform initialisation of tr(o|s) translation probabilities which is 1/3.
  for(int a=0; a<VO_SIZE; a++) {
    for(int b=0; b<VS_SIZE; b++) {
      translationProbability[a][b]=((1.0)/(3.0));
    }
  }

  // Initial Translation probability printed at the beginning.
  cout << "initial trans probs are\n";
  for(int a=0; a<VS_SIZE; a++) {
    for(int b=0; b<VO_SIZE; b++) {
      cout << VO[b] << " " << VS[a] << " " << translationProbability[b][a] << "\n";
    }
  }

  // Go through all iterations, printing the unnormalised counts and the resulting translation probability
  for(int iteration=0; iteration<ITERATIONS; iteration++) {
    std::vector<int> srcWords;
    std::vector<int> obsWords;

    // Set unnormalised count indexes to 0
    for(int a=0; a<VO_SIZE; a++) {
      for(int b=0; b<VS_SIZE; b++) {
        unnormalisedCounts[a][b] = 0.0;
      }
    }
    // Iterating through D_SIZE
    for(int a=0; a<D_SIZE; a++) {
      srcWords = S[a];
      obsWords = O[a];
      // Iterating through the vector of Obs words
      for(int b=0; b<obsWords.size(); b++) {
        double obsWordProb = 0.0;
        int obsWord = obsWords[b];
        // Iterating through the vector of Src words
        for(int c=0; c<srcWords.size(); c++) {
          int srcWord = srcWords[c];
          //Get probability of Obs word
          obsWordProb = translationProbability[obsWord][srcWord] + obsWordProb;
        }
        // Iterating through the vector of Src words
        for(int d=0; d<srcWords.size(); d++) {
          int srcWord = srcWords[d];
          //If translation probability is greater than 0, then calculate index for unnormalisedCounts vector.
          if (translationProbability[obsWord][srcWord] > 0) {
            unnormalisedCounts[obsWord][srcWord] = (translationProbability[obsWord][srcWord] / obsWordProb) + unnormalisedCounts[obsWord][srcWord];
          }
        }
      }
    }

    // Iterating through VS_SIZE
    for(int a=0; a<VS_SIZE; a++) {
      double normalisedCount = 0.0;
      // Iterating through VO_SIZE
      for(int b=0; b<VO_SIZE; b++) {
        normalisedCount = unnormalisedCounts[b][a] + normalisedCount;
      }
      if(normalisedCount < 0){
        // Iterating through VO_SIZE
        for(int c=0; c<VO_SIZE; c++){
          translationProbability[c][a] = normalisedCount;
        }
      }
      else {
        // normalisedCount is greater than 0 so we iterate through VO_SIZE and calculate index for translationProbability
        for(int d=0; d<VO_SIZE; d++) {
          translationProbability[d][a] = ((unnormalisedCounts[d][a])/normalisedCount);
        }
      }
    }

    // Print necessary outputs to simulate the file "hw_answer_output" given with the assignment.
    cout << "unnormalised counts in iteration " << iteration << " are" << "\n";
    for(int a=0; a<VS_SIZE; a++) {
      for(int b=0; b<VO_SIZE; b++) {
        cout << VO[b] << " " << VS[a] << " " << unnormalisedCounts[b][a] << "\n";
      }
    }

    cout << "after iteration " << iteration << " trans probs are" << "\n";
    for(int a=0; a<VS_SIZE; a++) {
      for(int b=0; b<VO_SIZE; b++) {
        cout << VO[b] << " " << VS[a] << " " << translationProbability[b][a] << "\n";
      }
    }
  }
}

void create_vocab_and_data() {

  VS[LA] = "la";
  VS[MAISON] = "maison";
  VS[FLEUR] = "fleur";

  VO[THE] = "the";
  VO[HOUSE] = "house";
  VO[FLOWER] = "flower";

  cout << "source vocab\n";
  for(int vi=0; vi < VS.size(); vi++) {
    cout << VS[vi] << " ";
  }
  cout << endl;
  cout << "observed vocab\n";
  for(int vj=0; vj < VO.size(); vj++) {
    cout << VO[vj] << " ";
  }
  cout << endl;

  // make S[0] be {LA,MAISON}
  //      O[0] be {THE,HOUSE}
  S[0] = {LA,MAISON};
  O[0] = {THE,HOUSE};

  // make S[1] be {LA,FLEUR}
  //      O[1] be {THE,FLOWER}
  S[1] = {LA,FLEUR};
  O[1] = {THE,FLOWER};

  for(int d = 0; d < S.size(); d++) {
    show_pair(d);
  }
}

void show_O(int d) {
  for(int i=0; i < O[d].size(); i++) {
    cout << VO[O[d][i]] << " ";
  }
}

void show_S(int d) {
  for(int i=0; i < S[d].size(); i++) {
    cout << VS[S[d][i]] << " ";
  }
}

void show_pair(int d) {
  cout << "S" << d << ": ";
  show_S(d);
  cout << endl;
  cout << "O" << d << ": ";
  show_O(d);
  cout << endl;
}