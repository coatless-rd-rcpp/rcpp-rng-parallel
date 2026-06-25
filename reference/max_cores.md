# Maximum Number of OpenMP Threads

Reports the number of threads *OpenMP* would use on this machine.

## Usage

``` r
max_cores()
```

## Value

An `integer` with the maximum number of *OpenMP* threads, or `0` when
the package was built without *OpenMP* support.

## Examples

``` r
max_cores()
#> [1] 4
```
