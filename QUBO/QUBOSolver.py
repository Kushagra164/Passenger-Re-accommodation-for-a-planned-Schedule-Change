from dwave.system import LeapHybridSampler
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
            initial_state_list = list(map(int, lines[i].split()))
            initial_state = {i: initial_state_list[i] for i in range(len(initial_state_list))}
            matrix_size = int(lines[i + 1].strip())
            qubo_matrix = [list(map(int, lines[j + i + 2].split())) for j in range(matrix_size)]
            test_cases.append((test_case_number, matrix_size, initial_state,qubo_matrix))
            i += matrix_size + 2  # Move to the next test case
    return test_cases

# Solve the QUBO using D-Wave Leap Hybrid Solver for a single test case
def solve_qubo(test_case, api_key):
    test_case_number, matrix_size, initial_state,qubo_matrix = test_case
    QUBO = {(i, j): qubo_matrix[i][j] for i in range(matrix_size) for j in range(matrix_size) if qubo_matrix[i][j]!=0}
    sampler = LeapHybridSampler(token = api_key, initial_states = [initial_state])
    response = sampler.sample_qubo(QUBO, time_limit = 20)

    initial_energy = sum([qubo_matrix[i][j] for i in range(matrix_size) for j in range(matrix_size) if initial_state[i]==1 and initial_state[j]==1] )
    result = [response.first.sample[i] for i in range(matrix_size)]
    calculated_energy = sum([qubo_matrix[i][j] for i in range(matrix_size) for j in range(matrix_size) if result[i]==1 and result[j]==1])
    print(test_case_number, initial_energy, calculated_energy, response.first.energy)
    
    return test_case_number, response.first.sample

def main():
    parser = argparse.ArgumentParser(description="Solve QUBO problems using D-Wave Leap Hybrid Solver.")
    parser.add_argument("--env_file", required=True, help="env file required for api key")
    parser.add_argument("--input_file_path", required=True, help="input for qubo")
    parser.add_argument("--output_file_path", required=True, help="input for qubo")
    args = parser.parse_args()

    load_dotenv(args.env_file)

    api_key = os.getenv('API_KEY')
    input_file_path = args.input_file_path
    output_file_path = args.output_file_path

    test_cases = read_test_cases(input_file_path)

    with concurrent.futures.ThreadPoolExecutor() as executor:
        # Submit each test case solver task concurrently
        futures = [executor.submit(solve_qubo, test_case, api_key) for test_case in test_cases]

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
                for j in range(n):
                    file.write(str(result[j]))
                file.write("\n")
            file.close()

if __name__ == "__main__":
    main()
