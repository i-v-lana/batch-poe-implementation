# Programmer Documentation

## 1. `Wesolowski`
- **Description**: Base implementation of the [Wesolowski protocol](https://eprint.iacr.org/2018/623)
- **Methods**:
  - `setup(int k, const mpz_t& _N, long long _t)`: Initializes the protocol with parameters.
    - `k`: Security parameter, determines the bit length of the prime used in the protocol.
    - `_N`: Modulus N = p*q.
    - `_t`: The parameter for the exponent: x^(2^_t) =? y
  - `prover_trapdoor(mpz_t l, mpz_t pi, const mpz_t x, const mpz_t phi_N)`: Computes the prover's part using a trapdoor (i.e. the factorization of N = p*q).
    - `l`: Output parameter for the generated prime.
    - `pi`: Output parameter for the proof.
    - `x`: Input parameter.
    - `phi_N`: Euler's totient function of N.
  - `prover(mpz_t l, mpz_t pi, const mpz_t x)`: Computes the prover's part.
    - `l`: Output parameter for the generated prime.
    - `pi`: Output parameter for the proof.
    - `x`: Input parameter.
  - `verifier(mpz_t x, mpz_t y_check, mpz_t l, mpz_t pi)`: Computes the verifier's part.
    - `x`: Input parameter.
    - `y_check`: Input parameter representing the claimed result.
    - `l`: Input parameter representing the generated prime.
    - `pi`: Input parameter representing the proof.

## 2. `Batching`
- **Description**: Base class for batching protocols. Represents [the exponent batching](https://eprint.iacr.org/2021/1209) approach from Lior Rotem's paper
- **Methods**:
  - `batch()`: Chooses a random exponent `alpha_i` for every instance, then calculates the product of `x_i^alpha_i`, and the product `y_i^alpha_i`, and finally runs the Wesolowski protocol for the batched instance.

## 3. `NaiveApproach`
- **Description**: Implements the naive batching approach, where each instance is batched individually.
- **Methods**:
  - `batch()`: Runs the naive batching protocol.
    - Batches each instance individually and verifies them one by one (by running the Wesolowski protocol for every instance).

## 4. `SubsetBatching`
- **Description**: Implements the subset batching approach, where instances are batched together based on randomly selected subsets.
- **Methods**:
  - `batch(int m)`: Runs the subset batching protocol.
    - `m`: Number of random subsets.
    - This method selects random subsets of instances and batches them together.

## 5. `HybridBatching`
- **Description**: Implements the hybrid batching approach, which combines exponent and subset batching methods.
- **Methods**:
  - `batch(int m)`: Runs the hybrid batching protocol.
    - `m`: Number of subsets.
    - This method first selects `m` random subsets, for each subset it calculates the batched instance, and then in runs the exponent approach to batch these `m` instances.

## 6. `BucketBatching`
- **Description**: Implements the bucket batching approach.
- **Methods**:
  - `batch(int _bucket_bit)`: Runs the bucket batching protocol.
    - `_bucket_bit`: Log2 of the number of buckets.
    - Puts every instance into the bucket (selected for it based on the PRF), then calculates the product of `x`-es and the product `y`-s for every bucket, and batches the final instances together using the exponent batching aporoach.

## 7. `PRF_crypto`
- **Description**: Implements a pseudo-random function using AES encryption.
- **Methods**:
  - `evaluate(bigint& x)`: Evaluates the PRF.
  - `decrypt(bigint& _cipher)`: Decrypts the PRF output.
  - `length()`: Returns the output length of the PRF.

## 8. `mpz_helper`
- **Description**: Provides helper functions for working with GMP big integers.
- **Methods**:
  - `get_random(int &bit_cnt)`: Generates a random big integer.
  - `generate_prime(int bit_cnt)`: Generates a random prime big integer.
  - `pow(bigint &x, bigint &p, bigint &mod)`: Computes x^p modulo mod.
  - `mul_mod(bigint &a, bigint &b, bigint &mod)`: Computes a * b modulo mod.
  - `get_random_mod(bigint &mod)`: Generates a random big integer modulo mod.
  - `gcd(bigint& a, bigint& b)`: Computes the greatest common divisor of a and b.
