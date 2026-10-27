from importlib.util import spec_from_loader, module_from_spec
from importlib.machinery import SourceFileLoader
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
first_file_path = os.path.join(current_dir, "2205167_first.py")

spec = spec_from_loader("first_module", SourceFileLoader("first_module", first_file_path))
first_module = module_from_spec(spec)
spec.loader.exec_module(first_module)

Signal = first_module.Signal
LTI_System = first_module.LTI_System


def read_file(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    first_line = lines[0].strip().split()
    n_start = int(first_line[0])
    n_end = int(first_line[1])
    
    values = list(map(float, lines[1].strip().split()))
    
    max_range = max(abs(n_start), abs(n_end))
    INF = max_range + 10
    
    signal = Signal(INF)
    for i, n in enumerate(range(n_start, n_end+1)):
        if i < len(values):
            signal.set_value_at_time(n, values[i])
    
    return signal


if __name__ == "__main__":
    input_signal = read_file("input_signal.txt")
    input_signal.plot("Noisy Input Signal")
    
    h = Signal(input_signal.INF)
    h.set_value_at_time(-2, 1/5)
    h.set_value_at_time(-1, 1/5)
    h.set_value_at_time(0, 1/5)
    h.set_value_at_time(1, 1/5)
    h.set_value_at_time(2, 1/5)
    
    smoothing_system = LTI_System(h) 
    smoothed_signal = smoothing_system.output(input_signal)
    smoothed_signal.plot("Smoothed Output Signal")
