# Using Quantum Computing to Solve QUBO


## Contents

- [Using Quantum Computing to Solve QUBO](#using-quantum-computing-to-solve-qubo)
  - [Contents](#contents)
  - [Quadratic Unconstrained Binary Optimization (QUBO)](#quadratic-unconstrained-binary-optimization-qubo)
  - [Using Quantum Computing to Solve QUBO](#using-quantum-computing-to-solve-qubo-1)
  - [D-Wave Systems and D-Wave Leap](#d-wave-systems-and-d-wave-leap)
  - [Annealing and Adiabatic Quantum Computing](#annealing-and-adiabatic-quantum-computing)
  - [Optimization Using Multithreading to Solve Subgraphs Simultaneously](#optimization-using-multithreading-to-solve-subgraphs-simultaneously)

![-----------------------------------------------------](../Images/rainbow.png)

## Quadratic Unconstrained Binary Optimization (QUBO)

- **Definition:**
  - QUBO: Quadratic Unconstrained Binary Optimization.
  - Mathematical framework for optimization with binary variables (0 or 1).
  - Objective: Minimize or maximize a quadratic objective function.

- **Mathematical Formulation:**
  - General QUBO equation:
    
    $$\min_{x} f(x) = ∑_{i=1}^{N} q_i x_i + ∑_{i=1}^{N}∑_{j=i+1}^{N} q_{ij} x_i x_j$$
    
  - where \($x_i$\) is a binary variable, \($q_i$\) is the linear coefficient, and \($q_{ij}$\) is the quadratic coefficient.

## Using Quantum Computing to Solve QUBO

- **Quantum Annealing:**
  - Approach in quantum computing for optimization problems, including QUBO.
  - Quantum system evolves from higher-energy state to find optimal solution.
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

In the pursuit of enhanced computational efficiency, a sophisticated optimization framework has been devised, leveraging a concurrent and parallelized approach. The optimization problem at hand, characterized by multiple subgraphs, is addressed through the implementation of multithreading, where each distinct subgraph is assigned to a dedicated computational thread.

The optimization process involves the formulation of Quantum Unconstrained Binary Optimization (QUBO) matrices for individual subgraphs, encapsulating the specific constraints and objectives unique to each subset. Subsequently, simultaneous API requests are dispatched to the D-Wave sampler, orchestrating a concurrent and parallelized exploration of the solution space. This multithreaded execution not only expedites the overall optimization procedure but also capitalizes on the parallel processing capabilities, allowing for a more expedient resolution of complex problem instances. The seamless integration of multithreading with quantum optimization techniques constitutes a robust and scalable methodology, poised to significantly enhance the computational efficacy of the overall optimization framework. This concurrent approach underscores a commitment to harnessing cutting-edge technologies to address intricate problem domains with unprecedented speed and efficiency.

--------------------------------------------
