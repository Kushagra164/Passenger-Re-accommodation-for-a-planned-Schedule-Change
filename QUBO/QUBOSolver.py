# Install Dwave Library
# !pip install dwave-ocean-sdk
from dwave.system import LeapHybridSampler
import concurrent.futures

# Read the QUBO input with multiple test cases
def read_test_cases(file_path):
    test_cases = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        t = int(lines[0].strip())  # Total number of test cases
        i = 1
        for _ in range(t):
            test_case_number = int(lines[i].strip())
            matrix_size = int(lines[i + 1].strip())
            qubo_matrix = [list(map(int, lines[j + i + 2].split())) for j in range(matrix_size)]
            test_cases.append((test_case_number, matrix_size, qubo_matrix))
            i += matrix_size + 2  # Move to the next test case
    return test_cases

# Solve the QUBO using D-Wave Leap Hybrid Solver for a single test case
def solve_qubo(test_case, no_samples, api_key):
    test_case_number, matrix_size, qubo_matrix = test_case
    QUBO = {(i, j): qubo_matrix[i][j] for i in range(matrix_size) for j in range(matrix_size)}
    sampler = LeapHybridSampler(token=api_key)
    Assignments = {}

    for i in range(no_samples):
        response = sampler.sample_qubo(QUBO)
        Assignments[i] = response

    return test_case_number, Assignments

def main():
    api_key = "DEV-176ed0ee355048851df48e48b37970206f5ead88"
    input_file_path = "Input.txt"
    OutputFilePath = "Output.txt"
    no_samples = 5  # Set the number of samples

    test_cases = read_test_cases(input_file_path)

    with concurrent.futures.ThreadPoolExecutor() as executor:
        # Submit each test case solver task concurrently
        futures = [executor.submit(solve_qubo, test_case, no_samples, api_key) for test_case in test_cases]

        # Wait for all tasks to complete
        concurrent.futures.wait(futures)

        # Get the results
        results = [future.result() for future in futures]

        # Do something with the results, e.g., write to output files
        
        with open(OutputFilePath, "w") as file:
            for test_case_number, result in results:
                file.writelines(str(test_case_number))
                # print(test_case_number)
                for i in range(no_samples):
                    file.writelines(str(result[i].first.sample))

if __name__ == "__main__":
    main()
