#! python
import subprocess
import os


if __name__ == "__main__":
    cmds = [
        f'python3.9 ./experiment_fixed_batch_instances.py --exp 10 --build_path ./../cmake-build-release/src --output_file exp_1e{i}.csv --inst {10**i}'
        for i in range(2, 8)
    ]
    cur_inst = 2
    for cmd in cmds:
        try:
            print(f'Executing experiments for the 1e{cur_inst} instances')
            process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                       universal_newlines=True)
            for line in process.stdout:
                print(line, end='')
            for line in process.stderr:
                print(line, end='')
            process.wait()
        except subprocess.CalledProcessError as e:
            print(f"Error executing command: {cmd}")
            print(e)
            break
        else:
            print(f"Command executed successfully: {cmd}")
        cur_inst += 1

