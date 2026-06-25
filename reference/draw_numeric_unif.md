# Draw Uniform Variates in Parallel with Independent Streams

Samples from a uniform distribution in parallel, giving each thread its
own independent substream of a single seeded `dqrng` generator so that
no two threads ever produce the same values.

## Usage

``` r
draw_numeric_unif(n, min = 0, max = 1, seed = 42L, n_cores = 1L)
```

## Arguments

- n:

  Number of values to generate.

- min:

  Lower bound of the continuous uniform distribution.

- max:

  Upper bound of the continuous uniform distribution.

- seed:

  Seed for the base random number generator.

- n_cores:

  Number of threads to use.

## Value

A `numeric` vector of length `n`.

## Details

A single `dqrng::xoshiro256plus` generator is seeded once. Inside the
parallel region each thread copies that generator and advances it with
`long_jump()` by an amount unique to the thread, which lands it on a
non-overlapping substream. The result is reproducible for a fixed `seed`
and `n_cores`. This is the correction to the naive approach of giving
each thread a default-constructed engine, which would leave every thread
drawing the same sequence.

## Examples

``` r
# 100 draws across 2 threads, reproducible for this seed and core count.
draw_numeric_unif(100, n_cores = 2, seed = 42)
#>   [1] 0.966337669 0.512797474 0.388963575 0.330519475 0.909714266 0.484384019
#>   [7] 0.655726219 0.966602790 0.518599363 0.094164281 0.574735354 0.196601129
#>  [13] 0.373704486 0.123933586 0.361712373 0.232795668 0.194726707 0.826175015
#>  [19] 0.980958089 0.465201694 0.007771581 0.659064368 0.051389524 0.953510596
#>  [25] 0.732810624 0.913746829 0.847316514 0.072982526 0.302433092 0.475226722
#>  [31] 0.670925751 0.736861956 0.166981771 0.196811404 0.861496618 0.563813554
#>  [37] 0.709291685 0.189504673 0.331382286 0.166517416 0.468163827 0.325096325
#>  [43] 0.218440414 0.258732814 0.482267776 0.163179775 0.926905712 0.311584187
#>  [49] 0.875149063 0.118324742 0.892224804 0.716552621 0.423707415 0.770319368
#>  [55] 0.199280463 0.895623128 0.877968114 0.623429491 0.883536166 0.828526876
#>  [61] 0.374753864 0.954003468 0.919365729 0.142265364 0.281290817 0.516340762
#>  [67] 0.288173096 0.096624052 0.429219518 0.012971484 0.218636045 0.482743250
#>  [73] 0.884296949 0.103601514 0.086059660 0.648861555 0.624911683 0.426023720
#>  [79] 0.865357103 0.456115615 0.840190723 0.572121227 0.328262300 0.033459442
#>  [85] 0.857076993 0.051306369 0.395496300 0.343454547 0.861326195 0.710390269
#>  [91] 0.777668924 0.159438201 0.097529711 0.187290595 0.080270375 0.543232023
#>  [97] 0.489635702 0.719278144 0.181086150 0.158004139
```
