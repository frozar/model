// Source code of AKS primality test in aks.c.
#include <stdio.h>
#include <stdarg.h>
#include "gmp.h"
#include "maplec.h"
#define TRUE 1
#define FALSE 0
typedef unsigned long int uli_t;
typedef signed long int sli_t;
/* Global variables */
char *MAPLE_rslt;
/* Function prototypes */
static void M_DECL textCallBack(void *, int, char *);
void mpz_logbase2cl(sli_t *, mpz_t);
void mpz_logbase2fl(sli_t *, mpz_t);
void find_smallest_r(mpz_t, mpz_t);
int r_good(mpz_t, mpz_t, mpz_t);
int poly_eq_holds(MKernelVector, mpz_t, mpz_t, mpz_t);
int aks(mpz_t);
/* Function implementations */
/* MAPLE Callback used for directing result output */
static void M_DECL textCallBack( void *data, int tag, char *output )
{
  MAPLE_rslt = output;
}
/* rslt is always integral due to ceiling operation */
void mpz_logbase2cl(sli_t *rslt, mpz_t n)
{
  if (mpz_get_d_2exp(rslt, n) == 0.5) {
    --(*rslt);
  }
}
/* rslt is always integral due to floor operation */
void mpz_logbase2fl(sli_t *rslt, mpz_t n)
{
  if (mpz_get_d_2exp(rslt, n) == 0.5) {
    --(*rslt);
  }
  else {
    ++(*rslt);
  }
}
void find_smallest_r(mpz_t r, mpz_t n)
{
  /* Try out successive values of r and test if n^k != 1 (mod r) for every
   * k <= 4(log n)^2.
   */
  mpz_t max_k;
  mpz_t logn;
  mpz_t logn_sqrd;
  sli_t exp_cl;
  sli_t exp_fl;
  mpz_init(max_k);
  mpz_init(logn);
  mpz_init(logn_sqrd);
  /* Compute log n
   * Ceiling has been applied!
   */
  mpz_logbase2cl(&exp_cl, n);
  mpz_set_si(logn, exp_cl);
  /* log n is computed, compute 4 * (log n)^2
   * Compute (log n)^2
   */
  mpz_mul(logn_sqrd, logn, logn);
  /* Compute max_k */
  mpz_mul_si(max_k, logn_sqrd, 4);
  gmp_printf("max_k = %Zd\n", max_k);
  /* Now find the appropriate r:
   * r = 2;
   * while (1) {
   * if (r_good(r, max_k, n)) {
   * break;
   * }
   * r++;
   *}
   */
  mpz_set_ui(r, 2);
  while (1) {
    if (r_good(r, max_k, n)) {
      break;
    }
    mpz_add_ui(r, r, 1);
  }
}
int r_good(mpz_t r, mpz_t max_k, mpz_t n)
{
  mpz_t k;
  mpz_t mod_rslt;
  mpz_init(k);
  mpz_init(mod_rslt);
  mpz_set_ui(k, 1);
  /* while (k <= max_k) {
   * mod_rslt = n^k mod r;
   * if (mod_rslt == 1) {
   * return false;
   * }
   * k++;
   *}
   */
  while (mpz_cmp(k, max_k) < 0 || mpz_cmp(k, max_k) == 0) {
    mpz_powm(mod_rslt, n, k, r);
    if (mpz_cmp_ui(mod_rslt, 1) == 0) {
      return FALSE;
    }
    mpz_add_ui(k, k, 1);
  }
  return TRUE;
}
int poly_eq_holds(MKernelVector kv, mpz_t a, mpz_t n, mpz_t r)
{
  char *a_str;
  char *n_str;
  char *r_str;
  char *a_buf;
  char *n_buf;
  char *r_buf;
  size_t a_size;
  size_t n_size;
  size_t r_size;
  char maple_stmt[4092];
  int maple_sez;
  /* La..La..La..La.. */
  a_size = mpz_sizeinbase(a, 10)+2;
  r_size = mpz_sizeinbase(r, 10)+2;
  n_size = mpz_sizeinbase(n, 10)+2;
  a_buf = (char *) malloc (a_size*sizeof(char));
  r_buf = (char *) malloc (r_size*sizeof(char));
  n_buf = (char *) malloc (n_size*sizeof(char));
  a_str = mpz_get_str(a_buf, 10, a);
  n_str = mpz_get_str(r_buf, 10, n);
  r_str = mpz_get_str(n_buf, 10, r);
  /* Build up MAPLE statement */
  sprintf(maple_stmt,
    "if(powmod(x+%s,%s,x^%s-1,x) mod %s = "
    "powmod(x,%s,x^%s-1,x)+%s mod %s) then 1 "
    "else 0 end if;",
    a_str,n_str,r_str,n_str,
    n_str,r_str,a_str,n_str);
  printf("maple_stmt = %s\n", maple_stmt);
  /* Feed the statement to MAPLE kernel */
 EvalMapleStatement(kv, maple_stmt);
  /* To avoid memory leakage */
  free(a_buf);
  free(r_buf);
  free(n_buf);
  if (atoi(MAPLE_rslt) == 1) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
int aks(mpz_t n)
{
  mpz_t r;
  mpz_t a;
  mpz_t max_a;
  mpz_t gcd_rslt;
  mpz_t totient_r;
  mpf_t ftotient_r;
  mpf_t sqrt_rslt;
  mpf_t sqrt_rslt2;
  mpf_t temp;
  mpf_t temp2;
  sli_t logn;
   /* For the sake of maple kernel */
  int argc = 0;
  char **argv;
  char err[2048];
  mpz_init(r);
  mpz_init(a);
  mpz_init(max_a);
  mpz_init(gcd_rslt);
  mpz_init(totient_r);
  mpf_init(ftotient_r);
  mpf_init(sqrt_rslt);
  mpf_init(sqrt_rslt2);
  mpf_init(temp);
  mpf_init(temp2);
  /* 1. If (n = a^k for a in N and b > 1) output COMPOSITE */
  if (mpz_perfect_power_p(n) != 0) {
    printf("Step 1 detected composite\n");
    return FALSE;
  }
  /* 2. Find the smallest r such that or(n) > 4(log n)^2 */
  find_smallest_r(r, n);
  gmp_printf("good r seems to be %Zd\n", r);
/* 3. If 1 < gcd(a, n) < n for some a <= r, output COMPOSITE */
/* for (a = 1; a <= r; a++) {
 * gcd_rslt = gcd(a, n);
 * if (gcd_rslt > 1 && gcd_rslt < n) {
 * return FALSE;
 * }
 *}
 */
 for (mpz_set_ui(a, 1);
      mpz_cmp(a, r) < 0 || mpz_cmp(a, r) == 0;
      mpz_add_ui(a, a, 1)) {
    mpz_gcd(gcd_rslt, a, n);
    if (mpz_cmp_ui(gcd_rslt, 1) > 0 && mpz_cmp(gcd_rslt, n) < 0) {
      printf("Step 3 detected composite\n");
      return FALSE;
    }
 }
/* 4. If n <= r, output PRIME */
if (mpz_cmp(n, r) < 0 || mpz_cmp(n, r) == 0) {
  printf("Step 4 detected prime\n");
  return TRUE;
}
/* 5. For a = 1 to floor(2*sqrt(totient(r))*(log n)
 *      if ( (X+a)^n != X^n + a (mod X^r-1, n) ), output COMPOSITE
 *
 * Choices of implementation to evaluate the polynomial equality:
 * (1) Implement powermodreduce on polynomial ourselves (tough manly way)
 * (2) Use MAPLE (not so manly, but less painful)
 */
/* Compute totient(r), since r is prime, this is simply r-1 */
mpz_sub_ui(totient_r, r, 1);
/* Compute log n (ceilinged) */
mpz_logbase2cl(&logn, n);
/* Compute sqrt(totient(r)) */
mpf_set_z(ftotient_r, totient_r);
mpf_sqrt(sqrt_rslt, ftotient_r);
/* Compute 2*sqrt(totient(r)) */
mpf_mul_ui(sqrt_rslt2, sqrt_rslt, 2);
/* Compute 2*sqrt(totient(r))*(log n) */
mpf_set(temp, sqrt_rslt2);
mpf_set_si(temp2, logn);
mpf_mul(temp, temp, temp2);
  /* Finally, compute max_a, after lots of singing and dancing */
  mpf_floor(temp, temp);
  mpz_set_f(max_a, temp);
  gmp_printf("max_a = %Zd\n", max_a);
  /* Now evaluate the polynomial equality with the help of maple kernel */
  /* Set up maple kernel incantations */
  MKernelVector kv;
  MCallBackVectorDesc cb = { textCallBack,
                       0, /* errorCallBack not used */
                       0, /* statusCallBack not used */
                       0, /* readLineCallBack not used */
                       0, /* redirectCallBack not used */
                       0, /* streamCallBack not used */
                       0, /* queryInterrupt not used */
                       0 /* callBackCallBack not used */
                    };
  /* Initialize Maple */
  if( ( kv = StartMaple(argc, argv, &cb, NULL, NULL, err) ) == NULL ) {
      printf( "Could not start Maple, %s\n", err );
      exit(666);
  }
  /* Here comes the complexity and bottleneck */
  /* for (a = 1; a <= max_a; a++) {
   * if (!poly_eq_holds(kv, a, n, r)) {
   *       return FALSE;
   * }
   *}
*/
  /* Make max_a only up to 5 */
  mpz_set_ui(max_a, 5);
  for (mpz_set_ui(a, 1);
       mpz_cmp(a, max_a) < 0 || mpz_cmp(a, max_a) == 0;
       mpz_add_ui(a, a, 1)) {
       if (!poly_eq_holds(kv, a, n, r)) {
         printf("Step 5 detected composite\n");
         return FALSE;
       }
  }
  /* 6. Output PRIME */
  printf("Step 6 detected prime\n");
  return TRUE;
}
int main()
{
  mpz_t n;
  mpz_init(n);
  printf("Enter a number: ");
  gmp_scanf("%Zd", &n);
  printf("AKS says: %s\n", aks(n)?"prime":"composite");
  return 0;
}
