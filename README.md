<h1 align = "center"> Passenger Re-accommodation for a planned Schedule Change </h1>

<p align="center"> 
<img src="Images/Logo.jpeg" alt="One Page Report of Proposed Solution" height="300px" width="400px">
</p>


**This repository serves as the official submission of Team 65 for the Mphasis Problem Statement.**

<h2 id="content"> 📝 Contents </h2>

 1. [Problem Statement](#problem)
 2. [Proposed Solution](#solution)
 3. [Usage Instructions](#usage)

![-----------------------------------------------------](./Images/rainbow.png)

<h2 id="problem"> 🌟 Problem Statement </h2>

Airlines frequently adjust flight schedules, impacting passengers who need to be re-accommodated on alternative flights. This project aims to automate the analysis of schedule changes, identify affected flights and passengers, and provide optimal re-accommodation solutions.

We need to Develop a system to identify and rank optimal alternate flight solutions for passengers affected by planned schedule changes. Utilize provided rule sets to ensure solution validity, considering factors like time, ancillary services impact, and passenger priority based on type and loyalty.

The Objectives are:

1. Identify impacted flights and passengers.
2. Optimize alternate flight selection based on time, ancillary services impact, etc.
3. Prioritize passenger re-accommodation based on factors like passenger type and loyalty.
4. Allow flexibility to customize business rules for flight and passenger ranking.
5. Generate and rank re-accommodation solutions for airlines to choose from.
6. Provide flexibility in altering business rules and reproducing solutions.
7. Implement expiration date and time for solutions.


:point_right: Click <a href="https://drive.google.com/file/d/1Ppkx6Iz7R2zpg1qSGHCDRIupKZ-0LBpM/view">here</a> to get the detailed problem statement. 

![-----------------------------------------------------](./Images/rainbow.png)

<h2 id="solution"> :brain: Proposed Solution </h2>

We can streamline our problem-solving approach by reducing the complexity in the following manner.

## Problem Reduced to Graph Representation

<p align="center"> 
<img src="Images/pentagraph.png" alt="Graph Representaion" height="400px" width="600px">
</p>

Here,
- $W$ represents set of nodes denoting canceled flights.
- $U$ represents set of nodes denoting affecting PNRs.
- $D$ represents set of nodes denoting proposed flights.
- $V$ represents set of nodes denoting direct proposed flight, class.
- $C$ represents set of nodes denoting Connecting proposed flights, class.



## Deduced Constraints :

1. At most one alternative flight solution is selected for each passenger in U, representing all impacted PNR(s) which ensures an effective reallocation of alternate flights for every affected passenger. Here $x_{u,v}$ and $x_{u,c}$ are boolean representations of existence of edge in $(U, V)$ and $(U, C)$ respectively.


2. It is imperative to ensure that seat allocations do not exceed the available number of seats, maintaining a meticulous alignment between the number of seats allocated and the actual capacity.


3. Let D denote the set comprising all proposed flights for rescheduling, and W represent the set of cancelled flights. A critical constraint demands that each cancelled flight, residing within set W, be exclusively assigned to a single new rescheduled flight from the set D.


## Proposed Scoring Function

![Scoring Function](Images/ScoringFunction.png)

The proposed scoring function for Optimization is shown above :

where,
- $n_{PNR}$ = Number of PNR reallocated;
- $n_{Passengers}$ = Number of Passengers allocated through the default solution;
- $s_v$ = Score of Edges selected in (U, V);
- $s_c$ = Score of Edges selected in (U, C).
- $\lambda_c$ = Number of Flights in C.

Here, $N$, $M$ are scaling factors for each individual property used to quantify its priority.

## Reduction To QUBO

## Using Quantum Computing to Solve QUBO

### Quadratic Unconstrained Binary Optimization (QUBO)

QUBO is a mathematical framework for binary optimization, aiming to minimize or maximize a quadratic objective function expressed as:

\[ f(x) = \sum_{i=1}^{N} q_i x_i + \sum_{i=1}^{N}\sum_{j=i+1}^{N} q_{ij} x_i x_j \]

### Quantum Annealing and Ising Model Mapping

Quantum annealing, a process in quantum computing, finds optimal solutions by starting in a higher-energy state. This approach utilizes the Ising model, transforming QUBO problems into the Ising Hamiltonian.

### D-Wave Systems and Leap Hybrid Solver

D-Wave Systems, specializing in quantum computing, provides D-Wave Leap—a cloud-based service for remote quantum processor access. The Leap Hybrid Solver seamlessly combines classical and quantum processing, leveraging D-Wave's quantum processing unit (QPU) alongside classical computing.

### Adiabatic Quantum Computing

Adiabatic quantum computing, a broader approach encompassing quantum annealing, relies on the adiabatic theorem. This theorem ensures that a quantum system remains in its ground state during a slow evolution between initial and final Hamiltonians, expressed as:

\[ E_0(t) \leq E_1(t) \leq E_2(t) \leq \ldots \]

This approach enables effective problem-solving by exploring various possibilities simultaneously.


![-----------------------------------------------------](./Images/rainbow.png)

<h2 id="solution"> :iphone: Analysis </h2>



![-----------------------------------------------------](./Images/rainbow.png)

<h2 id="usage"> 📋 Usage Instructions </h2>

Check out each repository's `README.md` to know about the specific usages. 

![-----------------------------------------------------](./Images/rainbow.png)

<h2 id="usage"> ✨ Contributors </h2>

- **[Sarthak Gupta](https://www.linkedin.com/in/sg1729/) - Team Lead**
- **[Kushagra Khare](https://www.linkedin.com/in/kushagra-khare-5b76b8215/) - Team Vice Lead**
- **[Akshat Rampuria](https://www.linkedin.com/in/akshat-rampuria/)**
- **[Arihant Garg](https://www.linkedin.com/in/arihant-garg-22a535247/)**
- **[Thatparthi Ajay Reddy](https://www.linkedin.com/in/ajay-reddy-5671b2247/)**
- **[Shreyan Bagchi](https://www.linkedin.com/in/shreyan-bagchi/)**
