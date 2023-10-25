#include "sdsl/int_vector.hpp"
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

#define VERBOSE

#include <common.hpp>
#include <subsampling_sa.hpp>

#include <lazy_lce.hpp>

int main(int argc, char *const argv[]) {
  Args args;
  parseArgs(argc, argv, args);

  // Subsample the samples of the run starts

  std::chrono::high_resolution_clock::time_point t_insert_start =
      std::chrono::high_resolution_clock::now();

  sdsl::int_vector<> samples_start;
  read_samples(args.filename + ".ssa", samples_start);
  sdsl::int_vector<> samples_end;
  read_samples(args.filename + ".esa", samples_end);

  ms_pointers<PlainSlp<var_t, Fblc, Fblc>> ms;

  ifstream in(args.filename + "." + to_string(args.maxLF) + ".aug");
  ms.load(in, args.filename, args.maxLF);

  for (size_t i = 0; i < samples_end.size(); i++) {
    if (!(ms.subsamples_last(i) == samples_end[i])) {
      cout << "run " << i << " : " << ms.subsamples_last(i) << endl;
      cout << "run " << i << " : " << samples_end[i] << endl;
      throw std::runtime_error("samples_last not reconstructed correctly");
      exit(1);
    }
  }
  for (size_t i = 0; i < samples_start.size(); i++) {
    if (!(ms.subsamples_start(i) == samples_start[i])) {
      cout << "run " << i << " : " << ms.subsamples_start(i) << endl;
      cout << "run " << i << " : " << samples_start[i] << endl;
      throw std::runtime_error("samples_start not reconstructed correctly");
      exit(1);
    }
  }

  verbose("Samples can be reconstructed correctly from subsamples.");

  std::chrono::high_resolution_clock::time_point t_insert_end =
      std::chrono::high_resolution_clock::now();

  verbose("Verifying subsampling complete");
  verbose("Memory peak: ", malloc_count_peak());
  verbose("Elapsed time (s): ", std::chrono::duration<double, std::ratio<1>>(
                                    t_insert_end - t_insert_start)
                                    .count());
}