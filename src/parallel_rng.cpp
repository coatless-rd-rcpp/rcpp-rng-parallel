#include <Rcpp.h>
#include <xoshiro.h>
#include <dqrng_distribution.h>

#ifdef _OPENMP
#include <omp.h>
#endif

//' Maximum Number of OpenMP Threads
//'
//' Reports the number of threads _OpenMP_ would use on this machine.
//'
//' @return
//' An `integer` with the maximum number of _OpenMP_ threads, or `0` when the
//' package was built without _OpenMP_ support.
//'
//' @export
//' @examples
//' max_cores()
// [[Rcpp::export]]
int max_cores() {
  int n_cores = 0;
#ifdef _OPENMP
  n_cores = omp_get_max_threads();
#endif
  return n_cores;
}

//' Draw Uniform Variates in Parallel with Independent Streams
//'
//' Samples from a uniform distribution in parallel, giving each thread its own
//' independent substream of a single seeded `dqrng` generator so that no two
//' threads ever produce the same values.
//'
//' @param n       Number of values to generate.
//' @param min     Lower bound of the continuous uniform distribution.
//' @param max     Upper bound of the continuous uniform distribution.
//' @param seed    Seed for the base random number generator.
//' @param n_cores Number of threads to use.
//'
//' @return
//' A `numeric` vector of length `n`.
//'
//' @details
//' A single `dqrng::xoshiro256plus` generator is seeded once. Inside the
//' parallel region each thread copies that generator and advances it with
//' `long_jump()` by an amount unique to the thread, which lands it on a
//' non-overlapping substream. The result is reproducible for a fixed `seed`
//' and `n_cores`. This is the correction to the naive approach of giving each
//' thread a default-constructed engine, which would leave every thread drawing
//' the same sequence.
//'
//' @export
//' @examples
//' # 100 draws across 2 threads, reproducible for this seed and core count.
//' draw_numeric_unif(100, n_cores = 2, seed = 42)
// [[Rcpp::export]]
std::vector<double> draw_numeric_unif(unsigned int n,
                                      double min = 0.0, double max = 1.0,
                                      int seed = 42, int n_cores = 1) {
  std::vector<double> sample_values(n);

  // OpenMP requires a positive thread count.
  if (n_cores < 1) n_cores = 1;

  // Base generator, seeded once on the host.
  dqrng::xoshiro256plus base(seed);

#ifdef _OPENMP
  #pragma omp parallel num_threads(n_cores)
  {
    // Each thread copies the base generator and jumps to an independent,
    // non-overlapping substream so the threads never overlap.
    dqrng::xoshiro256plus eng(base);
    eng.long_jump(omp_get_thread_num() + 1);

    dqrng::uniform_distribution dist(min, max);

    // schedule(static) fixes the index-to-thread partition, which keeps the
    // result reproducible for a given seed and n_cores.
#pragma omp for schedule(static)
    for (unsigned int i = 0; i < n; ++i) {
      sample_values[i] = dist(eng);
    }
  }
#else
  // Without OpenMP the draws come from the single base stream.
  dqrng::xoshiro256plus eng(base);
  dqrng::uniform_distribution dist(min, max);
  for (unsigned int i = 0; i < n; ++i) {
    sample_values[i] = dist(eng);
  }
#endif

  return sample_values;
}
