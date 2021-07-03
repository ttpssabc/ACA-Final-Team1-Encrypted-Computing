#include <math.h>
#include <stdlib.h>
#include "help_functions.h"
#include "model_weight.h"
#include <vector>
#include <iostream>
#include "global.h"

using namespace std;

#define N 8192
/*
volatile int *rlk,        //     [4][7][2][8][8192]
volatile int *gk,         // [4][25][7][2][8][8192]
volatile int *ct,         //    [25][4][2][8][8192]
volatile int *dense100    //       [13][4][8][8192]
volatile int *dense10     //           [4][8][8192]
volatile int *mask        //           [4][8][8192]
volatile int *bias100     //           [4][8][8192]
volatile int cnn_constant // 

cl_mem cnn_weight[4];
for i in range(4):
	cnn_weight[i] = clCreateBuffer(Context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 8*8192, const_cast<int *>(cnn_weight_ptr), &errCode);

cl_mem gk[4][25][7][2];

// HLS 
void K_NTT(volatile int *poly, volatile int *out, int mode, bool idx, int which);
void K_mulWise(volatile int *poly1, volatile int *poly2, volatile int *out);
void K_mulConst(volatile int *in, volatile int *out, int constant);
void K_add(volatile int *poly1, volatile int *poly2, volatile int *out);
void K_lift(volatile int *poly, volatile int *out, bool mode);
void K_scale(volatile int *poly1, volatile int *poly2, volatile int *out);
void K_mov(volatile int *in, volatile int *out);
void K_apply_galois(volatile int *in, int r, volatile int *out);
*/

#define FORWARD_7  0
#define INVERSE_7  1
#define FORWARD_8  2
#define INVERSE_8  3
#define SPECIAL    4
#define LIFT_TO_P  0
#define LIFT_TO_Q  1

// cl_mem temp1[2];
// cl_mem temp2[2];
// cl_mem temp3[4];
// cl_mem temp4[4];
// cl_mem temp5[2];
// cl_mem temp6[7];
// cl_mem temp7[7];
// cl_mem temp8[7];
// cl_mem temp9[2];
// cl_mem temp10[2];
// cl_mem temp_result[5][25][4][2];


int power(long long x, unsigned int y, int p)
{
    int res = 1; 
    x = x % p; 
    if (x == 0) return 0; 
    while (y > 0)
    {
        if (y & 1)
            res = (res*x) % p;
        y = y>>1; 
        x = (x*x) % p;
    }
    return res;
}

void keyswitching(cl_mem ct2,
                  cl_mem k[7][2],
                  cl_mem out[2])
{
    for (int i = 0; i < 7; i++) {
        do_k_ntt(ct2, temp6[i], SPECIAL, i);
        do_k_mulWise(temp6[i], k[i][0], temp7[i], 0);
        do_k_mulWise(temp6[i], k[i][1], temp8[i], 0);
    }
    for (int i = 1; i < 7; i++) {
        do_k_add(temp7[0], temp7[i], temp7[0], 0);
        do_k_add(temp8[0], temp8[i], temp8[0], 0);
    }
    do_k_mov(temp7[0], out[0]);
    do_k_mov(temp8[0], out[1]);
}

// HE primitives
void MulConst(cl_mem in[4][2],
              cl_mem out[4][2],
              int constant)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            do_k_mulConst(in[i][j], out[i][j], constant, 0);
        }
    }
}

void MulWise(cl_mem in1[4][2],
             cl_mem in2[4],
             cl_mem out[4][2])
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            do_k_mulWise(in1[i][j], in2[i], out[i][j], 0);
        }
    }
}

void HMul(cl_mem in1[4][2],
          cl_mem in2[4][2],
          cl_mem out[4][2]) 

{
    for (int i = 0; i < 4; i++) {
         do_k_ntt(in1[i][0], temp0[0], INVERSE_7, 0);
         do_k_ntt(in1[i][1], temp0[1], INVERSE_7, 0);
         do_k_ntt(in2[i][0], temp0[2], INVERSE_7, 0);
		 do_k_ntt(in2[i][1], temp0[3], INVERSE_7, 0);

         do_k_lift(temp0[0], temp1[0], LIFT_TO_P);
         do_k_lift(temp0[1], temp1[1], LIFT_TO_P);
         do_k_lift(temp0[2], temp2[0], LIFT_TO_P);
         do_k_lift(temp0[3], temp2[1], LIFT_TO_P);

         do_k_ntt(temp0[0], temp0[0], FORWARD_7, 0);
         do_k_ntt(temp0[1], temp0[1], FORWARD_7, 0);
         do_k_ntt(temp0[2], temp0[2], FORWARD_7, 0);
         do_k_ntt(temp0[3], temp0[3], FORWARD_7, 0);
         do_k_ntt(temp1[0], temp1[0], FORWARD_8, 0);
         do_k_ntt(temp1[1], temp1[1], FORWARD_8, 0);
         do_k_ntt(temp2[0], temp2[0], FORWARD_8, 0);
         do_k_ntt(temp2[1], temp2[1], FORWARD_8, 0);

        // Tensoring
        do_k_mulWise(temp0[0], temp0[2], temp3[0], 0);
        do_k_mulWise(temp0[1], temp0[2], temp3[1], 0);
        do_k_mulWise(temp0[0], temp0[3], temp3[3], 0);
        do_k_mulWise(temp0[1], temp0[3], temp3[2], 0);
        do_k_mulWise(temp1[0], temp2[0], temp4[0], 1);
        do_k_mulWise(temp1[1], temp2[0], temp4[1], 1);
        do_k_mulWise(temp1[0], temp2[1], temp4[3], 1);
        do_k_mulWise(temp1[1], temp2[1], temp4[2], 1);
        do_k_add(temp3[1], temp3[3], temp3[1], 0);
        do_k_add(temp4[1], temp4[3], temp4[1], 1);


         do_k_ntt(temp3[0], temp3[0], INVERSE_7, 0);
         do_k_ntt(temp3[1], temp3[1], INVERSE_7, 0);
         do_k_ntt(temp3[2], temp3[2], INVERSE_7, 0);

         do_k_ntt(temp4[0], temp4[0], INVERSE_8, 0);
         do_k_ntt(temp4[1], temp4[1], INVERSE_8, 0);
         do_k_ntt(temp4[2], temp4[2], INVERSE_8, 0);

         do_k_scale(temp3[0], temp4[0], temp4[0], i);
         do_k_scale(temp3[1], temp4[1], temp4[1], i);
         do_k_scale(temp3[2], temp4[2], temp4[2], i);

         do_k_lift(temp4[0], temp3[0], LIFT_TO_Q);
         do_k_lift(temp4[1], temp3[1], LIFT_TO_Q);
         do_k_lift(temp4[2], temp3[2], LIFT_TO_Q);
         // relin
         do_k_ntt(temp3[0], temp3[0], FORWARD_7, 0);
         do_k_ntt(temp3[1], temp3[1], FORWARD_7, 0);

         keyswitching(temp3[2], GM_rlk[i], temp5);
        do_k_add(temp3[0], temp5[0], out[i][0], 0);
        do_k_add(temp3[1], temp5[1], out[i][1], 0);
    }
}

void HAdd(cl_mem in1[4][2],
          cl_mem in2[4][2],
          cl_mem out[4][2])
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            do_k_add(in1[i][j], in2[i][j], out[i][j], 0);
        }
    }
}

void Move(cl_mem in[4][2],
          cl_mem out[4][2])
{
	for (int i = 0; i < 4; ++i) {
	  for (int j = 0; j < 2; ++j){
		do_k_mov(in[i][j], out[i][j]);
	  }
	}
}

void rotate_row(cl_mem in1[4][2],
                cl_mem out[4][2]) 
{
    for (int i = 0; i < 4; i++) {
        do_k_ntt(in1[i][0], in1[i][0], INVERSE_7, 0);
        do_k_ntt(in1[i][1], in1[i][1], INVERSE_7, 0);
        do_k_apply_galois(in1[i][0], 2*N-1, temp0[0]);
        do_k_apply_galois(in1[i][1], 2*N-1, temp0[1]);
        keyswitching(temp0[1], GM_gk[i][24], temp9);
        do_k_ntt(temp0[0], temp0[0], FORWARD_7, 0);
        do_k_add(temp0[0], temp9[0], out[i][0], 0);
        do_k_mov(temp9[1], out[i][1]);
    }
}

void rotate_column(cl_mem in1[4][2],
                   int r, // +- 1, 2, 4, ...
                   cl_mem out[4][2]) {
    vector<int> rot;
    vector<int> which_key;
    r = -r;
    while (r != 0) {
        double temp = log2(abs(r));
        int t = round(temp);
        int num = 1<<t;
        if (r < 0) {
            num = -num;
            t   = t + ((int)(log2(N))-1);
        }
        r -= num;
        if (num % (N/2) == 0)
            continue;
        num = (num + N/2) % (N/2);
        num = power(3,num,N);
        rot.push_back(num);
        which_key.push_back(t);
    }
    // cout << "Rotate: " << r << endl;
    for (int a = 0; a < rot.size(); a++) {
    	for (int i = 0; i < 4; i++) {
    		// cout << rot[a] << " " << which_key[a] << endl;
    		do_k_ntt(in1[i][0], in1[i][0], INVERSE_7, 0);
			do_k_ntt(in1[i][1], in1[i][1], INVERSE_7, 0);
			do_k_apply_galois(in1[i][0], rot[a], temp0[0]);
			do_k_apply_galois(in1[i][1], rot[a], temp0[1]);
			keyswitching(temp0[1], GM_gk[i][which_key[a]], temp9);
			do_k_ntt(temp0[0], temp0[0], FORWARD_7, 0);
			do_k_add(temp0[0], temp9[0], out[i][0], 0);
			do_k_mov(temp9[1], out[i][1]);

		}
    }
}

void CNN(cl_mem in1[25][4][2], cl_mem out[4][2]) { // temp
    for (int c = 0; c < 5; c++) {
        for (int i = 0; i < 25; i++) {
            MulConst(in1[i], temp_result[c][i], cnn_weight[c][i]);
        }
    }
     for (int c = 0; c < 5; c++)
         for (int i = 1; i < 25; i++)
             HAdd(temp_result[c][0], temp_result[c][i], temp_result[c][0]);
    
    for (int i = 1; i < 5; i++)
        rotate_column(temp_result[i][0], 169*i, temp_result[i][0]);
     for (int i = 1; i < 5; i++)
         HAdd(temp_result[0][0], temp_result[i][0], temp_result[0][0]);

    Move(temp_result[0][0], out);

}

void Square(cl_mem in1[4][2]) {
    HMul(in1, in1, in1);
}

// void Dense100(cl_mem in1[2]) { // rt_2[2], rt_4[2], rt_8[2], temp[13]
//     rotate_column(in1, 1024, rt_2);
//     HAdd(in1, rt_2, rt_2);
//     rotate_column(rt_2, 2048, rt_4);
//     HAdd(rt_2, rt_4, rt_4);
//     rotate_row(rt_4, 1, rt_8);
//     HAdd(rt_4, rt_8, rt_8);

//     for (int i = 0; i < 13; i++) {
//         MulWise(in1)
//     }
// }

// void Dense10() {
// }

// inference.cpp
void inference() 
{
    // clCreateCommandQueue(context, device_id, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
    CNN     (GM_ct, GM_ct[0]);
    // clEnqueueBarrierWithWaitList(Command_Queue, 0, NULL, NULL);
    // Square  (GM_ct);    
    // clEnqueueBarrierWithWaitList(Command_Queue, 0, NULL, NULL);
    // Dense100(GM_ct);
    // clEnqueueBarrierWithWaitList(Command_Queue, 0, NULL, NULL);
    // Square  (GM_ct);
    // clEnqueueBarrierWithWaitList(Command_Queue, 0, NULL, NULL);
    // Dense10 (GM_ct);
    // clEnqueueBarrierWithWaitList(Command_Queue, 0, NULL, NULL);
    // 先decrypt GM_ct[0];
}

void inference_test() 
{

// Kernel Level test
    // do_k_add( GM_ct[0][0][0],  GM_ct[0][0][0],  GM_ct[0][0][1], 1);
    // do_k_mulConst(GM_ct[0][0][0], GM_ct[0][0][1], 1<<17, 1);
    // do_k_mulWise( GM_ct[0][0][0],  GM_ct[0][0][0],  GM_ct[0][0][1], 1);
    //do_k_mov(GM_ct[0][0][0], GM_ct[0][0][1]);

//    do_k_ntt(         GM_ct[0][0][0], temp_result[0][0][0][0], 4, 4);
//    do_k_ntt(temp_result[0][0][0][0],          GM_ct[0][0][1], 1, 0);
//    do_k_mulInv(GM_ct[0][0][1], GM_ct[0][0][1], 0);

//	do_k_lift(GM_ct[0][0][0], temp_result[0][0][0][0], 0);
//	do_k_lift(temp_result[0][0][0][0], GM_ct[0][0][1], 1);

//	do_k_scale(GM_ct[0][0][0], GM_ct[0][0][0], GM_ct[0][0][1], 0);

// Function Level test
//	HAdd(GM_ct[0], GM_ct[1], GM_ct[0], 0);
//	MulWise(GM_ct[0], GM_dense100[0], GM_ct[0], 0);
//  MulConst(GM_ct[0], GM_ct[0], 1234);
//
//  int r = -128;
//	vector<int> rot;
//    vector<int> which_key;
//    r = -r;
//    while (r != 0) {
//        double temp = log2(abs(r));
//        int t = round(temp);
//        int num = 1<<t;
//        if (r < 0) {
//            num = -num;
//            t   = t + ((int)(log2(N))-1);
//        }
//        r -= num;
//        if (num % (N/2) == 0)
//            continue;
//        num = (num + N/2) % (N/2);
//        cout << "here " << num << ", " << t << ", "<< power(3, num, N) << endl;
//        num = power(3,num,N);
//        rot.push_back(num);
//        which_key.push_back(t);
//    }
//
//    for (int a = 0; a < rot.size(); a++) {
//    	cout << rot[a] << " " << which_key[a] << endl;
//    }
//	HMul(GM_ct[0], GM_ct[0], GM_ct[0]);
//	 rotate_row(GM_ct[0], GM_ct[0]);
//	 rotate_column(GM_ct[0], 119, GM_ct[0]);

}

//
