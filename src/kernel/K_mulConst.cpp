 #include "rns.h"
 #include "mod.h"
 #include "ap_int.h"
 #include "ap_fixed.h"
 #include <iostream>
 using namespace std;
 extern "C" {

 void K_mulConst(ap_int<PRIME_BIT> *in,
                 ap_int<PRIME_BIT> *out,
                 int constant,
                 int inv)
 {
 #pragma HLS INTERFACE m_axi port=in offset=slave
 #pragma HLS INTERFACE m_axi port=out offset=slave
 #pragma HLS INTERFACE s_axilite port = return bundle = control
 #pragma HLS INTERFACE s_axilite port = in bundle = control
 #pragma HLS INTERFACE s_axilite port = out bundle = control
 #pragma HLS INTERFACE s_axilite port = inv bundle = control
 #pragma HLS INTERFACE s_axilite port = constant bundle = control

   for (ap_uint<LOG_RNS+LOG_N+1> i = 0; i < RNS_NUM*N; i++) {
   #pragma HLS PIPELINE
     ap_int<LOG_RNS+1> idx = (i>>LOG_N) + RNS_NUM;
     ap_int<LOG_RNS+1> idx2 = (i>>LOG_N);
     ap_int<LOG_RNS+1> idx3 = (inv==1) ? idx : idx2;
     out[i] = modular_multiplication(in[i], constant, MOD[idx3], idx3);
   }

   return;
 }

 }
