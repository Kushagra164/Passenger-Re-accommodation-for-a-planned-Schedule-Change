# Using Quantum Computing to Solve QUBO


## Contents


  - [Quadratic Unconstrained Binary Optimization (QUBO)](#quadratic-unconstrained-binary-optimization-qubo)
  - [Using Quantum Computing to Solve QUBO](#using-quantum-computing-to-solve-qubo-1)
  - [D-Wave Systems and D-Wave Leap](#d-wave-systems-and-d-wave-leap)
  - [Annealing and Adiabatic Quantum Computing](#annealing-and-adiabatic-quantum-computing)
  - [Optimization Using Multithreading to Solve Subgraphs Simultaneously](#optimization-using-multithreading-to-solve-subgraphs-simultaneously)

![-----------------------------------------------------](../Assets/rainbow.png)

## Quadratic Unconstrained Binary Optimization (QUBO)

- **Definition:**
  - QUBO: Quadratic Unconstrained Binary Optimization.
  - It is a mathematical framework for optimization with binary variables (0 or 1).
  - Objective: Minimize or maximize a quadratic objective function.

- **Mathematical Formulation:**
  - General QUBO equation:
    
    $$\min_{x} f(x) = ∑_{i=1}^{N} q_i x_i + ∑_{i=1}^{N}∑_{j=i+1}^{N} q_{ij} x_i x_j$$
    
  - where \($x_i$\) is a binary variable, \($q_i$\) is the linear coefficient, and \($q_{ij}$\) is the quadratic coefficient.

## Using Quantum Computing to Solve QUBO

- **Quantum Annealing:**
  - Approach in quantum computing for optimization problems, including QUBO.
  - Quantum system evolves from higher-energy state to lower-energy states to find an optimal solution.
  - Process governed by a quantum Hamiltonian, gradually changing over time.

- **Mapping to Ising Model:**
  - QUBO problems mapped to Ising model.
  - Transformation involves mapping binary variables to spins and QUBO to Ising Hamiltonian.

## D-Wave Systems and D-Wave Leap

- **D-Wave Systems:**
  - Company specializing in quantum computing.
  - Quantum processors use quantum annealing for optimization problems.

- **D-Wave Leap:**
  - D-Wave's cloud-based service for remote access to quantum processors.
  - Users submit problems in QUBO or Ising models for solving using quantum annealing.

## Annealing and Adiabatic Quantum Computing

- **Quantum Annealing Process:**
  - Quantum annealing starts in a higher-energy state and gradually evolves to a state with the lowest energy.
  - Quantum fluctuations allow exploration of different possibilities simultaneously.

- **Adiabatic Quantum Computing:**
  - Broader approach encompassing quantum annealing.
  - Relies on the adiabatic theorem; the system remains in the ground state during slow evolution between initial and final Hamiltonians.

- **Mathematical Expression (Adiabatic Theorem):**
  - Adiabatic theorem:
    
    $$E_0(t) ≤ E_1(t) ≤ E_2(t) ≤ ...$$
    
  - If a quantum system starts in its ground state, a slow enough evolution ensures it remains in the ground state if the evolution is slow compared to the energy gap.

## Optimization Using Multithreading to Solve Subgraphs Simultaneously

A sophisticated optimization framework has been developed with the aim of improving computational efficiency. This framework utilizes a concurrent and parallelized approach. The optimization issue at hand, which is distinguished by the presence of numerous subgraphs, is resolved by employing multithreading, in which a dedicated computational thread is allocated to each unique subgraph.

The formulation of Quantum Unconstrained Binary Optimization (QUBO) matrices for individual subgraphs, which encapsulate the constraints and objectives that are distinct to each subset, is an integral part of the optimization process. Following this, the D-Wave sampler is notified of simultaneous API requests, which coordinates a parallel and concurrent exploration of the solution space. The utilization of multithreaded execution not only accelerates the optimization process as a whole, but also leverages the capabilities of parallel processing to resolve complex problem instances more quickly.


--------------------------------------------
