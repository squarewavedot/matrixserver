#ifndef __GENETIC_H__
#define __GENETIC_H__

#include "MatrixApplication.h"

class Genetic : public MatrixApplication{
public:
  Genetic();
  bool loop();
  ~Genetic();
  static int rnd (int i) { return rand() % i; }

private:
  class citizen {
    public:
      citizen();
      citizen(int chrom);
      int dna;
  };

  class comparer {
    public:
      comparer(int t);
      inline bool operator() (const citizen& c1, const citizen& c2);
    private:
      const int target_;
  };

  static int R(const int cit) { return at(cit, 16); }
  static int G(const int cit) { return at(cit, 8); }
  static int B(const int cit) { return at(cit, 0); }
  static int at(const int v, const  int offset) { return (v >> offset) & 0xFF; }
  static int calcFitness(const int value, const int target);

  void sort();
  void mate();
  void swap();
  void mutate(citizen& c);
  bool is85PercentFit();

  static const int bitsPerPixel = 24;
  int popSize_;
  int width_, height_;
  int target_;
  citizen* children_;
  citizen* parents_;
};

#endif
