from dwave.system import LeapHybridCQMSampler
import dimod
import os
from dotenv import load_dotenv
import concurrent.futures
import argparse

# Read the QUBO input with multiple test cases
def read_test_cases(file_path):
    test_cases = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        t = int(lines[0].strip())  # Total number of test cases
        i = 1
        for _ in range(t):
            test_case_number = int(lines[i].strip())
            i+=1
            size_constraints = int(lines[i].strip())
            i+=1
            constraints = []
            for j in range(size_constraints):
                w = int(lines[i].strip())
                i += 1
                constraints.append([list(map(int, lines[i].split())),w])
                i += 1
            matrix_size = int(lines[i].strip())
            qubo_matrix = [list(map(int, lines[j + i + 1].split())) for j in range(matrix_size)]
            test_cases.append((test_case_number, matrix_size, constraints, qubo_matrix))
            i += matrix_size + 1  # Move to the next test case
    return test_cases

# Solve the Optimisation using D-Wave Leap Hybrid Solver for a single test case
def solve(test_case, api_key):
    test_case_number, matrix_size, constraints, qubo_matrix = test_case
    QUBO = {(i, j): qubo_matrix[i][j] for i in range(matrix_size) for j in range(matrix_size) if qubo_matrix[i][j]!=0}
    t = [dimod.Binary(f"{i}") for i in range(matrix_size)]
    cqm = dimod.ConstrainedQuadraticModel()
    cqm.set_objective(
        dimod.quicksum(
            t[i]*t[j]*qubo_matrix[i][j] 
                for (i,j) in QUBO.keys()
        )
    )
    # print(cqm)
    for constraint in constraints:
        cqm.add_constraint(dimod.quicksum(t[i]*constraint[0][i] for i in range(len(constraint[0])) if constraint[0][i]>0) <= constraint[1])
    sampler = LeapHybridCQMSampler(token = api_key)
    sampleset = sampler.sample_cqm(cqm, time_limit = 15)                
    feasible_sampleset = sampleset.filter(lambda row: row.is_feasible)   
    print(test_case_number, "{} feasible solutions of {}.".format(len(feasible_sampleset), len(sampleset)))    

    # print(test_case_number, feasible_sampleset.first.energy)
    
    return test_case_number, feasible_sampleset.first.sample

def main():
    parser = argparse.ArgumentParser(description="Solve QUBO problems using D-Wave Leap Hybrid Solver.")
    parser.add_argument("--input_file_path", required=True, help="input for qubo")
    parser.add_argument("--output_file_path", required=True, help="input for qubo")
    args = parser.parse_args()

    load_dotenv(override=True)

    api_key = os.getenv('API_KEY')
    input_file_path = args.input_file_path
    output_file_path = args.output_file_path

    test_cases = read_test_cases(input_file_path)

    with concurrent.futures.ThreadPoolExecutor() as executor:
        # Submit each test case solver task concurrently
        futures = [executor.submit(solve, test_case, api_key) for test_case in test_cases]

        # Wait for all tasks to complete
        concurrent.futures.wait(futures)

        # Get the results
        results = [future.result() for future in futures]

        # Do something with the results, e.g., write to output files
        with open(output_file_path, "w") as file:
            file.write(str(len(test_cases))+"\n")
            for test_case_number, result in results:
                n = len(result)
                file.write(str(n)+"\n")
                print(result)
                for j in range(n):
                    print(test_case_number, j)
                    file.write(str(int(result[str(j)])))
                file.write("\n")
            file.close()

if __name__ == "__main__":
    main()
