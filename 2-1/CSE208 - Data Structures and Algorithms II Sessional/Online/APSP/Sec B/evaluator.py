# Takes input a cpp file and evaluates it

import os
import subprocess
import sys
import time


def evaluate_cpp(file_path, input_file, output_file):
    # Compile the C++ file
    compile_command = f"g++ {file_path} -o main"
    compile_process = subprocess.run(
        compile_command, shell=True, capture_output=True, text=True)

    if compile_process.returncode != 0:
        print("Compilation Error:")
        print(compile_process.stderr)
        return False

    # Use cmd.exe explicitly to run the program
    run_command = f'cmd.exe /c "main < {input_file} > {output_file}"'
    run_process = subprocess.run(
        run_command, shell=True, capture_output=True, text=True)

    time.sleep(1)
    if run_process.returncode != 0:
        print("Runtime Error:")
        print(run_process.stderr)
        return False

    return True


def compare_output(output_file, expected_output_file):
    with open(output_file, 'r') as f1, open(expected_output_file, 'r') as f2:
        output = f1.read()
        expected_output = f2.read()

    output_arr = output.rstrip('\n').splitlines()
    expected_output_arr = expected_output.rstrip('\n').splitlines()

    # Remove trailing spaces from each line
    output_arr = [line.rstrip() for line in output_arr]
    expected_output_arr = [line.rstrip() for line in expected_output_arr]

    for i in range(len(output_arr)):
        if i >= len(expected_output_arr):
            print(f"> Line {i+1} mismatch:")
            return False

        elif output_arr[i] != expected_output_arr[i]:
            print(f"> Line {i+1} mismatch:")
            print(f"> Expected: {expected_output_arr[i]}")
            print(f"> Got: {output_arr[i]}")
            return False

    return True


def main():
    # Example usage
    # test cases are in the same directory as the script. test1.txt, test2.txt, test3.txt and so one.
    # expected output is in the same directory as the script. ans1.txt, ans2.txt, ans3.txt and so one.
    # Path to the C++ file to be evaluated -- take as command line argument
    if len(sys.argv) != 2:
        print("Usage: python evaluate.py <path_to_cpp_file>")
        sys.exit(1)

    cpp_file = sys.argv[1]

    success = 0
    for i in range(1, 11):
        input_file = f"test{i}.txt"
        expected_output_file = f"ans{i}.txt"
        output_file = f"output{i}.txt"

        if evaluate_cpp(cpp_file, input_file, output_file):
            if compare_output(output_file, expected_output_file):
                print(f"Test case {i} passed!")
                success += 1
            else:
                print(f"Test case {i} failed!")
        else:
            print(f"Test case {i} encountered an error.")

    print(f"Total test cases passed: {success}/10")


if __name__ == "__main__":
    main()
