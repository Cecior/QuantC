# Project Analysis

## 1. Objective and some History

This project did not start with the ambition of building the best or fastest quantum simulator, nor with the intention of competing with existing frameworks; the primary objective was to satisfy a personal curiosity about how quantum states evolve when quantum gates are applied.  
To achieve this, I first studied how quantum states are represented mathematically, how quantum gates operate on them, and finally how the application of each gate could be computationally optimized. 

Before starting this project I knew very little about High Performance Computing or Quantum Computing, but thought this could be a good opportunity to dive into both worlds.

From the beginning, I followed an iterative approach: think, write, optimize, repeat.  
There is also an important trial-and-error component. The first draft of the project included classes for the quantum state and gates, since I wanted to expand the project so that it could handle whole quantum circuits.  
Predictably enough, that didn't work. The gates became more and more problematic to manage, and the whole codebase started to look messy.  
And that's what led me to completely redesign everything.  
More than once.  
In the end, the final and current implementation is focused only on the repeated application and benchmarking of a single gate for better analysis.

And it was until this version was ready and running that I was victim of an early assumption my poor brain made at the very beginning and never really dismissed:

> "Quantum Computing is a complicated field, so the main complexity factor must be computational power"

Then, I remembered that the number of amplitudes scales exponentially with the number of qubits, and therefore, memory was the real problem.  
*Curtain falls* 

## 2. Setup & Profiling tools

To better understand what was going on, and *why* it was going on, I had to run some tests and look for evidence. But before that, I would like to introduce the absolute protagonist and unwilling victim of my experiments: my trusted laptop.

### The Laptop

My dear companion features the following:
* **CPU**: Intel i5-12500H (12 Cores: 4 Performance Cores, 8 Efficient Cores). It also has 18 MiB of L3 Cache. As we will se later, a many-qubit state vector is not particularly impressed
* **RAM**: 2x8 GB of DDR4 memory @ 3200 MHz
* **GPU**: NVIDIA RTX 3050 Laptop. Equipped with 2048 CUDA Cores, ~4 GB of VRAM, a 128-bit memory bus and a 1.5 MiB L2 Cache

Certainly not the fastest hardware, but it surely serves the purpose.

### Running on the Laptop

To collect accurate measurements and metrics about the execution of this kernel, I relied on three profiling tools:

* **Linux `perf`**: very useful to measure instructions, CPU cycles and Instructions per Cycle (IPC). It also strongly suggested the CPU might not be so hard-working, at least in this codebase
* **Intel PCM (Performance Counter Monitor)**: complements `perf` by exposing memory traffic and bandwidth information, making it possible to better understand where the data is flowing
* **NVIDIA Nsight Compute (`ncu`)**: definitely one of the most detailed tools I've used so far. It allowed me to inspect the data traffic on the GPU, the VRAM usage and compare the metrics to the roofline model

## 3. General Approach

Every implementation currently present in the kernel shares some common features, and we will see them in a moment.  

### The State Vector

Based on my pre-development studies, a state vector of $n$ qubits is represented by $2^n$ complex amplitudes. I will later re-learn this when seeing the complete memory-bound behaviour of the system.

Back at it, if there are $2^n$ complex amplitudes and I store every real or imaginary part as a double, we will end up with $2^{n+1}$ double values. Each gate-applying function will then receive the state vector as a pointer to double, pointing to a memory area of $2^{n+1}$ elements, where the $i$-th element represent a part of the $\lfloor \frac{i}{2} \rfloor$-th complex amplitude. The mapping follows this rule:

* **Even indices** ($i = 0, 2, 4 \dots)$ store the **Real** part of the amplitude
* **Odd indices** ($i = 1, 3, 5 \dots)$ store the **Complex** part of the amplitude

Truth be told, the very first implementation stored the state vector as a vector of std::complex values, but for the sake of reproducibility that was later removed.

### The Algorithm

When applying a single-qubit gate to a specific `target` qubit, the algorithm doesn't just process all the states sequentially.  
It pairs and updates simultaneously states that differ only by the value of the `target` qubit, more specifically the state where the target is $0$ ($|x0y\rangle$) and the state where the target is $1$ ($|x1y\rangle$), where $x$ and $y$ represent the values of the qubits to the left and right.

All implementations in this project follow a simple, fundamental sequence:

1. **Calculate the stride**: Determine the memory distance between the states $|x0y\rangle$ and $|x1y\rangle$. Using bitwise operations, this can be done with `stride = 1ULL << target`
2. **Outer loop (prefixes)**: Iterate over all possible binary configurations of the qubits to the *left* of the target ($x$)
3. **Inner loop (suffixes)**: Iterate over all possible binary configurations of the qubits to the *right* of the target ($y$)
4. **Address computation**: Calculate the exact memory indices for both the real and imaginary components of the two interacting amplitudes
5. **Apply superposition**: Read the current amplitudes, calculate the superposition of the real and imaginary parts, and store them in the correct memory address

Each execution of the kernel iterates over all the selected qubits, and for each one of them applies the gate `iter` times.  
See [README.md](README.md) for more specifics.

## 4. Step 1: The Naive Approach (C++ Raw)

Ok, we're finally getting to the heart of it.  
This implementation acts exactly as stated in [The Algorithm](#the-algorithm). Nothing more, nothing less

### Benchmarks

**IMPORTANT INFO OVER HERE**: I might have forgotten to state it before, but the program can automatically measure the average execution time for each qubit the gate is applied to.  
If you read the [README.md](README.md) you already knew that, but it can't hurt to remember it.

So, 
## 5. Step 2: Help! An army of threads is approaching (OpenMP)
## 6. Step 3: SIMD City? (AVX)
## 7. Step 4: Not Only Graphics (CUDA)