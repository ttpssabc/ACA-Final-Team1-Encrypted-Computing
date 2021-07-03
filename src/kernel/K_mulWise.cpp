 #include "rns.h"
 #include "mod.h"
 #include "ap_int.h"
 #include "ap_fixed.h"
 #include <iostream>
 using namespace std;
 extern "C" {

 void K_mulWise(ap_int<PRIME_BIT> *poly1, ap_int<PRIME_BIT> *poly2, ap_int<PRIME_BIT> *out, int inv) {
     #pragma HLS INTERFACE m_axi port=poly1 offset=slave
     #pragma HLS INTERFACE m_axi port=poly2 offset=slave
     #pragma HLS INTERFACE m_axi port=out offset=slave
     #pragma HLS INTERFACE s_axilite port = poly1 bundle = control
     #pragma HLS INTERFACE s_axilite port = poly2 bundle = control
     #pragma HLS INTERFACE s_axilite port = out bundle = control
     #pragma HLS INTERFACE s_axilite port = inv bundle = control
     #pragma HLS INTERFACE s_axilite port = return bundle = control

   for (ap_uint<LOG_RNS+LOG_N+1> i = 0; i < RNS_NUM*N; i++) {
       #pragma HLS PIPELINE
     ap_int<LOG_RNS+1> idx = (i>>LOG_N) + RNS_NUM;
     ap_int<LOG_RNS+1> idx2 = (i>>LOG_N);
     ap_int<LOG_RNS+1> idx3 = (inv==1) ? idx : idx2;
     out[i] = modular_multiplication(poly1[i], poly2[i], MOD[idx3], idx3);
   }

   return;
 }
 }
