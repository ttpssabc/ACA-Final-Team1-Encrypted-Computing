# ACA-Final-Team1-Encrypted-Computing

## Introduction

- The project is encrypted computing, where the encrpyted data is computed
- We provide
	1. python client code to do encoding, encryption, decryption, decoding and 
	2. server host code and kernel code for receiving data from client and sending it into kernel accelerator to do homomorphic computation
- The implementation is referenced from [BFV-RNS](https://link.springer.com/chapter/10.1007/978-3-030-12612-4_5)
	- One should read it before getting into this project
  
## Major optimization

- We optimize individual kernel code used in `src/kernel` to achieve nearly the same performance as [this paper](https://ieeexplore.ieee.org/document/8675244) did using HLS
	- Lots of code restructure, array partition, unrolling, refer to the `docs/doc.txt` for further details
- We parametrized the code so that it can be easily tested under different settings
- We did not perform system level optimization and intend to do it as future work


## Folder hierarchy

```
├── LICENSE
├── README.md
├── build
│   └── README.md
├── docs
│   └── doc.txt
├── impl_result
│   ├── K_ntt_csynth.rpt
│   ├── NTT_csynth.rpt
│   ├── README.md
│   ├── add_sub_csynth.rpt
│   ├── butterfly_csynth.rpt
│   └── mul_mod_csynth.rpt
├── src
│   ├── client
│   │   ├── README.md                    // see the README.md to set up
│   │   ├── afterDense100_8192.txt
│   │   ├── afterSquare_8192.txt
│   │   ├── bfv_rns.py                   // BFV_RNS scheme python code
│   │   ├── bias100_8192.txt
│   │   ├── client.py                    // client code using BFV-RNS
│   │   ├── cnn8192.txt.zip
│   │   ├── ct.txt
│   │   ├── data.h
│   │   ├── data.py
│   │   ├── defs.py
│   │   ├── dense100_8192.txt
│   │   ├── dense10_8192.txt
│   │   ├── encrypt8192.txt
│   │   ├── gk.txt.partaa
│   │   ├── gk.txt.partab
│   │   ├── gk.txt.partac
│   │   ├── gk.txt.partad
│   │   ├── gk.txt.partae
│   │   ├── inference.py                  // reference code of how to write encrypted inference
│   │   ├── inference_multiprocess.py     // reference code of how to write encrypted inference
│   │   ├── mask8192.txt
│   │   ├── model8192.txt.zip
│   │   ├── rlk.txt
│   │   ├── test.txt.partaa
│   │   ├── test.txt.partab
│   │   ├── test.txt.partac
│   │   ├── test.txt.partad
│   │   ├── test.txt.partae
│   │   └── util.py
│   ├── host
│   │   ├── global.h
│   │   ├── help_functions.cpp
│   │   ├── help_functions.h
│   │   ├── host.cpp                      // Host code for dealing with networking and kernel interaction
│   │   ├── inference.cpp                 // HE primitives or even higher level code examples
│   │   ├── inference.h
│   │   └── model_weight.h
│   └── kernel                            // Kernel functions
│       ├── K_add.cpp
│       ├── K_apply_galois.cpp
│       ├── K_mov.cpp
│       ├── K_mulConst.cpp
│       ├── K_mulWise.cpp
│       ├── K_ntt.cpp
│       ├── lift.cpp
│       ├── mod.cpp
│       ├── mod.h
│       ├── rns.h
│       └── scale.cpp
└── tests
    └── README.md
```

## Build Setup

- Follow `README.md` in `src/client/` to recover all files needed
- Vitis: include `host.cpp` and all codes under `kernel/` and build kernel codes
- Lots of details, email me <r09922028@csie.ntu.edu.tw> if you really want to give it a try

## Run test

1. Server run the built code
2. run `client.py` under `src/client` and wait for the result


