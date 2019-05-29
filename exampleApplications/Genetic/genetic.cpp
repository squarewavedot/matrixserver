#include "genetic.h"
//general
#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <random>

Genetic::Genetic() : MatrixApplication(40){
    width_ = 64;
    height_ = 64;
    popSize_ = width_ * height_;

    // Allocate memory
    children_ = new citizen[popSize_];
    parents_ = new citizen[popSize_];
    srand(time(NULL));

    // Set a random target_
    target_ = rand() & 0xFFFFFF;

    // Create the first generation of random children_
    for (int i = 0; i < popSize_; ++i) {
      children_[i].dna = rand() & 0xFFFFFF;
    }
}

bool Genetic::loop(){
  static long loopcount = 0;
  if(loopcount%2 == 0){
    swap();
    sort();
    mate();
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle (children_, children_ + popSize_, g);

    // Draw citizens to canvas
    for(int i=0; i < popSize_; i++) {
      int c = children_[i].dna;
      int x = i % width_;
      int y = (int)(i / width_);
      for(auto screen : screens){
        screen->setPixel(x, y, R(c), G(c), B(c));
      }
    }

    // When we reach the 85% fitness threshold...
    if(is85PercentFit()) {
      // ...set a new random target_
      target_ = rand() & 0xFFFFFF;

      // Randomly mutate everyone for sake of new colors
      for (int i = 0; i < popSize_; ++i) {
        mutate(children_[i]);
      }
    }
  }

  loopcount++;
  return true;
}

Genetic::~Genetic() {
  delete [] children_;
  delete [] parents_;
}


  /// sort by fitness so the most fit citizens are at the top of parents_
  /// this is to establish an elite population of greatest fitness
  /// the most fit members and some others are allowed to reproduce
  /// to the next generation
  void Genetic::sort() {
    std::sort(parents_, parents_ + popSize_, comparer(target_));
  }

  /// let the elites continue to the next generation children
  /// randomly select 2 parents of (near)elite fitness and determine
  /// how they will mate. after mating, randomly mutate citizens
  void Genetic::mate() {
    // Adjust these for fun and profit
    const float eliteRate = 0.30f;
    const float mutationRate = 0.20f;

    const int numElite = popSize_ * eliteRate;
    for (int i = 0; i < numElite; ++i) {
      children_[i] = parents_[i];
    }

    for (int i = numElite; i < popSize_; ++i) {
      //select the parents randomly
      const float sexuallyActive = 1.0 - eliteRate;
      const int p1 = rand() % (int)(popSize_ * sexuallyActive);
      const int p2 = rand() % (int)(popSize_ * sexuallyActive);
      const unsigned matingMask = (~0u) << (rand() % bitsPerPixel);

      // Make a baby
      unsigned baby = (parents_[p1].dna & matingMask)
        | (parents_[p2].dna & ~matingMask);
      children_[i].dna = baby;

      // Mutate randomly based on mutation rate
      if ((rand() / (float)RAND_MAX) < mutationRate) {
        mutate(children_[i]);
      }
    }
  }

  /// parents make children,
  /// children become parents,
  /// and they make children...
  void Genetic::swap() {
    citizen* temp = parents_;
    parents_ = children_;
    children_ = temp;
  }

  void Genetic::mutate(citizen& c) {
    // Flip a random bit
    c.dna ^= 1 << (rand() % bitsPerPixel);
  }

  /// can adjust this threshold to make transition to new target seamless
  bool Genetic::is85PercentFit() {
    int numFit = 0;
    for (int i = 0; i < popSize_; ++i) {
      if (calcFitness(children_[i].dna, target_) < 1) {
        ++numFit;
      }
    }
    return ((numFit / (float)popSize_) > 0.85f);
  }

  int Genetic::calcFitness(const int value, const int target) {
    // Count the number of differing bits
    int diffBits = 0;
    for (unsigned int diff = value ^ target; diff; diff &= diff - 1) {
      ++diffBits;
    }
    return diffBits;
  }


Genetic::citizen::citizen() { }

Genetic::citizen::citizen(int chrom) : dna(chrom) {
}

Genetic::comparer::comparer(int t) : target_(t) {

}

inline bool Genetic::comparer::operator() (const citizen& c1, const citizen& c2) {
  return (calcFitness(c1.dna, target_) < calcFitness(c2.dna, target_));
}
