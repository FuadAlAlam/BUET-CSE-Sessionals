import numpy as np
import matplotlib.pyplot as plt

class Signal:
    def __init__(self, INF):
        self.INF = INF
        self.values = np.zeros(2*INF+1)
    
    def _index(self, t):
        return t + self.INF
    
    def _time(self, index):
        return index - self.INF
    
    def get_value_at_time(self, t):
        if t >= -self.INF and t <= self.INF:
            return self.values[self._index(t)]
        return 0
    
    def set_value_at_time(self, t, value):
        if t >= -self.INF and t <= self.INF:
            self.values[self._index(t)] = value
    
    def shift(self, k):
        shifted_signal = Signal(self.INF)
        for n in range(-self.INF, self.INF+1):
            if n-k >= -self.INF and n-k <= self.INF:
                shifted_signal.set_value_at_time(n, self.get_value_at_time(n-k))
        return shifted_signal
    
    def add(self, other: Signal):
        max_INF = max(self.INF, other.INF)
        sum_signal = Signal(max_INF)
        for n in range(-max_INF, max_INF+1):
            if n >= -self.INF and n <= self.INF:
                val1 = self.get_value_at_time(n)
            else:
                val1 = 0
            if n >= -other.INF and n <= other.INF:
                val2 = other.get_value_at_time(n)
            else:
                val2 = 0
            sum_signal.set_value_at_time(n, val1+val2)
        return sum_signal
    
    def multiply(self, scalar):
        scaled_signal = Signal(self.INF)
        for n in range(-self.INF, self.INF+1):
            scaled_signal.set_value_at_time(n, scalar*self.get_value_at_time(n))
        return scaled_signal
    
    def plot(self, title="Discrete Signal"):
        n = np.arange(-self.INF, self.INF+1)

        plt.figure(figsize=(10, 4))
        plt.stem(n, self.values, basefmt=" ")
        plt.xlabel('n')
        plt.ylabel('x(n)')
        plt.title(title)
        plt.grid(True)
        plt.tight_layout()
        plt.show()


class LTI_System:
    def __init__(self, impulse_response: Signal):
        self.impulse_response = impulse_response
    
    def linear_combination_of_impulses(self, input_signal: Signal):
        impulses = []
        coefficients = []
        
        for k in range(-input_signal.INF, input_signal.INF+1):
            coefficient = input_signal.get_value_at_time(k)
            if coefficient != 0:
                impulse = Signal(input_signal.INF)
                impulse.set_value_at_time(k, 1)
                
                impulses.append(impulse)
                coefficients.append(coefficient)
        
        return impulses, coefficients
    
    def output(self, input_signal: Signal):
        impulses, coefficients = self.linear_combination_of_impulses(input_signal)
        
        output_INF = input_signal.INF + self.impulse_response.INF
        output_signal = Signal(output_INF)
        
        for i, (impulse, coefficient) in enumerate(zip(impulses, coefficients)):
            for k in range(-impulse.INF, impulse.INF+1):
                if impulse.get_value_at_time(k) == 1:
                    shifted_h = self.impulse_response.shift(k)
                    
                    scaled_shifted = Signal(output_INF)
                    for n in range(-output_INF, output_INF+1):
                        if n >= -shifted_h.INF and n <= shifted_h.INF:
                            val = shifted_h.get_value_at_time(n)
                        else:
                            val = 0
                        scaled_shifted.set_value_at_time(n, coefficient*val)
                    
                    output_signal = output_signal.add(scaled_shifted)
                    break
        
        return output_signal


if __name__ == "__main__":
    INF = 10

    # Input signal x(n)
    x = Signal(INF)
    x.set_value_at_time(-2, 1)
    x.set_value_at_time(0, 2)
    x.set_value_at_time(3, -1)

    x.plot("Input Signal x(n)")

    # Impulse response h(n)
    h = Signal(INF)
    h.set_value_at_time(0, 1)
    h.set_value_at_time(1, 0.5)

    h.plot("Impulse Response h(n)")

    # LTI System
    system = LTI_System(h)

    # Output
    y = system.output(x)
    y.plot("Output Signal y(n)")
